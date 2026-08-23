#ifndef _TFT_H_
#define _TFT_H_

#include "stdint.h"
#include "st7789.h"
		
#define FONT_MENU_GAME									Font_16x26
#define LETTER_MENU_GAME_COLOR					YELLOW
#define LETTER_MODE_GAME_COLOR					RED
#define BACKGROUND_MENU_GAME_COLOR			BLACK
#define FONT_OPTION_FUNCTION						Font_11x18
#define FONT_STATS											Font_7x10

void Menu_Game(void);
void Waiting_Display(void);
void Ui_Action();
void Ui_Monster_Appearance();
void Ui_Player_Appearance();
void Ui_Stats_Appearance();
void Ui_Endless_Page();
void Ui_Setting_Page();
void Ui_Select_Mode(uint8_t select_mode);
void Ui_Deselect_Mode(uint8_t deselect_mode);
void Ui_Select_Action(uint8_t select_action);
void Ui_Deselect_Action(uint8_t deselect_action);



#endif // _TFT_H_