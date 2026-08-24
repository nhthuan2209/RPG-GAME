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
	TFT_WRITE_OPTION(100, 100, "[HEAL]");
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

void Ui_Endless_Map_Page()
{
	TFT_WRITE_MENU(10, 10, "ENDLESS");
	TFT_WRITE_LINE(40, 70, 40, 100);
	TFT_WRITE_OPTION(23, 100, "You");
	
	TFT_WRITE_LINE(50, 60, 90, 60);
	TFT_WRITE_CIRCLE(40, 60, 10);
	
	TFT_WRITE_LINE(110, 60, 150, 60);
	TFT_WRITE_CIRCLE(100, 60, 10);
	
	TFT_WRITE_LINE(170, 60, 210, 60);
	TFT_WRITE_CIRCLE(160, 60, 10);
	
	TFT_WRITE_CIRCLE(220, 60, 10);
	
	TFT_WRITE_LINE(220, 70, 220, 110);
	TFT_WRITE_CIRCLE(220, 120, 10);

	TFT_WRITE_LINE(230, 120, 260, 120);
	TFT_WRITE_OPTION(260, 113, "Boss");
	
	TFT_WRITE_OPTION(10, 140, "This is infinite loop mode");
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
			TFT_WRITE_SELECT(100, 100, "[HEAL]");
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
			TFT_WRITE_DESELECT(100, 100, "[HEAL]");
			break;
	}	
}

void Ui_Campaign_Map_Page()
{
	TFT_WRITE_MENU(10, 10, "CAMPAIGN");
	TFT_WRITE_LINE(40, 70, 40, 120);
	TFT_WRITE_OPTION(10, 120, "Forest");
	TFT_WRITE_LINE(50, 60, 90, 60);
	TFT_WRITE_CIRCLE(40, 60, 10);
	
	TFT_WRITE_LINE(110, 60, 150, 60);
	TFT_WRITE_CIRCLE(100, 60, 10);
	
	TFT_WRITE_LINE(160, 70, 160, 110);
	TFT_WRITE_CIRCLE(160, 60, 10);
	
	TFT_WRITE_LINE(170, 120, 210, 120);
	TFT_WRITE_CIRCLE(160, 120, 10);
	
	TFT_WRITE_LINE(220, 70, 220, 110);
	TFT_WRITE_CIRCLE(220, 120, 10);

	TFT_WRITE_OPTION(200, 65, "Boss");
	
}
