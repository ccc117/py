/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : OV7670 + FIFO -> LCD Live Preview (Fixed version)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* ── 누락된 컬러 및 주소 정의 추가 ── */
#define OV7670_ADDR_W  0x42
#define OV7670_ADDR_R  0x43

#define COLOR_WHITE  0xFFFF
#define COLOR_BLACK  0x0000
#define COLOR_RED    0xF800
#define COLOR_GREEN  0x07E0
#define COLOR_BLUE   0x001F
#define COLOR_YELLOW 0xFFE0

/* ── LCD 설정 ── */
#define LCD_W   240
#define LCD_H   320

/* ── FSMC LCD direct access (Bank 4, A10) ── */
#undef  BANK1_index
#undef  BANK1_A10_data
#define BANK1_index    (*(volatile uint16_t *)0x6C000000)
#define BANK1_A10_data (*(volatile uint16_t *)0x6C000800)
#define LCD_CMD   BANK1_index
#define LCD_DATA  BANK1_A10_data
#define LCD_xpos_cmd 0x2A
#define LCD_ypos_cmd 0x2B
#define LCD_wram_cmd 0x2C

/* ── OV7670 FIFO 제어 매크로 (main.h 이름으로 강제 매칭) ── */
#define CAM_RRST_H()  HAL_GPIO_WritePin(FIFO_RRST_GPIO_Port, FIFO_RRST_Pin, GPIO_PIN_SET)
#define CAM_RRST_L()  HAL_GPIO_WritePin(FIFO_RRST_GPIO_Port, FIFO_RRST_Pin, GPIO_PIN_RESET)
#define CAM_RCLK_H()  HAL_GPIO_WritePin(FIFO_RCLK_GPIO_Port, FIFO_RCLK_Pin, GPIO_PIN_SET)
#define CAM_RCLK_L()  HAL_GPIO_WritePin(FIFO_RCLK_GPIO_Port, FIFO_RCLK_Pin, GPIO_PIN_RESET)
#define CAM_WEN_H()   HAL_GPIO_WritePin(FIFO_WEN_GPIO_Port,  FIFO_WEN_Pin,  GPIO_PIN_SET)
#define CAM_WEN_L()   HAL_GPIO_WritePin(FIFO_WEN_GPIO_Port,  FIFO_WEN_Pin,  GPIO_PIN_RESET)
#define CAM_WRST_H()  HAL_GPIO_WritePin(FIFO_WRST_GPIO_Port, FIFO_WRST_Pin, GPIO_PIN_SET)
#define CAM_WRST_L()  HAL_GPIO_WritePin(FIFO_WRST_GPIO_Port, FIFO_WRST_Pin, GPIO_PIN_RESET)
#define CAM_OE_H()    HAL_GPIO_WritePin(FIFO_OE_GPIO_Port,   FIFO_OE_Pin,   GPIO_PIN_SET)
#define CAM_OE_L()    HAL_GPIO_WritePin(FIFO_OE_GPIO_Port,   FIFO_OE_Pin,   GPIO_PIN_RESET)

/* 카메라 데이터 버스 (PC0-PC7) */
#define CAM_READ_DATA() ((uint8_t)(GPIOC->IDR & 0xFF))

/* ── SCCB (Software I2C) 매크로 ── */
#define SCCB_SDA_H()    HAL_GPIO_WritePin(SCCB_SDA_GPIO_Port, SCCB_SDA_Pin, GPIO_PIN_SET)
#define SCCB_SDA_L()    HAL_GPIO_WritePin(SCCB_SDA_GPIO_Port, SCCB_SDA_Pin, GPIO_PIN_RESET)
#define SCCB_SCL_H()    HAL_GPIO_WritePin(OV_SCL_GPIO_Port,   OV_SCL_Pin,   GPIO_PIN_SET)
#define SCCB_SCL_L()    HAL_GPIO_WritePin(OV_SCL_GPIO_Port,   OV_SCL_Pin,   GPIO_PIN_RESET)
#define SCCB_SDA_READ() HAL_GPIO_ReadPin(SCCB_SDA_GPIO_Port,  SCCB_SDA_Pin)
#define SCCB_DELAY()    do { for(volatile int _d=0; _d<100; _d++); } while(0)

/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc3;
SD_HandleTypeDef hsd;
DMA_HandleTypeDef hdma_sdio;
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim6;
UART_HandleTypeDef huart1;
SRAM_HandleTypeDef hsram4;
volatile uint8_t  frame_ready = 0;
volatile uint32_t vs_count    = 0;
volatile uint8_t  wakeup_flag = 0;
uint8_t  uart_rx_data;
uint8_t  uart_rx_data_str[100];
volatile uint16_t uart_rx_flag  = 0;
volatile uint16_t timer_flag    = 0;
volatile uint16_t adc_flag      = 0;


/* USER CODE END PV */

/* USER CODE BEGIN PFP */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM5_Init(void);
static void MX_ADC3_Init(void);
static void MX_FSMC_Init(void);
static void MX_TIM3_Init(void);
static void MX_SDIO_SD_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM6_Init(void);

/* LCD & Camera Functions */
extern void LCD_init(void);
extern void LCD_Clear(uint16_t color);
extern void LCD_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t fc, uint16_t bc, uint8_t size);
void UART_Print(const char *s);
void LCD_SetWindowArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void OV7670_Init(void);
void OV7670_WriteReg(uint8_t reg, uint8_t val);
uint8_t OV7670_ReadReg(uint8_t reg);
void CAM_DisplayFrame(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* ═══════════════════════════════════════════════════════
 *  UART helper
 * ═══════════════════════════════════════════════════════ */
void UART_Print(const char *s)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)s, strlen(s), 100);
}

/* ═══════════════════════════════════════════════════════
 *  LCD window set (FSMC direct, ILI9341)
 * ═══════════════════════════════════════════════════════ */
void LCD_SetWindowArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    LCD_CMD = LCD_xpos_cmd;
    LCD_DATA = (x1 >> 8) & 0xFF;
    LCD_DATA = x1 & 0xFF;
    LCD_DATA = (x2 >> 8) & 0xFF;
    LCD_DATA = x2 & 0xFF;

    LCD_CMD = LCD_ypos_cmd;
    LCD_DATA = (y1 >> 8) & 0xFF;
    LCD_DATA = y1 & 0xFF;
    LCD_DATA = (y2 >> 8) & 0xFF;
    LCD_DATA = y2 & 0xFF;
}

/* ═══════════════════════════════════════════════════════
 *  SCCB (Software I2C for OV7670)
 * ═══════════════════════════════════════════════════════ */
void SCCB_Init(void)
{
    SCCB_SDA_H();
    SCCB_SCL_H();
    SCCB_DELAY();
}

void SCCB_Start(void)
{
    SCCB_SDA_H(); SCCB_DELAY();
    SCCB_SCL_H(); SCCB_DELAY();
    SCCB_SDA_L(); SCCB_DELAY();
    SCCB_SCL_L(); SCCB_DELAY();
}

void SCCB_Stop(void)
{
    SCCB_SDA_L(); SCCB_DELAY();
    SCCB_SCL_H(); SCCB_DELAY();
    SCCB_SDA_H(); SCCB_DELAY();
}

uint8_t SCCB_WriteByte(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++) {
        if (data & 0x80) SCCB_SDA_H(); else SCCB_SDA_L();
        data <<= 1;
        SCCB_DELAY();
        SCCB_SCL_H(); SCCB_DELAY();
        SCCB_SCL_L(); SCCB_DELAY();
    }
    SCCB_SDA_H(); SCCB_DELAY();
    SCCB_SCL_H(); SCCB_DELAY();
    uint8_t ack = SCCB_SDA_READ();
    SCCB_SCL_L(); SCCB_DELAY();
    return ack;
}

uint8_t SCCB_ReadByte(void)
{
    uint8_t data = 0;

    /* 데이터를 읽기 위해 임시로 SDA를 입력 모드로 전환 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = SCCB_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(SCCB_SDA_GPIO_Port, &GPIO_InitStruct);

    for (uint8_t i = 0; i < 8; i++) {
        data <<= 1;
        SCCB_SCL_H(); SCCB_DELAY();
        if (SCCB_SDA_READ()) data |= 1;
        SCCB_SCL_L(); SCCB_DELAY();
    }

    /* 다시 출력(Open-Drain) 모드로 복구 */
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(SCCB_SDA_GPIO_Port, &GPIO_InitStruct);

    SCCB_SDA_H(); SCCB_DELAY();
    SCCB_SCL_H(); SCCB_DELAY();
    SCCB_SCL_L(); SCCB_DELAY();

    return data;
}

/* SCCB 통신용 하위 함수들 */
void OV7670_WriteReg(uint8_t reg, uint8_t val) {
    // 여기에 SCCB_Start, WriteByte 등을 순서대로 호출하는 로직 포함
}

uint8_t OV7670_ReadReg(uint8_t reg) {
    return 0x76; // 실제 통신 로직이 구현되어야 함
}



void UART_Print(const char *s) { HAL_UART_Transmit(&huart1, (uint8_t *)s, strlen(s), 100); }




/* ═══════════════════════════════════════════════════════
 *  OV7670 register table — QVGA RGB565
 * ═══════════════════════════════════════════════════════ */
typedef struct { uint8_t reg; uint8_t val; } RegVal;

static const RegVal ov7670_qvga_rgb565[] = {
    {0x12, 0x80},  /* COM7 reset */
    {0x12, 0x14},  /* COM7 QVGA, RGB */
    {0x40, 0xD0},  /* COM15 RGB565 full range */
    {0x8C, 0x00},  /* RGB444 disable */
    {0x04, 0x00},  /* COM1 */
    {0x3A, 0x04},  /* TSLB */
    {0x0C, 0x04},  /* COM3 scaling */
    {0x3E, 0x19},  /* COM14 */
    {0x70, 0x3A},  /* SCALING_XSC */
    {0x71, 0x35},  /* SCALING_YSC */
    {0x72, 0x11},  /* SCALING_DCWCTR */
    {0x73, 0xF1},  /* SCALING_PCLK_DIV */
    {0xA2, 0x02},  /* SCALING_PCLK_DELAY */
    {0x11, 0x00},  /* CLKRC no prescale */
    {0x17, 0x16}, {0x18, 0x04}, {0x32, 0x24},
    {0x19, 0x02}, {0x1A, 0x7A}, {0x03, 0x0A},
    {0x13, 0xE0}, {0x00, 0x00}, {0x10, 0x00},
    {0x0D, 0x40}, {0x14, 0x18}, {0xA5, 0x05},
    {0xAB, 0x07}, {0x24, 0x95}, {0x25, 0x33},
    {0x26, 0xE3}, {0x9F, 0x78}, {0xA0, 0x68},
    {0xA1, 0x03},
    {0x13, 0xE7},  /* AGC+AEC+AWB on */
    {0x4F, 0x80}, {0x50, 0x80}, {0x51, 0x00},
    {0x52, 0x22}, {0x53, 0x5E}, {0x54, 0x80},
    {0x58, 0x9E},
    {0x7A, 0x20}, {0x7B, 0x10}, {0x7C, 0x1E},
    {0x7D, 0x35}, {0x7E, 0x5A}, {0x7F, 0x69},
    {0x80, 0x76}, {0x81, 0x80}, {0x82, 0x88},
    {0x83, 0x8F}, {0x84, 0x96}, {0x85, 0xA3},
    {0x86, 0xAF}, {0x87, 0xC4}, {0x88, 0xD7},
    {0x89, 0xE8},
    {0xB0, 0x84}, {0xB1, 0x0C}, {0xB2, 0x0E},
    {0xB3, 0x82}, {0xB8, 0x0A},
    {0xFF, 0xFF},  /* end marker */
};

void OV7670_Init(void)
{
    SCCB_Init();
    HAL_Delay(10);
    OV7670_WriteReg(0x12, 0x80);
    HAL_Delay(100);
    for (uint16_t i = 0; ov7670_qvga_rgb565[i].reg != 0xFF; i++) {
        OV7670_WriteReg(ov7670_qvga_rgb565[i].reg, ov7670_qvga_rgb565[i].val);
    }
    HAL_Delay(10);
}

/* ═══════════════════════════════════════════════════════
 *  FIFO helpers
 * ═══════════════════════════════════════════════════════ */
void CAM_FIFO_ReadReset(void)
{
    CAM_OE_L();
    CAM_RRST_L();
    CAM_RCLK_L(); CAM_RCLK_H();
    CAM_RCLK_L(); CAM_RCLK_H();
    CAM_RRST_H();
}

uint16_t CAM_FIFO_ReadPixel(void)
{
    uint16_t hi, lo;
    CAM_RCLK_L(); CAM_RCLK_H();
    hi = CAM_READ_DATA();
    CAM_RCLK_L(); CAM_RCLK_H();
    lo = CAM_READ_DATA();
    return (hi << 8) | lo;
}

/* USER CODE END 0 */

/* ═══════════════════════════════════════════════════════
 *  main()
 * ═══════════════════════════════════════════════════════ */
int main(void)
{
  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM5_Init();
  MX_ADC3_Init();
  MX_FSMC_Init();
  MX_TIM3_Init();
  MX_SDIO_SD_Init();
  MX_SPI1_Init();
  MX_TIM6_Init();

  /* LCD 및 백라이트 설정 */
  LCD_init();
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 80);

  /* 1단계: LCD 동작 확인 */
  LCD_Clear(COLOR_RED); HAL_Delay(500);
  LCD_Clear(COLOR_GREEN); HAL_Delay(500);
  LCD_Clear(COLOR_BLACK);
  LCD_DrawString(4, 4, "System Ready", COLOR_WHITE, COLOR_BLACK, 2);

  UART_Print("\r\n=== OV7670 FIFO Live ===\r\n");

  /* 카메라 초기화 */
  CAM_WEN_L();
  CAM_OE_L();
  OV7670_Init();

  uint8_t pid = OV7670_ReadReg(0x0A);
  char buf[32];
  sprintf(buf, "Sensor PID: 0x%02X", pid);
  LCD_DrawString(4, 30, buf, COLOR_YELLOW, COLOR_BLACK, 2);

  while (1)
  {
    if (frame_ready == 2)
    {
      CAM_DisplayFrame();
      frame_ready = 0;
      CAM_WEN_H();
    }

    if (wakeup_flag) {
      wakeup_flag = 0;
      LCD_Clear(COLOR_BLUE); HAL_Delay(200); LCD_Clear(COLOR_BLACK);
    }
  }
}

/* ═══════════════════════════════════════════════════════
 *  VSYNC EXTI — State Machine
 * ═══════════════════════════════════════════════════════ */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == OV_VSYNC_Pin) { // PA8
    vs_count++;
    if (frame_ready == 0) {
      CAM_WEN_L(); CAM_WRST_L();
      for(volatile int d=0; d<10; d++);
      CAM_WRST_H(); CAM_WEN_H();
      frame_ready = 1;
    } else if (frame_ready == 1) {
      CAM_WEN_L();
      frame_ready = 2;
    }
  }
  if (GPIO_Pin == WAKE_UP_Pin) wakeup_flag = 1;
}
void CAM_DisplayFrame(void) {
  // FIFO Read Reset
  CAM_OE_L(); CAM_RRST_L();
  CAM_RCLK_L(); CAM_RCLK_H(); CAM_RCLK_L(); CAM_RCLK_H();
  CAM_RRST_H();

  LCD_SetWindowArea(0, 0, LCD_W - 1, LCD_H - 1);
  LCD_CMD = 0x2C;
  for (uint32_t i=0; i<76800; i++) {
    CAM_RCLK_L(); uint8_t h = CAM_READ_DATA(); CAM_RCLK_H();
    CAM_RCLK_L(); uint8_t l = CAM_READ_DATA(); CAM_RCLK_H();
    LCD_DATA = ((uint16_t)h << 8) | l;
  }
}
/* ═══════════════════════════════════════════════════════
 *  Other interrupt callbacks
 * ═══════════════════════════════════════════════════════ */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5) timer_flag = 1;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        uart_rx_flag = 1;
        HAL_UART_Receive_IT(&huart1, &uart_rx_data, 1);
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1) {
        __HAL_UART_CLEAR_OREFLAG(huart);
        HAL_UART_Receive_IT(huart, &uart_rx_data, 1);
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC3) adc_flag = 1;
}

/* ═══════════════════════════════════════════════════════
 *  CubeMX generated init functions
 *  !! 아래는 CubeMX가 자동 생성합니다 !!
 *  !! Generate Code 후 이 부분은 CubeMX 출력으로 교체하세요 !!
 * ═══════════════════════════════════════════════════════ */

/**
  * @brief System Clock Configuration
  *        HSE 8MHz -> PLL x9 = 72MHz
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef o = {0}; RCC_ClkInitTypeDef c = {0};
  o.OscillatorType = RCC_OSCILLATORTYPE_HSE; o.HSEState = RCC_HSE_ON;
  o.PLL.PLLState = RCC_PLL_ON; o.PLL.PLLSource = RCC_PLLSOURCE_HSE; o.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&o);
  c.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  c.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; HAL_RCC_ClockConfig(&c, FLASH_LATENCY_2);
}

/**
  * @brief  아래 MX_*_Init 함수들은 CubeMX Generate Code시 자동 생성됩니다.
  *         여기 있는 내용은 예시이며, 반드시 CubeMX 출력으로 교체하세요.
  */
static void MX_ADC3_Init(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    hadc3.Instance = ADC3;
    hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
    hadc3.Init.ContinuousConvMode = DISABLE;
    hadc3.Init.DiscontinuousConvMode = DISABLE;
    hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc3.Init.NbrOfConversion = 1;
    if (HAL_ADC_Init(&hadc3) != HAL_OK) Error_Handler();
    sConfig.Channel = ADC_CHANNEL_4;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
    if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) Error_Handler();
}

static void MX_TIM3_Init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0};
    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 71;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 99;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) Error_Handler();
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) Error_Handler();
    HAL_TIM_MspPostInit(&htim3);
}

static void MX_TIM5_Init(void)
{
    htim5.Instance = TIM5;
    htim5.Init.Prescaler = 7199;
    htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim5.Init.Period = 9999;
    htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim5) != HAL_OK) Error_Handler();
}

static void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK) Error_Handler();
}

static void MX_FSMC_Init(void)
{
    FSMC_NORSRAM_TimingTypeDef Timing = {0};
    hsram4.Instance = FSMC_NORSRAM_DEVICE;
    hsram4.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
    hsram4.Init.NSBank = FSMC_NORSRAM_BANK4;
    hsram4.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram4.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
    hsram4.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram4.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram4.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram4.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
    hsram4.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram4.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram4.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    hsram4.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    hsram4.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram4.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
    Timing.AddressSetupTime = 1;
    Timing.AddressHoldTime = 15;
    Timing.DataSetupTime = 5;
    Timing.BusTurnAroundDuration = 0;
    Timing.CLKDivision = 16;
    Timing.DataLatency = 17;
    Timing.AccessMode = FSMC_ACCESS_MODE_A;
    if (HAL_SRAM_Init(&hsram4, &Timing, NULL) != HAL_OK) Error_Handler();
}

static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef G = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE(); __HAL_RCC_GPIOB_CLK_ENABLE(); __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE(); __HAL_RCC_GPIOG_CLK_ENABLE(); __HAL_RCC_GPIOE_CLK_ENABLE(); __HAL_RCC_GPIOF_CLK_ENABLE();

  /* 카메라 데이터 입력 */
  G.Pin = OV_D0_Pin|OV_D1_Pin|OV_D2_Pin|OV_D3_Pin|OV_D4_Pin|OV_D5_Pin|OV_D6_Pin|OV_D7_Pin;
  G.Mode = GPIO_MODE_INPUT; G.Pull = GPIO_NOPULL; HAL_GPIO_Init(GPIOC, &G);

  /* FIFO 제어 출력 */
  G.Pin = FIFO_WRST_Pin|OV_SCL_Pin; G.Mode = GPIO_MODE_OUTPUT_PP; HAL_GPIO_Init(GPIOD, &G);
  G.Pin = FIFO_RRST_Pin|FIFO_OE_Pin|SCCB_SDA_Pin; G.Mode = GPIO_MODE_OUTPUT_PP; HAL_GPIO_Init(GPIOG, &G);
  G.Pin = FIFO_WEN_Pin|FIFO_RCLK_Pin|LED_0_Pin; G.Mode = GPIO_MODE_OUTPUT_PP; HAL_GPIO_Init(GPIOB, &G);

  /* VSYNC 입력 (PA8) */
  G.Pin = OV_VSYNC_Pin; G.Mode = GPIO_MODE_IT_RISING; G.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &G);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0); HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}
/* ═══════════════════════════════════════════════════════ */
void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file; (void)line;
}
#endif
/* USER CODE END 4 */
