#ifndef _UI_H_
#define _UI_H_

#include "stdint.h"
#include "character.h"

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

void Menu_Game(void);
void Waiting_Display(void);
void Check_Card(void);
void Ui_PollWaitingScreen(void);
void Ui_Select_Mode(uint8_t select_mode);
void Ui_Deselect_Mode(uint8_t deselect_mode);
void Ui_Display_Battle_Stat(void);
void Ui_Endless_Map_Page(void);
void Ui_Confirm_Mode(uint8_t mode);
void Ui_Move_Mode(uint8_t *sl_mode);
static void Ui_Battle_End(uint8_t win);
void Ui_Battle_Page(void);
static void Ui_Show_Campaign();
static void Ui_Show_Endless();
void Ui_Show_Setting(Character *player);
void Ui_Reset_Stat();
void Ui_Back_MenuGame();

#endif // _UI_H_