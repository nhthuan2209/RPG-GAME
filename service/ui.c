#include "ui.h"

#include "stdio.h"

#include "user.h"
#include "definition.h"
#include "button.h"
#include "character.h"
#include "add_monster.h"
#include "battle.h"
#include "campaign.h"
#include "rfid.h"
#include "tft.h"

uint8_t card = 0;
int8_t current_player = -1;
uint8_t select_mode = 1;
uint8_t select_action = 1;

static Character *battle_player = 0;
static Monster   *battle_monster = 0;

UiPage ui_page = UI_PAGE_WAITING;

void Ui_PollWaitingScreen(void)
{
	if (ui_page == UI_PAGE_WAITING)
	{
		uint8_t uid[RFID_UID_LEN];
 
		if (Rfid_TryReadUid(uid)) {
			current_player = User_FindUid(uid);
			if (current_player >= 0) {
				ui_page = UI_PAGE_MENU;
				RESET_DISPLAY;
				Menu_Game();
			}
		}
	}
}

void Ui_Move_Action(uint8_t *sl_action)
{
	if(Button_Down())
	{
		HAL_Delay(100);
		Ui_Deselect_Action(*sl_action);
		if (*sl_action < 3)
		{
			(*sl_action)++;
		}
		Ui_Select_Action(*sl_action);
	}
	if(Button_Up())
	{
		HAL_Delay(100);
		Ui_Deselect_Action(*sl_action);
		if (*sl_action > 1)
		{
			(*sl_action)--;
		}
		Ui_Select_Action(*sl_action);
	}
}

#define UI_WRITE_STAT(x, y, text) 			ST7789_WriteString(x, y, text, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)

void Ui_Player_Stats(Character *player)
{
	char save[20];
	
	sprintf(save, "%3d", player->level);
	UI_WRITE_STAT(40, 20, save);
	
	sprintf(save, "%3d/%3d", player->hp, player->max_hp);
	UI_WRITE_STAT(35, 130, save);
	
	sprintf(save, "%3d", player->attack);
	UI_WRITE_STAT(35, 140, save);
	
	sprintf(save, "%5d", player->exp);
	UI_WRITE_STAT(30, 150, save);
}

void Ui_Monster_Stats(Monster *monster)
{
	char save[20];
	
	sprintf(save, "%3d/%3d", monster->hp, monster->max_hp);
	UI_WRITE_STAT(260, 130, save);
	
	sprintf(save, "%3d", monster->attack);
	UI_WRITE_STAT(270, 140, save);
	
	sprintf(save, "%5d", monster->exp_reward);
	UI_WRITE_STAT(270, 150, save);
}

void Ui_Update_Stats(Character *player, Monster *monster)
{
	Ui_Player_Stats(player);
	Ui_Monster_Stats(monster);
}

static void Ui_Show_Campaign()
{
	Ui_Campaign_Map_Page();
	ui_page = UI_PAGE_BATTLE;
	battle_player = &character_list[current_player];
	battle_monster = &monster_list[campaign[current_state].monster_id];
	Battle_Start(battle_player, battle_monster);
	Ui_Display_Battle_Stat();	
}


#define UI_WRITE_RESULT(x, y, text) 			ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

static void Ui_Battle_End(uint8_t win)
{
	battle_player = 0;
	battle_monster = 0;
	
	RESET_DISPLAY;
	
	if(win)
	{
		UI_WRITE_RESULT(90, 100, "YOU WIN");
		HAL_Delay(2000);
		if (current_state >= campaign_state_count - 1)
		{
			RESET_DISPLAY;
			UI_WRITE_RESULT(90, 100, "CAMPAIGN CLEAR!");
			HAL_Delay(2000);

			Campaign_Reset();
			RESET_DISPLAY;
			ui_page = UI_PAGE_MENU;
			Menu_Game();
			return;			
		}
		Campaign_NextStage();
		RESET_DISPLAY;
		Ui_Show_Campaign();		
	}
	else
	{
		UI_WRITE_RESULT(90, 100, "YOU LOSE");
		HAL_Delay(2000);

		Campaign_Reset();
		RESET_DISPLAY;
		ui_page = UI_PAGE_MENU;
		Menu_Game();
	}
}

void Ui_Display_Battle_Stat(void)
{
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

static uint8_t select_locked = 0;
#define UI_WRITE_ANNOUNCEMENT(x, y, text)					ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

void Ui_Battle_Page(void)
{
	if(battle_player == 0 || battle_monster == 0)
	{
		return;
	}
	if(Battle_GetState() == BATTLE_VICTORY || Battle_GetState() == BATTLE_DEFEAT)
	{
		return;
	}
	Ui_Select_Action(select_action);
	Ui_Move_Action(&select_action);
	if(!Button_Select())
	{
		select_locked = 0;   
	}
	else if(!select_locked)
	{
		select_locked = 1;
		if (select_action == 1)
		{
			Battle_Attack_Monster();
			Ui_Update_Stats(battle_player, battle_monster);
	 
			if(Battle_GetState() == BATTLE_VICTORY)
			{
				Ui_Battle_End(1);
				return;
			}
			HAL_Delay(1000);
			Battle_Attack_Player();
			Ui_Update_Stats(battle_player, battle_monster);
			
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				Ui_Battle_End(0);
				return;
			}
		}
		else if (select_action == 2)
		{
			Battle_Defending();
			Ui_Update_Stats(battle_player, battle_monster);
			
			HAL_Delay(400);
      Battle_Attack_Player();        
      Ui_Update_Stats(battle_player, battle_monster);
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				Ui_Battle_End(0);
				return;
			}
		}
		else if (select_action == 3)
		{
			if(Battle_Count_HealPotion() == 0)
			{
				UI_WRITE_ANNOUNCEMENT(30, 70, "OUT OF POTION");
				HAL_Delay(550);
				RESET_DISPLAY;
				Ui_Display_Battle_Stat();			
			}
			else
			{
				Battle_Heal();
				Ui_Update_Stats(battle_player, battle_monster);
				
				HAL_Delay(400);
				Battle_Attack_Player();        
				Ui_Update_Stats(battle_player, battle_monster);
				if(Battle_GetState() == BATTLE_DEFEAT)
				{
					Ui_Battle_End(0);
					return;
				}			
			}
		}
	}
}

void Ui_Confirm_Mode(uint8_t mode)
{
	if(Button_Select())
	{
		switch(mode)
		{
			case 1:
				RESET_DISPLAY;
				Ui_Show_Campaign();
				break;
			case 2:
				RESET_DISPLAY;
				ui_page = UI_PAGE_ENDLESS;
				Ui_Endless_Page();
				break;
			case 3:
				RESET_DISPLAY;
				ui_page = UI_PAGE_SETTING;
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

