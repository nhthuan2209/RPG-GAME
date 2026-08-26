#ifndef _UI_H_
#define _UI_H_

#include "stdint.h"
#include "player.h"

typedef enum {
	UI_PAGE_WAITING = 0,
	UI_PAGE_MENU,
	UI_PAGE_BATTLE,
	UI_PAGE_ENDLESS,
	UI_PAGE_SETTING,
	UI_PAGE_GAME_OVER,
	UI_PAGE_SKILLS,
} UiPage;

extern uint8_t uid[4];
extern uint8_t card;
extern UiPage  ui_page;
extern int8_t  current_player;
extern uint8_t select_mode;
extern uint8_t select_skill;
extern uint8_t active_skill;

void UI_HandleWaitingForRfidCard(void);
void UI_DisplayBattleStat(void);
void UI_ConfirmMode(uint8_t mode);
void UI_MoveMode(uint8_t *sl_mode);
void UI_MoveOption(uint8_t *sl_option);
void UI_BattlePage(void);
void UI_GameOverPage(void);
void UI_MoveSkill(uint8_t *sl_skill);
void UI_ConfirmSkill(uint8_t skill);
void UI_ShowSetting(Player *campaign_player, Player *endless_player);
void UI_ResetStat(void);
void UI_BackMenuGame(void);

#endif // _UI_H_