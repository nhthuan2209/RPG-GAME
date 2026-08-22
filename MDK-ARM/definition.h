#ifndef _DEFINITION_H_
#define _DEFINITION_H_

#include <main.h>
#include <stdint.h>
#include "st7789.h"

#define RESET_DISPLAY 									ST7789_Fill_Color(BLACK);			
#define ST7789_RST_PORT 								GPIOB
#define ST7789_RST_PIN  								GPIO_PIN_1
#define ST7789_DC_PORT  								GPIOB
#define ST7789_DC_PIN   								GPIO_PIN_0

#define ST7789_CS_PORT  								GPIOA
#define ST7789_CS_PIN   								GPIO_PIN_4
#define FONT_MENU_GAME									Font_16x26
#define LETTER_MENU_GAME_COLOR					YELLOW
#define LETTER_MODE_GAME_COLOR					RED
#define BACKGROUND_MENU_GAME_COLOR			BLACK
#define FONT_OPTION_FUNCTION						Font_11x18
#define FONT_STATS											Font_7x10




#endif // _DEFINITION_H_