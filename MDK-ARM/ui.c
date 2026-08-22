#include "stdio.h"

#include "rc522.h"
#include "user.h"
#include "st7789.h"
#include "definition.h"
#include "ui.h"
#include "button.h"
#include "character.h"
#include "add_monster.h"
#include "battle.h"
#include "campaign.h"


uint8_t card = 0;
uint8_t game_page = 0;
int8_t current_player = -1;
uint8_t select_mode = 1;
uint8_t select_action = 1;

static Character *battle_player = 0;
static Monster   *battle_monster = 0;


uint8_t Read_Card()
{
	if(MFRC522_Request(PICC_REQIDL, uid) == MI_OK)
	{
		if(MFRC522_Anticoll(uid) == MI_OK)
		{
			current_player = Find_Uid(uid);
			if(current_player >= 0)
			{
				return 1;
			}
		}
	}
	return 0;
}

void Menu_Game(void)
{
	ST7789_WriteString(85, 20, "RFID GAME", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(85, 60, "CAMPAIGN", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(85, 100, "ENDLESS", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(85, 140, "SETTING", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
}
void Waiting_Display(void)
{
	ST7789_WriteString(85, 20, "RFID GAME", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(50, 60, " PLEASE SCAN", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(50, 100, "  YOUR CARD", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(110, 140, "RFID", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
}
void Check_Card(void)
{
	if(game_page == 0)
	{
		if(Read_Card())
		{
			game_page = 1;
			ST7789_Fill_Color(BLACK);
			Menu_Game();
		}
	}
	else if(game_page == 1)
	{
		
	}
}
void Ui_Select_Mode(uint8_t select_mode)
{
	switch(select_mode)
	{
		case 1:
			ST7789_WriteString(85, 60, "CAMPAIGN", FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
		case 2:
			ST7789_WriteString(85, 100, "ENDLESS", FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
		case 3:
			ST7789_WriteString(85, 140, "SETTING", FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
	}
}

void Ui_Deselect_Mode(uint8_t deselect_mode)
{
	switch(deselect_mode)
	{
		case 1:
			ST7789_WriteString(85, 60, "CAMPAIGN", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
		case 2:
			ST7789_WriteString(85, 100, "ENDLESS", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
		case 3:
			ST7789_WriteString(85, 140, "SETTING", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
	}
}

void Ui_Select_Action(uint8_t select_action)
{
	switch(select_action)
	{
		case 1:
			ST7789_WriteString(100, 40, "[ATTACK]", FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
		case 2:
			ST7789_WriteString(100, 70, "[DEFENSE]", FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
		case 3:
			ST7789_WriteString(100, 100, "[RUN]", FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
	}	
}

void Ui_Deselect_Action(uint8_t deselect_action)
{
	switch(deselect_action)
	{
		case 1:
			ST7789_WriteString(100, 40, "[ATTACK]", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
		case 2:
			ST7789_WriteString(100, 70, "[DEFENSE]", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
		case 3:
			ST7789_WriteString(100, 100, "[RUN]", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			break;
	}	
}

void Ui_Move_Action(uint8_t *sl_mode)
{
	if(Button_Down())
	{
		HAL_Delay(100);
		Ui_Deselect_Mode(*sl_mode);
		if (*sl_mode < 3)
		{
			(*sl_mode)++;
		}
		Ui_Select_Mode(*sl_mode);
	}
	if(Button_Up())
	{
		HAL_Delay(100);
		Ui_Deselect_Mode(*sl_mode);
		if (*sl_mode > 1)
		{
			(*sl_mode)--;
		}
		Ui_Select_Mode(*sl_mode);
	}
}

void Ui_Action()
{
	ST7789_WriteString(100, 40, "[ATTACK]", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(100, 70, "[DEFENSE]", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(100, 100, "[RUN]", FONT_OPTION_FUNCTION, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
}


void Ui_Monster_Appearance()
{
	ST7789_DrawCircle(250, 50, 10, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(250, 60, 250, 90, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(250, 90, 230, 120, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(250, 90, 270, 120, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(250, 75, 230, 75, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(250, 75, 270, 75, LETTER_MENU_GAME_COLOR);
	
}

void Ui_Player_Appearance()
{
	ST7789_DrawCircle(30, 50, 10, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(30, 60, 30, 90, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(30, 90, 10, 120, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(30, 90, 50, 120, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(30, 75, 10, 75, LETTER_MENU_GAME_COLOR);
	ST7789_DrawLine(30, 75, 50, 75, LETTER_MENU_GAME_COLOR);
}

void Ui_Stats_Appearance()
{
	ST7789_DrawLine(10, 120, 320, 120, LETTER_MENU_GAME_COLOR);
	ST7789_WriteString(10, 130, "HP: ", FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(10, 140, "ATK: ", FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(10, 20, "LEVEL: ", FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(10, 150, "EXP: ", FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	
	
	ST7789_WriteString(250, 130, "HP: ", FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(250, 140, "ATK: ", FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_WriteString(250, 150, "EXP: ", FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
}

void Ui_Player_Stats(Character *player)
{
	char save[20];
	
	sprintf(save, "%3d", player->level);
	ST7789_WriteString(40, 20, save, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	
	sprintf(save, "%3d/%3d", player->hp, player->max_hp);
	ST7789_WriteString(35, 130, save, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	
	sprintf(save, "%3d", player->attack);
	ST7789_WriteString(35, 140, save, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	
	sprintf(save, "%5d", player->exp);
	ST7789_WriteString(30, 150, save, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
}

void Ui_Monster_Stats(Monster *monster)
{
	char save[20];
	
	sprintf(save, "%3d/%3d", monster->hp, monster->max_hp);
	ST7789_WriteString(260, 130, save, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	
	sprintf(save, "%3d", monster->attack);
	ST7789_WriteString(270, 140, save, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	
	sprintf(save, "%5d", monster->exp_reward);
	ST7789_WriteString(270, 150, save, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
}

void Ui_Update_Stats(Character *player, Monster *monster)
{
	Ui_Player_Stats(player);
	Ui_Monster_Stats(monster);
}

void Ui_Battle_Page(void)
{
	if(battle_player == 0 || battle_monster == 0)
	{
		return;
	}
	Ui_Select_Action(select_action);
	Ui_Move_Action(&select_action);
	if(Button_Select())
	{
		HAL_Delay(100);
		
		if(select_action == 1)
		{
			Battle_Player_Attack();
			Ui_Update_Stats(battle_player, battle_monster);
	 
			if(Battle_GetState() == BATTLE_VICTORY)
			{
				ST7789_WriteString(90, 200, "YOU WIN!", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
				return;
			}
 
			HAL_Delay(400);
			Battle_Monster_Attack();
			Ui_Update_Stats(battle_player, battle_monster);
	 
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				ST7789_WriteString(90, 200, "YOU LOSE!", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			}
		}
	}
}


void Ui_Campaign_Page()
{
	ST7789_WriteString(10, 10, "CAMPAIGN", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_DrawLine(40, 70, 40, 120, WHITE);
	ST7789_WriteString(10, 120, "Forest", FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	ST7789_DrawLine(50, 60, 90, 60, WHITE);
	ST7789_DrawCircle(40, 60, 10, WHITE);
	
	ST7789_DrawLine(110, 60, 150, 60, WHITE);
	ST7789_DrawCircle(100, 60, 10, WHITE);
	
	ST7789_DrawLine(160, 70, 160, 110, WHITE);
	ST7789_DrawCircle(160, 60, 10, WHITE);
	
	ST7789_DrawLine(170, 120, 210, 120, WHITE);
	ST7789_DrawCircle(160, 120, 10, WHITE);
	
	ST7789_DrawLine(220, 70, 220, 110, WHITE);
	ST7789_DrawCircle(220, 120, 10, WHITE);
	
	ST7789_WriteString(200, 65, "Boss", FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
	
	HAL_Delay(100);
	ST7789_Fill_Color(BLACK);
	Ui_Monster_Appearance();
	Ui_Player_Appearance();
	Ui_Stats_Appearance();
	Ui_Action();
	if(battle_player != 0 && battle_monster != 0)
	{
		Ui_Update_Stats(battle_player, battle_monster);
	}
}

void Ui_Endless_Page()
{
	ST7789_WriteString(50,50, "Red", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
}

void Ui_Setting_Page()
{
	ST7789_WriteString(50,50, "Red", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
}

void Ui_Confirm_Mode(uint8_t mode)
{
	if(Button_Select())
	{
		switch(mode)
		{
			case 1:
				RESET_DISPLAY;
				game_page = 2;
				battle_player = &character_list[current_player];
				battle_monster = &monster_list[campaign[current_state].monster_id];
				Battle_Start(battle_player, battle_monster);
				Ui_Campaign_Page();
				break;
			case 2:
				RESET_DISPLAY;
				game_page = 3;
				Ui_Endless_Page();
				break;
			case 3:
				RESET_DISPLAY;
				game_page = 4;
				Ui_Setting_Page();
				break;
		}
	}
}

void Ui_Move_Mode(uint8_t *sl_mode)
{
	if(Button_Down())
	{
		HAL_Delay(100);
		Ui_Deselect_Mode(*sl_mode);
		if (*sl_mode < 3)
		{
			(*sl_mode)++;
		}
		Ui_Select_Mode(*sl_mode);
	}
	if(Button_Up())
	{
		HAL_Delay(100);
		Ui_Deselect_Mode(*sl_mode);
		if (*sl_mode > 1)
		{
			(*sl_mode)--;
		}
		Ui_Select_Mode(*sl_mode);
	}
}

