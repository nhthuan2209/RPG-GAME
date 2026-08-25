#include "ui.h"

#include "stdio.h"

#include "user.h"
#include "definition.h"
#include "button.h"
#include "player.h"
#include "monster.h"
#include "battle.h"
#include "campaign.h"
#include "rfid.h"
#include "lcd.h"

uint8_t card = 0;
int8_t current_player = -1;
uint8_t select_mode = 1;
uint8_t select_action = 1;

static uint32_t map_endless = 0; 
static uint8_t battle_mode = 0;
static Player *battle_player = 0;
static Monster   *battle_monster = 0;

UiPage ui_page = UI_PAGE_WAITING;

void UI_HandleWaitingForRfidCard(void)
{
	if (ui_page == UI_PAGE_WAITING)
	{
		uint8_t uid[RFID_UID_LEN];
 
		if (Rfid_TryReadUid(uid)) {
			current_player = User_FindUid(uid);
			if (current_player >= 0) {
				ui_page = UI_PAGE_MENU;
				RESET_DISPLAY;
				LCD_Menu_Game();
			}
		}
	}
}

void UI_Move_Action(uint8_t *sl_action)
{
	if(Button_Down())
	{
		HAL_Delay(100);
		LCD_Deselect_Action(*sl_action);
		if (*sl_action < 3)
		{
			(*sl_action)++;
		}
		LCD_Select_Action(*sl_action);
	}
	if(Button_Up())
	{
		HAL_Delay(100);
		LCD_Deselect_Action(*sl_action);
		if (*sl_action > 1)
		{
			(*sl_action)--;
		}
		LCD_Select_Action(*sl_action);
	}
}

#define UI_WRITE_STAT(x, y, text) 			ST7789_WriteString(x, y, text, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)

void UI_Player_Stats(Player *player)
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

void UI_Monster_Stats(Monster *monster)
{
	char save[20];
	
	sprintf(save, "%3d/%3d", monster->hp, monster->max_hp);
	UI_WRITE_STAT(260, 130, save);
	
	sprintf(save, "%3d", monster->attack);
	UI_WRITE_STAT(270, 140, save);
	
	sprintf(save, "%5d", monster->exp_reward);
	UI_WRITE_STAT(270, 150, save);
}

void UI_Update_Stats(Player *player, Monster *monster)
{
	UI_Player_Stats(player);
	UI_Monster_Stats(monster);
}

static void UI_Show_Campaign(void)
{
	LCD_Campaign_Map_Page();
	ui_page = UI_PAGE_BATTLE;
	battle_mode = 0;
	battle_player = &player_list[current_player];
	battle_monster = &monster_list[campaign[current_state].monster_id];
	Battle_Start(battle_player, battle_monster);
	UI_Display_Battle_Stat();
}

#define UI_WRITE_ANNOUNCEMENT(x, y, text)					ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

static void UI_Show_Endless(void)
{
	char announe_nextmap[20];
	
	HAL_Delay(800);
	sprintf(announe_nextmap, "MAP:%d", map_endless + 1);
	UI_WRITE_ANNOUNCEMENT(100, 80, announe_nextmap);
	
	RESET_DISPLAY;
	ui_page = UI_PAGE_BATTLE;
	battle_mode = 1;
	battle_player = &player_list[current_player];
	battle_monster = &monster_list[map_endless % monster_count];
	Battle_Start(battle_player, battle_monster);
	UI_Display_Battle_Stat();
}

void UI_Show_Setting(Player *player)
{
	char save[20];
	
	UI_WRITE_ANNOUNCEMENT(10, 10, "CHARACTER");

	sprintf(save, "Name: %s", player->name);
	UI_WRITE_STAT(10, 35, save);
	
	sprintf(save, "Health: %d/%d", player->hp, player->max_hp);
	UI_WRITE_STAT(10, 55, save);
	
	sprintf(save, "Strength: %d", player->attack);
	UI_WRITE_STAT(10, 75, save);
	
	sprintf(save, "Defense: %d", player->defense);
	UI_WRITE_STAT(10, 95, save);
	
	sprintf(save, "Experience: %d", player->exp);
	UI_WRITE_STAT(10, 115, save);
	
	sprintf(save, "Level: %d", player->level);
	UI_WRITE_STAT(10, 135, save);
	
	UI_WRITE_ANNOUNCEMENT(120, 70, "PRESS SELECT");
	UI_WRITE_ANNOUNCEMENT(145, 100, "TO RESET");
}

static uint8_t setting_confirm = 0;
static uint32_t setting_confirm_wait = 0;

void UI_Reset_Stat(void)
{
	if (setting_confirm && HAL_GetTick() >= setting_confirm_wait)
	{
		setting_confirm = 0;
	}
	if (!Button_Select())
	{
		return;
	}
	if(setting_confirm == 0)
	{
		RESET_DISPLAY;
		UI_WRITE_ANNOUNCEMENT(40, 60, "PRESS AGAIN TO");
		UI_WRITE_ANNOUNCEMENT(40, 85, " RESET STATS");
		HAL_Delay(1000);
		RESET_DISPLAY;
		UI_Show_Setting(&player_list[current_player]);
		
    setting_confirm = 1;
    setting_confirm_wait = 3000 + HAL_GetTick();		
	}
	else
	{
		setting_confirm = 0;
		Player_Reset(&player_list[current_player]);
	
		RESET_DISPLAY;
		UI_WRITE_ANNOUNCEMENT(70, 70, "RESET COMPLETE");
		HAL_Delay(1000);
		RESET_DISPLAY;
		ui_page = UI_PAGE_MENU;
		LCD_Menu_Game();
	}
}

#define UI_WRITE_RESULT(x, y, text) 							ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

static void UI_Battle_End(uint8_t win)
{
	battle_player = 0;
	battle_monster = 0;
	
	RESET_DISPLAY;
	
	if(win)
	{
		UI_WRITE_RESULT(90, 100, "YOU WIN");
		HAL_Delay(2000);
		if(battle_mode == 1)
		{
			map_endless++;
			RESET_DISPLAY;
			UI_Show_Endless();
			return;
		}
		
		if (current_state >= campaign_state_count - 1)
		{
			RESET_DISPLAY;
			UI_WRITE_RESULT(75, 100, "CAMPAIGN CLEAR!");
			HAL_Delay(2000);

			Campaign_Reset();
			RESET_DISPLAY;
			ui_page = UI_PAGE_MENU;
			LCD_Menu_Game();
			return;			
		}
		Campaign_NextStage();
		RESET_DISPLAY;
		UI_Show_Campaign();
	}
	else
	{
		UI_WRITE_RESULT(90, 100, "YOU LOSE");
		HAL_Delay(2000);
	
		if(battle_mode == 1)
		{
			char result[20];
			RESET_DISPLAY;
			sprintf(result, "Reached:%d map", map_endless + 1);
			UI_WRITE_RESULT(70, 90, result);
			
			HAL_Delay(2000);
			map_endless = 0;
			RESET_DISPLAY;
			ui_page = UI_PAGE_MENU;
			LCD_Menu_Game();
			return;			
		}
		Campaign_Reset();
		RESET_DISPLAY;
		ui_page = UI_PAGE_MENU;
		LCD_Menu_Game();
	}
}

void UI_Display_Battle_Stat(void)
{
	ST7789_Fill_Color(BLACK);
	LCD_Monster_Appearance();
	LCD_Player_Appearance();
	LCD_Stats_Appearance();
	LCD_Action();
	if(battle_player != 0 && battle_monster != 0)
	{
		UI_Update_Stats(battle_player, battle_monster);
	}	
}

static uint8_t select_locked = 0;
void UI_Battle_Page(void)
{
	if(battle_player == 0 || battle_monster == 0)
	{
		return;
	}
	if(Battle_GetState() == BATTLE_VICTORY || Battle_GetState() == BATTLE_DEFEAT)
	{
		return;
	}
	LCD_Select_Action(select_action);
	UI_Move_Action(&select_action);
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
			UI_Update_Stats(battle_player, battle_monster);
	 
			if(Battle_GetState() == BATTLE_VICTORY)
			{
				Player_GainExp(battle_player, battle_monster->exp_reward);
				UI_Battle_End(1);
				return;
			}
			HAL_Delay(1000);
			Battle_Attack_Player();
			UI_Update_Stats(battle_player, battle_monster);
			
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				UI_Battle_End(0);
				return;
			}
		}
		else if (select_action == 2)
		{
			Battle_Defending();
			UI_Update_Stats(battle_player, battle_monster);
			
			HAL_Delay(400);
      Battle_Attack_Player();        
	UI_Update_Stats(battle_player, battle_monster);
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				UI_Battle_End(0);
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
				UI_Display_Battle_Stat();
			}
			else
			{
				Battle_Heal();
				UI_Update_Stats(battle_player, battle_monster);
				
				HAL_Delay(400);
				Battle_Attack_Player();        
				UI_Update_Stats(battle_player, battle_monster);
				if(Battle_GetState() == BATTLE_DEFEAT)
				{
					UI_Battle_End(0);
					return;
				}			
			}
		}
	}
}

void UI_Confirm_Mode(uint8_t mode)
{
	if(Button_Select())
	{
		switch(mode)
		{
			case 1:
				RESET_DISPLAY;
				UI_Show_Campaign();
				break;
			case 2:
				RESET_DISPLAY;
				LCD_Endless_Map_Page();
				UI_Show_Endless();
				break;
			case 3:
				RESET_DISPLAY;
				ui_page = UI_PAGE_SETTING;
				UI_Show_Setting(&player_list[current_player]);
				break;
		}
	}
}

void UI_Move_Mode(uint8_t *sl_mode)
{
	if(Button_Down())
	{
		HAL_Delay(100);
		LCD_Deselect_Mode(*sl_mode);
		if (*sl_mode < 3)
		{
			(*sl_mode)++;
		}
		LCD_Select_Mode(*sl_mode);
	}
	if(Button_Up())
	{
		HAL_Delay(100);
		LCD_Deselect_Mode(*sl_mode);
		if (*sl_mode > 1)
		{
			(*sl_mode)--;
		}
		LCD_Select_Mode(*sl_mode);
	}
}

void UI_Back_MenuGame(void)
{
	if(Button_Back())
	{
		RESET_DISPLAY;
		ui_page = UI_PAGE_MENU;
		LCD_Menu_Game();
	}
}
