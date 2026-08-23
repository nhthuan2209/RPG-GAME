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
uint8_t game_page = 0;
int8_t current_player = -1;
uint8_t select_mode = 1;
uint8_t select_action = 1;

static Character *battle_player = 0;
static Monster   *battle_monster = 0;

typedef enum {
	UI_PAGE_1 = 0,
	UI_PAGE_2,
	UI_PAGE_3,
	UI_PAGE_MAX,
} UI_PAGE;

typedef void (*UI_PAGE_DISPLAY)(void);

void UI_DisplayPage1(void){
	// ...
}

void UI_DisplayPage2(void){
	// ...
}

void UI_DisplayPage3(void){
	// ...
}

UI_PAGE_DISPLAY ui_display_functions[UI_PAGE_MAX] = {
	UI_DisplayPage1,
	UI_DisplayPage2,
	UI_DisplayPage3,
};

static UI_PAGE current_page = UI_PAGE_1;

void UI_Main(void)
{
	UI_PAGE_DISPLAY display_function = ui_display_functions[current_page];
	display_function();
}

void Ui_PollWaitingScreen(void)
{
	if(game_page == 0)
	{
		uint8_t uid[RFID_UID_LEN];
 
		if (Rfid_TryReadUid(uid)) {
			current_player = User_FindUid(uid);
			if (current_player >= 0) {
				game_page = 1;
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
			Battle_Attack_Monster();
			Ui_Update_Stats(battle_player, battle_monster);
	 
			if(Battle_GetState() == BATTLE_VICTORY)
			{
				ST7789_WriteString(90, 200, "YOU WIN!", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
				return;
			}
 
			HAL_Delay(400);
			Battle_Attack_Player();
			Ui_Update_Stats(battle_player, battle_monster);
	 
			if(Battle_GetState() == BATTLE_DEFEAT)
			{
				ST7789_WriteString(90, 200, "YOU LOSE!", FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR);
			}
		}
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

#define UI_WRITE_MENU(x, y, text)					ST7789_WriteString(x, y, text, FONT_MENU_GAME, LETTER_MENU_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)
#define UI_WRITE_OPTION(x, y, text)				ST7789_WriteString(x, y, text, FONT_OPTION_FUNCTION, LETTER_MODE_GAME_COLOR, BACKGROUND_MENU_GAME_COLOR)
#define UI_WRITE_LINE(x1, y1, x2, y2)			ST7789_DrawLine(x1, y1, x2, y2, WHITE)
#define UI_WRITE_CIRCLE(x, y, r)					ST7789_DrawCircle(x, y, r, WHITE)

void Ui_Update_Stats(Character *player, Monster *monster)
{
	Ui_Player_Stats(player);
	Ui_Monster_Stats(monster);
}

void Ui_Campaign_Page()
{
	UI_WRITE_MENU(10, 10, "CAMPAIGN");
	UI_WRITE_LINE(40, 70, 40, 120);
	UI_WRITE_OPTION(10, 120, "Forest");
	UI_WRITE_LINE(50, 60, 90, 60);
	UI_WRITE_CIRCLE(40, 60, 10);
	
	UI_WRITE_LINE(110, 60, 150, 60);
	UI_WRITE_CIRCLE(100, 60, 10);
	
	UI_WRITE_LINE(160, 70, 160, 110);
	UI_WRITE_CIRCLE(160, 60, 10);
	
	UI_WRITE_LINE(170, 120, 210, 120);
	UI_WRITE_CIRCLE(160, 120, 10);
	
	UI_WRITE_LINE(220, 70, 220, 110);
	UI_WRITE_CIRCLE(220, 120, 10);
	
	UI_WRITE_OPTION(200, 65, "Boss");
	
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

