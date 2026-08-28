#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <stdint.h>
#include "potion.h"

#define EXP_UP_LEVEL					40

typedef struct
{
	char name[20];
	
	int32_t max_hp;
	int32_t hp;
	
	int32_t attack;
	int32_t defense;
	
	int32_t level;
	int32_t exp;

	int32_t gold;
	uint8_t potion_count[POTION_COUNT - 1];
	
}Player;

extern Player player_list[];
extern uint8_t player_count;

void Player_GainGold(Player *player, int16_t gold);
void Player_GainExp(Player *player, int16_t exp);
void Player_Reset(Player *player);
#endif // _PLAYER_H_