#ifndef _USER_H_
#define _USER_H_

#include "stdint.h"

typedef struct 
{
	uint8_t uid[4];
	char character[30];
	uint32_t health;
	uint32_t damage;
	uint32_t defense;
	uint32_t range;
}Card;

#endif // _USER_H_