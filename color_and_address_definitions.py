"""
Color and Address Definitions for OV7670 + FIFO -> LCD Live Preview

This module provides:
  - RGB565 color constants for LCD display
  - ILI9341 LCD controller command/register addresses
  - OV7670 camera register addresses
  - AL422B FIFO control pin definitions
"""

# =============================================================================
# RGB565 Color Definitions (16-bit: 5 red, 6 green, 5 blue)
# =============================================================================

WHITE = 0xFFFF
BLACK = 0x0000
RED = 0xF800
GREEN = 0x07E0
BLUE = 0x001F
YELLOW = 0xFFE0
CYAN = 0x07FF
MAGENTA = 0xF81F
ORANGE = 0xFD20
PINK = 0xFE19
GRAY = 0x8410
DARK_GREEN = 0x03E0
DARK_BLUE = 0x0010
LIGHT_BLUE = 0x7D7C
LIGHT_GREEN = 0xB7E0
LIGHT_GRAY = 0xC618
DARK_GRAY = 0x7BEF
BROWN = 0xA145
PURPLE = 0x8010


# =============================================================================
# ILI9341 LCD Controller Command Addresses
# =============================================================================

# System commands
LCD_NOP = 0x00                # No Operation
LCD_SWRESET = 0x01            # Software Reset
LCD_READ_ID = 0x04            # Read Display Identification
LCD_READ_STATUS = 0x09        # Read Display Status
LCD_READ_POWER_MODE = 0x0A    # Read Display Power Mode
LCD_SLEEP_IN = 0x10           # Enter Sleep Mode
LCD_SLEEP_OUT = 0x11          # Sleep Out
LCD_PARTIAL_ON = 0x12         # Partial Mode On
LCD_NORMAL_DISPLAY = 0x13     # Normal Display Mode On
LCD_INVERSION_OFF = 0x20      # Display Inversion OFF
LCD_INVERSION_ON = 0x21       # Display Inversion ON
LCD_GAMMA_SET = 0x26          # Gamma Set
LCD_DISPLAY_OFF = 0x28        # Display OFF
LCD_DISPLAY_ON = 0x29         # Display ON

# Addressing commands
LCD_COLUMN_ADDR = 0x2A        # Column Address Set
LCD_PAGE_ADDR = 0x2B          # Page Address Set
LCD_GRAM_WRITE = 0x2C         # Memory Write
LCD_GRAM_READ = 0x2E          # Memory Read

# Display control
LCD_PARTIAL_AREA = 0x30       # Partial Area
LCD_VSCROLL_DEF = 0x33        # Vertical Scrolling Definition
LCD_TEARING_OFF = 0x34        # Tearing Effect Line OFF
LCD_TEARING_ON = 0x35         # Tearing Effect Line ON
LCD_MAC = 0x36                # Memory Access Control
LCD_VSCROLL_ADDR = 0x37       # Vertical Scrolling Start Address
LCD_IDLE_OFF = 0x38           # Idle Mode OFF
LCD_IDLE_ON = 0x39            # Idle Mode ON
LCD_PIXEL_FORMAT = 0x3A       # Pixel Format Set
LCD_WR_MEM_CONT = 0x3C        # Write Memory Continue
LCD_RD_MEM_CONT = 0x3E        # Read Memory Continue
LCD_SET_TEAR_SCANLINE = 0x44  # Set Tear Scanline

# Panel control
LCD_RGB_INTERFACE = 0xB0      # RGB Interface Signal Control
LCD_FRAME_CTRL_NORMAL = 0xB1  # Frame Rate Control (Normal Mode)
LCD_FRAME_CTRL_IDLE = 0xB2    # Frame Rate Control (Idle Mode)
LCD_FRAME_CTRL_PARTIAL = 0xB3  # Frame Rate Control (Partial Mode)
LCD_INVERSION_CTRL = 0xB4     # Display Inversion Control
LCD_BLANK_PORCH = 0xB5        # Blanking Porch Control
LCD_DISPLAY_FUNC = 0xB6       # Display Function Control
LCD_ENTRY_MODE = 0xB7         # Entry Mode Set

# Power control
LCD_POWER1 = 0xC0             # Power Control 1
LCD_POWER2 = 0xC1             # Power Control 2
LCD_VCOM1 = 0xC5              # VCOM Control 1
LCD_VCOM2 = 0xC7              # VCOM Control 2

# ID read
LCD_READ_ID1 = 0xDA           # Read ID1
LCD_READ_ID2 = 0xDB           # Read ID2
LCD_READ_ID3 = 0xDC           # Read ID3
LCD_READ_ID4 = 0xD3           # Read ID4

# Gamma control
LCD_POS_GAMMA = 0xE0          # Positive Gamma Correction
LCD_NEG_GAMMA = 0xE1          # Negative Gamma Correction

# Extended commands
LCD_INTERFACE_CTRL = 0xF6     # Interface Control
LCD_POWER_A = 0xCB            # Power Control A
LCD_POWER_B = 0xCF            # Power Control B
LCD_TIMING_A = 0xE8           # Driver Timing Control A
LCD_TIMING_B = 0xEA           # Driver Timing Control B
LCD_POWER_SEQ = 0xED          # Power On Sequence Control
LCD_ENABLE_3G = 0xF2          # Enable 3 Gamma Control
LCD_PUMP_RATIO = 0xF7         # Pump Ratio Control


# =============================================================================
# LCD Display Dimensions and FSMC Address
# =============================================================================

LCD_WIDTH = 240
LCD_HEIGHT = 320

# STM32 FSMC (Flexible Static Memory Controller) LCD addresses
# These assume FSMC Bank 1, NOR/SRAM sub-bank 1 with A16 as RS (D/C) pin
LCD_BASE_ADDR = 0x60000000
LCD_CMD_ADDR = LCD_BASE_ADDR             # Register Select = 0 (Command)
LCD_DATA_ADDR = LCD_BASE_ADDR | 0x00020000  # Register Select = 1 (Data, A16 high)


# =============================================================================
# OV7670 Camera Register Addresses
# =============================================================================

# Device address (7-bit I2C/SCCB address)
OV7670_WRITE_ADDR = 0x42     # OV7670 write address
OV7670_READ_ADDR = 0x43      # OV7670 read address

# AGC/AEC registers
OV7670_REG_GAIN = 0x00       # AGC Gain Control
OV7670_REG_BLUE = 0x01       # AWB Blue Channel Gain
OV7670_REG_RED = 0x02        # AWB Red Channel Gain
OV7670_REG_VREF = 0x03       # Vertical Frame Control
OV7670_REG_COM1 = 0x04       # Common Control 1
OV7670_REG_BAVE = 0x05       # U/B Average Level
OV7670_REG_GBAVE = 0x06      # Y/Gb Average Level
OV7670_REG_AECHH = 0x07      # AEC MS 5 bits
OV7670_REG_RAVE = 0x08       # V/R Average Level
OV7670_REG_COM2 = 0x09       # Common Control 2
OV7670_REG_PID = 0x0A        # Product ID Number (MSB)
OV7670_REG_VER = 0x0B        # Product ID Number (LSB)
OV7670_REG_COM3 = 0x0C       # Common Control 3
OV7670_REG_COM4 = 0x0D       # Common Control 4
OV7670_REG_COM5 = 0x0E       # Common Control 5
OV7670_REG_COM6 = 0x0F       # Common Control 6
OV7670_REG_AECH = 0x10       # AEC LS 8 bits
OV7670_REG_CLKRC = 0x11      # Internal Clock
OV7670_REG_COM7 = 0x12       # Common Control 7
OV7670_REG_COM8 = 0x13       # Common Control 8
OV7670_REG_COM9 = 0x14       # Common Control 9
OV7670_REG_COM10 = 0x15      # Common Control 10

# Timing / windowing
OV7670_REG_HSTART = 0x17     # Horizontal Frame Start
OV7670_REG_HSTOP = 0x18      # Horizontal Frame Stop
OV7670_REG_VSTART = 0x19     # Vertical Frame Start
OV7670_REG_VSTOP = 0x1A      # Vertical Frame Stop
OV7670_REG_PSHFT = 0x1B      # Pixel Delay After HREF
OV7670_REG_MIDH = 0x1C       # Manufacturer ID High Byte
OV7670_REG_MIDL = 0x1D       # Manufacturer ID Low Byte
OV7670_REG_MVFP = 0x1E       # Mirror / Vertical Flip

# AEC/AGC additional
OV7670_REG_AEW = 0x24        # AGC/AEC Stable Operating Region Upper Limit
OV7670_REG_AEB = 0x25        # AGC/AEC Stable Operating Region Lower Limit
OV7670_REG_VPT = 0x26        # AGC/AEC Fast Mode Operating Region

# Output format and resolution
OV7670_REG_HREF = 0x32       # HREF Control
OV7670_REG_TSLB = 0x3A       # Line Buffer Test Option
OV7670_REG_COM11 = 0x3B      # Common Control 11
OV7670_REG_COM12 = 0x3C      # Common Control 12
OV7670_REG_COM13 = 0x3D      # Common Control 13
OV7670_REG_COM14 = 0x3E      # Common Control 14
OV7670_REG_EDGE = 0x3F       # Edge Enhancement Adjustment
OV7670_REG_COM15 = 0x40      # Common Control 15
OV7670_REG_COM16 = 0x41      # Common Control 16
OV7670_REG_COM17 = 0x42      # Common Control 17

# Matrix coefficients
OV7670_REG_MTX1 = 0x4F       # Matrix Coefficient 1
OV7670_REG_MTX2 = 0x50       # Matrix Coefficient 2
OV7670_REG_MTX3 = 0x51       # Matrix Coefficient 3
OV7670_REG_MTX4 = 0x52       # Matrix Coefficient 4
OV7670_REG_MTX5 = 0x53       # Matrix Coefficient 5
OV7670_REG_MTX6 = 0x54       # Matrix Coefficient 6
OV7670_REG_MTXS = 0x58       # Matrix Coefficient Sign

# Gamma curve
OV7670_REG_SLOP = 0x7A       # Gamma Curve Highest Segment Slope
OV7670_REG_GAM1 = 0x7B       # Gamma Curve 1st Segment Input End Point
OV7670_REG_GAM2 = 0x7C       # Gamma Curve 2nd Segment Input End Point
OV7670_REG_GAM3 = 0x7D       # Gamma Curve 3rd Segment Input End Point
OV7670_REG_GAM4 = 0x7E       # Gamma Curve 4th Segment Input End Point
OV7670_REG_GAM5 = 0x7F       # Gamma Curve 5th Segment Input End Point
OV7670_REG_GAM6 = 0x80       # Gamma Curve 6th Segment Input End Point
OV7670_REG_GAM7 = 0x81       # Gamma Curve 7th Segment Input End Point
OV7670_REG_GAM8 = 0x82       # Gamma Curve 8th Segment Input End Point
OV7670_REG_GAM9 = 0x83       # Gamma Curve 9th Segment Input End Point
OV7670_REG_GAM10 = 0x84      # Gamma Curve 10th Segment Input End Point
OV7670_REG_GAM11 = 0x85      # Gamma Curve 11th Segment Input End Point
OV7670_REG_GAM12 = 0x86      # Gamma Curve 12th Segment Input End Point
OV7670_REG_GAM13 = 0x87      # Gamma Curve 13th Segment Input End Point
OV7670_REG_GAM14 = 0x88      # Gamma Curve 14th Segment Input End Point
OV7670_REG_GAM15 = 0x89      # Gamma Curve 15th Segment Input End Point

# DSP control
OV7670_REG_DM_LNL = 0x92     # Dummy Line Low 8 Bits
OV7670_REG_DM_LNH = 0x93     # Dummy Line High 8 Bits
OV7670_REG_LCC1 = 0x62       # Lens Correction Option 1
OV7670_REG_LCC2 = 0x63       # Lens Correction Option 2
OV7670_REG_LCC3 = 0x64       # Lens Correction Option 3
OV7670_REG_LCC4 = 0x65       # Lens Correction Option 4
OV7670_REG_LCC5 = 0x66       # Lens Correction Option 5
OV7670_REG_MANU = 0x67       # Manual U Value
OV7670_REG_MANV = 0x68       # Manual V Value
OV7670_REG_GFIX = 0x69       # Fix Gain Control

OV7670_REG_SCALING_XSC = 0x70    # Test Pattern / Horizontal Scale Factor
OV7670_REG_SCALING_YSC = 0x71    # Test Pattern / Vertical Scale Factor
OV7670_REG_SCALING_DCWCTR = 0x72  # DCW Control
OV7670_REG_SCALING_PCLK_DIV = 0x73  # DSP Scale Control Clock Divider
OV7670_REG_SCALING_PCLK_DELAY = 0xA2  # Pixel Clock Delay

# Expected product ID values
OV7670_PID_EXPECTED = 0x76    # Expected PID register value
OV7670_VER_EXPECTED = 0x73    # Expected VER register value


# =============================================================================
# AL422B FIFO Control Definitions
# =============================================================================

# FIFO control signals (active levels)
FIFO_WRST_ACTIVE = 0         # Write Reset active low
FIFO_WRST_INACTIVE = 1       # Write Reset inactive
FIFO_RRST_ACTIVE = 0         # Read Reset active low
FIFO_RRST_INACTIVE = 1       # Read Reset inactive
FIFO_WEN_ENABLE = 1          # Write Enable active high
FIFO_WEN_DISABLE = 0         # Write Enable inactive
FIFO_RCK_ACTIVE = 0          # Read Clock active low
FIFO_RCK_INACTIVE = 1        # Read Clock inactive
FIFO_OE_ENABLE = 0           # Output Enable active low
FIFO_OE_DISABLE = 1          # Output Enable inactive

# OV7670 image dimensions (QVGA for LCD preview)
OV7670_QVGA_WIDTH = 320
OV7670_QVGA_HEIGHT = 240
OV7670_QQVGA_WIDTH = 160
OV7670_QQVGA_HEIGHT = 120


# =============================================================================
# Helper: Convert 24-bit RGB to RGB565
# =============================================================================

def rgb_to_rgb565(r, g, b):
    """Convert 8-bit RGB values to a 16-bit RGB565 color value.

    Args:
        r: Red component (0-255)
        g: Green component (0-255)
        b: Blue component (0-255)

    Returns:
        16-bit RGB565 color value
    """
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)


def rgb565_to_rgb(color):
    """Convert a 16-bit RGB565 color value to 8-bit RGB components.

    Args:
        color: 16-bit RGB565 color value

    Returns:
        Tuple of (r, g, b) with 8-bit values
    """
    r = (color >> 11) & 0x1F
    g = (color >> 5) & 0x3F
    b = color & 0x1F
    # Scale to 8-bit range
    r = (r << 3) | (r >> 2)
    g = (g << 2) | (g >> 4)
    b = (b << 3) | (b >> 2)
    return (r, g, b)
