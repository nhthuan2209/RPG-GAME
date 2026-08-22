#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "stdint.h"

typedef struct
{
	char name[20];
	
	int8_t max_hp;
	int8_t hp;
	
	int8_t attack;
	int8_t defense;
	
	int8_t level;
	int16_t exp;
	
}Character;

extern Character character_list[];
extern uint8_t character_count;

#endif // _CHARACTER_H_