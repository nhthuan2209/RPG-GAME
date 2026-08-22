#ifndef _USER_H_
#define _USER_H_

#include "stdint.h"

typedef struct 
{
	uint8_t uid[4];
	char name[20];
	
	uint8_t character_id;
}Card;

int8_t Find_Uid(uint8_t *uid);
#endif // _USER_H_