#ifndef _LCD_H_
#define _LCD_H_

#include "stdint.h"
#include "st7789.h"
		
#define FONT_MENU_GAME							Font_16x26
#define LETTER_MENU_GAME_COLOR					YELLOW
#define LETTER_MODE_GAME_COLOR					RED
#define BACKGROUND_MENU_GAME_COLOR			    BLACK
#define FONT_OPTION_FUNCTION					Font_11x18
#define FONT_STATS								Font_7x10

void LCD_Menu_Game(void);
void LCD_Waiting_Display(void);
void LCD_Action(void);
void LCD_Monster_Appearance(void);
void LCD_Player_Appearance(void);
void LCD_Stats_Appearance(void);
void LCD_Endless_Map_Page(void);
void LCD_Select_Mode(uint8_t select_mode);
void LCD_Deselect_Mode(uint8_t deselect_mode);
void LCD_Select_Action(uint8_t select_action);
void LCD_Deselect_Action(uint8_t deselect_action);
void LCD_Campaign_Map_Page(void);


#endif // _LCD_H_