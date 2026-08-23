#include "tft.h"


#define TFT_WRITE_MENU(x, y, text) 			ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)
#define TFT_WRITE_OPTION(x, y, text) 		ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)
#define TFT_WRITE_STAT(x, y, text) 			ST7789_WriteString(x, y, text, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)
#define TFT_WRITE_CIRCLE(x, y, r)				ST7789_DrawCircle(x, y, r, LETTER_MENU_GAME_COLOR);
#define TFT_WRITE_LINE(x1, y1, x2, y2)	ST7789_DrawLine(x1, y1, x2, y2, LETTER_MENU_GAME_COLOR);
#define TFT_WRITE_SELECT(x, y, text)		ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
#define TFT_WRITE_DESELECT(x, y, text)	ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

void Menu_Game(void)
{
	TFT_WRITE_MENU  (85, 20, "RFID GAME");
	TFT_WRITE_OPTION(85, 60, "CAMPAIGN");
	TFT_WRITE_OPTION(85, 100, "ENDLESS");
	TFT_WRITE_OPTION(85, 140, "SETTING");
}

void Waiting_Display(void)
{
	TFT_WRITE_MENU(85, 20, "RFID GAME");
	TFT_WRITE_MENU(50, 60, " PLEASE SCAN");
	TFT_WRITE_MENU(50, 100, "  YOUR CARD");
	TFT_WRITE_MENU(110, 140, "RFID");
}

void Ui_Action()
{
	TFT_WRITE_OPTION(100, 40, "[ATTACK]");
	TFT_WRITE_OPTION(100, 70, "[DEFENSE]");
	TFT_WRITE_OPTION(100, 100, "[RUN]");
}

void Ui_Monster_Appearance()
{
	TFT_WRITE_CIRCLE(250, 50, 10);
	TFT_WRITE_LINE(250, 60, 250, 90);
	TFT_WRITE_LINE(250, 90, 230, 120);
	TFT_WRITE_LINE(250, 90, 270, 120);
	TFT_WRITE_LINE(250, 75, 230, 75);
	TFT_WRITE_LINE(250, 75, 270, 75);
}

void Ui_Player_Appearance()
{
	TFT_WRITE_CIRCLE(30, 50, 10);
	TFT_WRITE_LINE(30, 60, 30, 90);
	TFT_WRITE_LINE(30, 90, 10, 120);
	TFT_WRITE_LINE(30, 90, 50, 120);
	TFT_WRITE_LINE(30, 75, 10, 75);
	TFT_WRITE_LINE(30, 75, 50, 75);
}

void Ui_Stats_Appearance()
{
	TFT_WRITE_LINE(10, 120, 320, 120);
	TFT_WRITE_STAT(10, 130, "HP: ");
	TFT_WRITE_STAT(10, 140, "ATK: ");
	TFT_WRITE_STAT(10, 20, "LEVEL: ");
	TFT_WRITE_STAT(10, 150, "EXP: ");
	
	
	TFT_WRITE_STAT(250, 130, "HP: ");
	TFT_WRITE_STAT(250, 140, "ATK: ");
	TFT_WRITE_STAT(250, 150, "EXP: ");
}

void Ui_Endless_Page()
{
	TFT_WRITE_MENU(50,50, "Red");
}

void Ui_Setting_Page()
{
	TFT_WRITE_MENU(50,50, "Red");
}
void Ui_Select_Mode(uint8_t select_mode)
{
	switch(select_mode)
	{
		case 1:
			TFT_WRITE_SELECT(85, 60, "CAMPAIGN");
			break;
		case 2:
			TFT_WRITE_SELECT(85, 100, "ENDLESS");
			break;
		case 3:
			TFT_WRITE_SELECT(85, 140, "SETTING");
			break;
	}
}

void Ui_Deselect_Mode(uint8_t deselect_mode)
{
	switch(deselect_mode)
	{
		case 1:
			TFT_WRITE_DESELECT(85, 60, "CAMPAIGN");
			break;
		case 2:
			TFT_WRITE_DESELECT(85, 100, "ENDLESS");
			break;
		case 3:
			TFT_WRITE_DESELECT(85, 140, "SETTING");
			break;
	}
}

void Ui_Select_Action(uint8_t select_action)
{
	switch(select_action)
	{
		case 1:
			TFT_WRITE_SELECT(100, 40, "[ATTACK]");
			break;
		case 2:
			TFT_WRITE_SELECT(100, 70, "[DEFENSE]");
			break;
		case 3:
			TFT_WRITE_SELECT(100, 100, "[RUN]");
			break;
	}	
}

void Ui_Deselect_Action(uint8_t deselect_action)
{
	switch(deselect_action)
	{
		case 1:
			TFT_WRITE_DESELECT(100, 40, "[ATTACK]");
			break;
		case 2:
			TFT_WRITE_DESELECT(100, 70, "[DEFENSE]");
			break;
		case 3:
			TFT_WRITE_DESELECT(100, 100, "[RUN]");
			break;
	}	
}
