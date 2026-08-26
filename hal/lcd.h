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

void LCD_Draw_Menu_Game(void);
void LCD_Draw_Waiting_Display(void);
void LCD_Action(void);
void LCD_Draw_Monster_Appearance(void);
void LCD_Draw_Player_Appearance(void);
void LCD_AttackEffect(uint8_t player_attacks);
void LCD_Draw_Stats(void);
void LCD_Draw_HealthBar(uint16_t x, uint16_t y, uint16_t width, int32_t hp, int32_t max_hp, uint16_t color);
void LCD_Draw_Endless_Map(void);
void LCD_Select_Mode(uint8_t select_mode);
void LCD_Deselect_Mode(uint8_t deselect_mode);
void LCD_Select_Action(uint8_t select_action);
void LCD_Deselect_Action(uint8_t deselect_action);
void LCD_Draw_Campaign_Map(void);


#endif // _LCD_H_