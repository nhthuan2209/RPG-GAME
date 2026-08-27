#include "ui.h"

#include "stdio.h"

#include "user.h"
#include "definition.h"
#include "button.h"
#include "player.h"
#include "monster.h"
#include "battle.h"
#include "campaign.h"
#include "skill.h"
#include "rfid.h"
#include "lcd.h"

int8_t current_player = -1;
uint8_t select_mode = 1;
uint8_t select_skill = 1;
uint8_t active_skill = 0;
uint8_t select_action = 1;

#define PLAYER_MODE_COUNT 2

static uint32_t map_endless = 0; 
static uint8_t battle_mode = 0;
static Player campaign_players[PLAYER_MODE_COUNT];
static Player endless_players[PLAYER_MODE_COUNT];
static Player *battle_player = 0;
static Monster *battle_monster = 0;
static uint8_t select_option = 1;
static uint8_t gameover_select_locked = 1;

UiPage ui_page = UI_PAGE_WAITING;

void UI_HandleWaitingForRfidCard(void)
{
	if (ui_page == UI_PAGE_WAITING)
	{
		uint8_t uid[RFID_UID_LEN];
 
		if (Rfid_TryReadUid(uid, RFID_UID_LEN)) {
			current_player = User_FindUid(uid);
			if (current_player >= 0) {
				campaign_players[current_player] = player_list[current_player];
				endless_players[current_player] = player_list[current_player];
				ui_page = UI_PAGE_MENU;
				RESET_DISPLAY;
				LCD_DrawMenuGame();
			}
		}
	}
}

void UI_MoveAction(uint8_t *sl_action)
{
	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectAction(*sl_action);
		if (*sl_action < 4)
		{
			(*sl_action)++;
		}
		LCD_SelectAction(*sl_action);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectAction(*sl_action);
		if (*sl_action > 1)
		{
			(*sl_action)--;
		}
		LCD_SelectAction(*sl_action);
	}
}

#define UI_WRITE_STAT(x, y, text) 			ST7789_WriteString(x, y, text, FONT_STATS, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)

void UI_PlayerStats(Player *player)
{
	char save[20];
	
	sprintf(save, "%3d", player->level);
	UI_WRITE_STAT(40, 20, save);
	
	sprintf(save, "%3d/%3d", player->hp, player->max_hp);
	UI_WRITE_STAT(35, 130, save);
	LCD_DrawHealthBar(10, 122, 90, player->hp, player->max_hp, GREEN);
	
	sprintf(save, "%3d", player->attack);
	UI_WRITE_STAT(35, 140, save);
	
	sprintf(save, "%5d", player->exp);
	UI_WRITE_STAT(30, 150, save);
}

void UI_MonsterStats(Monster *monster)
{
	char save[20];
	
	sprintf(save, "%3d/%3d", monster->hp, monster->max_hp);
	UI_WRITE_STAT(260, 130, save);
	LCD_DrawHealthBar(220, 122, 90, monster->hp, monster->max_hp, RED);
	
	sprintf(save, "%3d", monster->attack);
	UI_WRITE_STAT(270, 140, save);
	
	sprintf(save, "%5d", monster->exp_reward);
	UI_WRITE_STAT(270, 150, save);
}

void UI_UpdateStats(Player *player, Monster *monster)
{
	UI_PlayerStats(player);
	UI_MonsterStats(monster);
}

static void UI_ShowCampaign(void)
{
	LCD_DrawCampaignMap();
	ui_page = UI_PAGE_BATTLE;
	battle_mode = 0;
	battle_player = &campaign_players[current_player];
	battle_monster = &monster_list[campaign[current_state].monster_id];
	Battle_Start(battle_player, battle_monster);
	UI_DisplayBattleStat();
}

#define UI_WRITE_ANNOUNCEMENT(x, y, text)					ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

static void UI_ShowEndless(void)
{
	char announe_nextmap[20];
	
	HAL_Delay(800);
	RESET_DISPLAY;
	sprintf(announe_nextmap, "MAP:%d", map_endless + 1);
	UI_WRITE_ANNOUNCEMENT(100, 80, announe_nextmap);
	HAL_Delay(500);
	
	RESET_DISPLAY;
	ui_page = UI_PAGE_BATTLE;
	battle_mode = 1;
	battle_player = &endless_players[current_player];
	battle_monster = &monster_list[map_endless % monster_count];
	Battle_Start(battle_player, battle_monster);
	UI_DisplayBattleStat();
}

void UI_ShowSetting(Player *campaign_player, Player *endless_player)
{
	char save[32];
	
	UI_WRITE_ANNOUNCEMENT(10, 10, "CAMPAIGN");
	UI_WRITE_ANNOUNCEMENT(180, 10, "ENDLESS");

	sprintf(save, "Name: %s", campaign_player->name);
	UI_WRITE_STAT(10, 35, save);
	sprintf(save, "Name: %s", endless_player->name);
	UI_WRITE_STAT(170, 35, save);
	
	sprintf(save, "HP:%d/%d", campaign_player->hp, campaign_player->max_hp);
	UI_WRITE_STAT(10, 55, save);
	sprintf(save, "HP:%d/%d", endless_player->hp, endless_player->max_hp);
	UI_WRITE_STAT(170, 55, save);
	
	sprintf(save, "ATK:%d", campaign_player->attack);
	UI_WRITE_STAT(10, 75, save);
	sprintf(save, "ATK:%d", endless_player->attack);
	UI_WRITE_STAT(170, 75, save);
	
	sprintf(save, "DEF:%d", campaign_player->defense);
	UI_WRITE_STAT(10, 95, save);
	sprintf(save, "DEF:%d", endless_player->defense);
	UI_WRITE_STAT(170, 95, save);
	
	sprintf(save, "EXP:%d", campaign_player->exp);
	UI_WRITE_STAT(10, 115, save);
	sprintf(save, "EXP:%d", endless_player->exp);
	UI_WRITE_STAT(170, 115, save);
	
	sprintf(save, "LV:%d", campaign_player->level);
	UI_WRITE_STAT(10, 135, save);
	sprintf(save, "LV:%d", endless_player->level);
	UI_WRITE_STAT(170, 135, save);
	
	UI_WRITE_STAT(10, 155, "SELECT:RESET");
	UI_WRITE_STAT(170, 155, "BACK:EXIT");
}

static uint8_t setting_confirm = 0;
static uint32_t setting_confirm_wait = 0;

void UI_ResetStat(void)
{
	uint32_t now = HAL_GetTick();

	if (!HAL_Buttonselect())
	{
		return;
	}

	if (select_option == 1)
	{
		RESET_DISPLAY;
		if (battle_mode == 1)
		{
			map_endless = 0;
			endless_players[current_player].hp = endless_players[current_player].max_hp;
			UI_ShowEndless();
		}
		else
		{
			Campaign_Reset();
			campaign_players[current_player].hp = campaign_players[current_player].max_hp;
			UI_ShowCampaign();
		}
	}
	else
	{
		map_endless = 0;
		Campaign_Reset();
		ui_page = UI_PAGE_MENU;
		RESET_DISPLAY;
		LCD_DrawMenuGame();
		UI_WRITE_ANNOUNCEMENT(48, 70, "RESET COMPLETE");
		HAL_Delay(1000);
		RESET_DISPLAY;
		ui_page = UI_PAGE_MENU;
		LCD_DrawMenuGame();
	}
}

#define UI_WRITE_RESULT(x, y, text) 							ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);

static void UI_BattleEnd(uint8_t win)
{
	battle_player = 0;
	battle_monster = 0;
	
	RESET_DISPLAY;
	
	if(win)
	{
		UI_WRITE_RESULT(104, 75, "YOU WIN");
		HAL_Delay(2000);
		if(battle_mode == 1)
		{
			map_endless++;
			UI_ShowEndless();
			return;
		}
		
		if (current_state >= campaign_state_count - 1)
		{
			RESET_DISPLAY;
			UI_WRITE_RESULT(40, 75, "CAMPAIGN CLEAR!");
			HAL_Delay(2000);

			Campaign_Reset();
			RESET_DISPLAY;
			ui_page = UI_PAGE_MENU;
			LCD_DrawMenuGame();
			return;			
		}
		Campaign_NextStage();
		RESET_DISPLAY;
		UI_ShowCampaign();
	}
	else
	{
		select_option = 1;
		ui_page = UI_PAGE_GAME_OVER;
		gameover_select_locked = 1;
		RESET_DISPLAY;
		LCD_DrawEndPage();
		LCD_SelectOption(select_option);
	}
}

void UI_GameOverPage(void)
{
	if (gameover_select_locked)
	{
		if (!HAL_Buttonselect())
		{
			gameover_select_locked = 0;
		}
		return;
	}

	UI_MoveOption(&select_option);

	if (!HAL_Buttonselect())
	{
		return;
	}

	if (select_option == 1)
	{
		RESET_DISPLAY;
		if (battle_mode == 1)
		{
			map_endless = 0;
			endless_players[current_player].hp = endless_players[current_player].max_hp;
			UI_ShowEndless();
		}
		else
		{
			Campaign_Reset();
			campaign_players[current_player].hp = campaign_players[current_player].max_hp;
			UI_ShowCampaign();
		}
	}
	else
	{
		map_endless = 0;
		Campaign_Reset();
		ui_page = UI_PAGE_MENU;
		RESET_DISPLAY;
		LCD_DrawMenuGame();
	}
}

void UI_DisplayBattleStat(void)
{
	ST7789_Fill_Color(BLACK);
	LCD_DrawMonsterAppearance();
	LCD_DrawPlayerAppearance();
	LCD_DrawStats();
	LCD_Action();
	if(battle_player != 0 && battle_monster != 0)
	{
		UI_UpdateStats(battle_player, battle_monster);
	}	
}

static uint8_t select_locked = 0;
void UI_BattlePage(void)
{
	if(battle_player == 0 || battle_monster == 0)
	{
		return;
	}
	if(Battle_GetState() == BATTLE_VICTORY || Battle_GetState() == BATTLE_DEFEAT)
	{
		return;
	}
	LCD_SelectAction(select_action);
	UI_MoveAction(&select_action);
	if(!HAL_Buttonselect())
	{
		select_locked = 0;   
	}
	else if(!select_locked)
	{
		select_locked = 1;
		if (select_action == 1)
		{
			LCD_AttackEffect(1);
			Battle_Attack_Monster();
			UI_UpdateStats(battle_player, battle_monster);
	 
			if(Battle_GetState() == BATTLE_VICTORY)
			{
				Player_GainExp(battle_player, battle_monster->exp_reward);
				UI_BattleEnd(1);
				return;
			}
				if (active_skill == 1)
				{
					active_skill = 0;
					Battle_Freeze();
					UI_UpdateStats(battle_player, battle_monster);
					return;
				}
			HAL_Delay(1000);
			LCD_AttackEffect(0);
				if (active_skill == 2)
				{
					Battle_Counter();
					active_skill = 0;
				}
				else
				{
					Battle_Attack_Player();
				}
			UI_UpdateStats(battle_player, battle_monster);
			
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				UI_BattleEnd(0);
				return;
			}
		}
		else if (select_action == 2)
		{
			Battle_Defending();
			UI_UpdateStats(battle_player, battle_monster);
			
			HAL_Delay(400);
			LCD_AttackEffect(0);
			Battle_Attack_Player();
			UI_UpdateStats(battle_player, battle_monster);
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				UI_BattleEnd(0);
				return;
			}
		}
		else if (select_action == 3)
		{
			RESET_DISPLAY;
			ui_page = UI_PAGE_BATTLE_SKILLS;
			select_skill = 1;
			LCD_DrawBattleSkillMenu();
			LCD_SelectBattleSkill(select_skill);
		}
		else if (select_action == 4)
		{
			if(Battle_Count_HealPotion() == 0)
			{
				UI_WRITE_ANNOUNCEMENT(80, 75, "OUT OF POTION");
				HAL_Delay(550);
				RESET_DISPLAY;
				UI_DisplayBattleStat();
			}
			else
			{
				Battle_Heal();
				UI_UpdateStats(battle_player, battle_monster);
				
				HAL_Delay(400);
				LCD_AttackEffect(0);
				Battle_Attack_Player();        
				UI_UpdateStats(battle_player, battle_monster);
				if(Battle_GetState() == BATTLE_DEFEAT)
				{
					UI_BattleEnd(0);
					return;
				}			
			}
		}
	}
}

void UI_ConfirmMode(uint8_t mode)
{
	if(HAL_Buttonselect())
	{
		switch(mode)
		{
			case 1:
				RESET_DISPLAY;
				UI_ShowCampaign();
				break;
			case 2:
				RESET_DISPLAY;
				LCD_DrawEndlessMap();
				UI_ShowEndless();
				break;
			case 3:
				RESET_DISPLAY;
				ui_page = UI_PAGE_SKILLS;
				LCD_DrawSkillMenu();
				LCD_SelectSkill(select_skill);
				break;
			case 4:
				RESET_DISPLAY;
				ui_page = UI_PAGE_SETTING;
				UI_ShowSetting(&campaign_players[current_player], &endless_players[current_player]);
				break;
		}
	}
}

void UI_MoveMode(uint8_t *sl_mode)
{
	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectMode(*sl_mode);
		if (*sl_mode < 4)
		{
			(*sl_mode)++;
		}
		LCD_SelectMode(*sl_mode);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectMode(*sl_mode);
		if (*sl_mode > 1)
		{
			(*sl_mode)--;
		}
		LCD_SelectMode(*sl_mode);
	}
}

void UI_MoveSkillMenu(uint8_t *sl_skill)
{
	if (ui_page != UI_PAGE_SKILLS)
	{
		return;
	}

	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectSkill(*sl_skill);
		if (*sl_skill < 3)
		{
			(*sl_skill)++;
		}
		LCD_SelectSkill(*sl_skill);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectSkill(*sl_skill);
		if (*sl_skill > 1)
		{
			(*sl_skill)--;
		}
		LCD_SelectSkill(*sl_skill);
	}
}

void UI_MoveBattleSkill(uint8_t *sl_skill)
{
	if (ui_page != UI_PAGE_BATTLE_SKILLS)
	{
		return;
	}

	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectBattleSkill(*sl_skill);
		if (*sl_skill < 2)
		{
			(*sl_skill)++;
		}
		LCD_SelectBattleSkill(*sl_skill);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectBattleSkill(*sl_skill);
		if (*sl_skill > 1)
		{
			(*sl_skill)--;
		}
		LCD_SelectBattleSkill(*sl_skill);
	}
}

void UI_ConfirmSkillMenu(uint8_t skill)
{
	if(HAL_Buttonselect())
	{
		switch(skill)
		{
			case 1:
				if ((select_mode == 1 && Skill_Unlocked(&campaign_players[current_player], SKILL_FREEZE)) ||
					(select_mode == 2 && Skill_Unlocked(&endless_players[current_player], SKILL_FREEZE)))
				{
					active_skill = 1;
					RESET_DISPLAY;
					UI_WRITE_ANNOUNCEMENT(90, 70, "FREEZE SET");
					HAL_Delay(700);
					RESET_DISPLAY;
					LCD_DrawSkillMenu();
					LCD_SelectSkill(select_skill);
				}
				else
				{
					UI_WRITE_ANNOUNCEMENT(65, 70, "NEED LEVEL 3");
					HAL_Delay(700);
					RESET_DISPLAY;
					LCD_DrawSkillMenu();
					LCD_SelectSkill(select_skill);
				}
				break;
			case 2:
				if ((select_mode == 1 && Skill_Unlocked(&campaign_players[current_player], SKILL_COUNTER)) ||
					(select_mode == 2 && Skill_Unlocked(&endless_players[current_player], SKILL_COUNTER)))
				{
					active_skill = 2;
					RESET_DISPLAY;
					UI_WRITE_ANNOUNCEMENT(78, 70, "COUNTER SET");
					HAL_Delay(700);
					RESET_DISPLAY;
					LCD_DrawSkillMenu();
					LCD_SelectSkill(select_skill);
				}
				else
				{
					UI_WRITE_ANNOUNCEMENT(65, 70, "NEED LEVEL 6");
					HAL_Delay(700);
					RESET_DISPLAY;
					LCD_DrawSkillMenu();
					LCD_SelectSkill(select_skill);
				}
				break;
			case 3:
				RESET_DISPLAY;
				ui_page = UI_PAGE_MENU;
				LCD_DrawMenuGame();
				break;
		}
	}
}

void UI_ConfirmBattleSkill(uint8_t skill)
{
	if(HAL_Buttonselect())
	{
		switch(skill)
		{
			case 1:
				if (Skill_Unlocked(battle_player, SKILL_FREEZE))
				{
					active_skill = 1;
					RESET_DISPLAY;
					UI_WRITE_ANNOUNCEMENT(90, 70, "FREEZE READY");
					HAL_Delay(700);
					RESET_DISPLAY;
					ui_page = UI_PAGE_BATTLE;
					select_locked = 1;
					UI_DisplayBattleStat();
					LCD_SelectAction(select_action);
				}
				else
				{
					UI_WRITE_ANNOUNCEMENT(65, 70, "NEED LEVEL 3");
					HAL_Delay(700);
					RESET_DISPLAY;
					ui_page = UI_PAGE_BATTLE;
					select_locked = 1;
					UI_DisplayBattleStat();
					LCD_SelectAction(select_action);
				}
				break;
			case 2:
				if (Skill_Unlocked(battle_player, SKILL_COUNTER))
				{
					active_skill = 2;
					RESET_DISPLAY;
					UI_WRITE_ANNOUNCEMENT(78, 70, "COUNTER READY");
					HAL_Delay(700);
					RESET_DISPLAY;
					ui_page = UI_PAGE_BATTLE;
					select_locked = 1;
					UI_DisplayBattleStat();
					LCD_SelectAction(select_action);
				}
				else
				{
					UI_WRITE_ANNOUNCEMENT(65, 70, "NEED LEVEL 6");
					HAL_Delay(700);
					RESET_DISPLAY;
					ui_page = UI_PAGE_BATTLE;
					select_locked = 1;
					UI_DisplayBattleStat();
					LCD_SelectAction(select_action);
				}
				break;
		}
	}
}

uint8_t UI_BackSkillMenu(void)
{
	if(HAL_Buttonback())
	{
		RESET_DISPLAY;
		ui_page = UI_PAGE_MENU;
		LCD_DrawMenuGame();
		return 1;
	}
	return 0;
}

uint8_t UI_BackBattleSkillMenu(void)
{
	if(HAL_Buttonback())
	{
		RESET_DISPLAY;
		ui_page = UI_PAGE_BATTLE;
		UI_DisplayBattleStat();
		LCD_SelectAction(select_action);
		return 1;
	}
	return 0;
}


void UI_MoveOption(uint8_t *sl_option)
{
	if(HAL_Buttondown())
	{
		HAL_Delay(100);
		LCD_DeselectOption(*sl_option);
		if (*sl_option < 2)
		{
			(*sl_option)++;
		}
		LCD_SelectOption(*sl_option);
	}
	if(HAL_Buttonup())
	{
		HAL_Delay(100);
		LCD_DeselectOption(*sl_option);
		if (*sl_option > 1)
		{
			(*sl_option)--;
		}
		LCD_SelectOption(*sl_option);
	}
}


void UI_BackMenuGame(void)
{
	if(HAL_Buttonback())
	{
		RESET_DISPLAY;
		ui_page = UI_PAGE_MENU;
		LCD_DrawMenuGame();
	}
}
