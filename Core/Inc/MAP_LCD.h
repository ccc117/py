/*
 * MAP_LCD.h
 *
 *  Created on: Jan 12, 2024
 *      Author: MAP Lab
 */

#ifndef INC_MAP_LCD_H_
#define INC_MAP_LCD_H_

#include "MAP.h"

#define BANK1_index		*((uint16_t *)0x6c000000)
#define BANK1_A10_data	*((uint16_t *)0x6c000800)

#define LCD_wram_cmd	0x2c
#define LCD_xpos_cmd	0x2a
#define LCD_ypos_cmd	0x2b



#define LCD_width	240
#define LCD_height	320


#define COLOR_BLACK	0x0000
#define COLOR_BLUE	0x001f
#define COLOR_CYAN	0x07ff
#define COLOR_GREEN	0x07e0
#define COLOR_RED	0xf800
#define COLOR_MAGENTA	0xf81f
#define COLOR_YELLOW	0xffe0
#define COLOR_WHITE	0xffff

#define LCD_PRINT_BUF_SIZE	128

void LCD_BL_on();
void LCD_BL_off();

void LCD_Check_ID();

void LCD_init();
void LCD_SetPos(uint16_t xPos, uint16_t yPos);
void LCD_Clear(uint16_t Color);
void LCD_Draw_Point(uint16_t x, uint16_t y, uint16_t Color);
void LCD_DrawChar(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg, uint8_t size);
void LCD_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size);

#endif /* INC_MAP_LCD_H_ */
