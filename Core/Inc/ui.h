#ifndef _UI_H_
#define _UI_H_

#include "stdint.h"


extern uint8_t uid[4];
extern uint8_t card;
extern uint8_t game_page;
extern int8_t  current_player;
extern uint8_t select_mode;
uint8_t Read_Card(void);

void Menu_Game(void);
void Waiting_Display(void);
void Check_Card(void);
void Ui_Select_Mode(uint8_t select_mode);
void Ui_Deselect_Mode(uint8_t deselect_mode);
void Ui_Campaign_Page();
void Ui_Endless_Page();
void Ui_Setting_Page();
void Ui_Confirm_Mode(uint8_t mode);
void Ui_Move_Mode(uint8_t *sl_mode);
void Ui_Battle_Page(void);
#endif // _UI_H_