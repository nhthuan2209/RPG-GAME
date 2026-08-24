#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "stdint.h"

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
	
}Character;

extern Character character_list[];
extern uint8_t character_count;

void Character_Up_Level(Character *player);
void Character_Gain_Exp(Character *player, int16_t exp);

#endif // _CHARACTER_H_