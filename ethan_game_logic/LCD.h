#ifndef __STM32L476G_DISCOVERY_LCD_H
#define __STM32L476G_DISCOVERY_LCD_H
#include "stm32l476xx.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#define bool _Bool

void LCD_Initialization(void);
void LCD_bar(void);
void LCD_Clock_Init(void);
void LCD_PIN_Init(void);
void LCD_Configure(void);
void LCD_Clear(void);

void LCD_DisplayNum(uint32_t number);
void LCD_WriteChar(uint8_t* ch, bool point, bool colon, uint8_t position);
static void LCD_Conv_Char_Seg(uint8_t* c,bool point,bool colon, uint8_t* digit);
void LCD_Display_Name(void);

#endif /* __STM32L476G_DISCOVERY_LCD_H */
