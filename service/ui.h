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
} UiPage;

extern uint8_t uid[4];
extern uint8_t card;
extern UiPage  ui_page;
extern int8_t  current_player;
extern uint8_t select_mode;

void UI_HandleWaitingForRfidCard(void);
void UI_Display_Battle_Stat(void);
void UI_Confirm_Mode(uint8_t mode);
void UI_Move_Mode(uint8_t *sl_mode);
void UI_Battle_Page(void);
void UI_Show_Setting(Player *player);
void UI_Reset_Stat(void);
void UI_Back_MenuGame(void);

#endif // _UI_H_