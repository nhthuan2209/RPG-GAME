#include "user.h"
#include "main.h"

Card list_user[] = {
	{{0x1A,0x01,0x02,0x02}, "User 1",0}, {{0x07,0xF6,0x29,0x07}, "User 2",1}
};

int8_t Find_Uid(uint8_t *uid)
{	
	for(int i = 0; i < 2; i++)
	{
		if(list_user[i].uid[0] == uid[0] && list_user[i].uid[1] == uid[1] && list_user[i].uid[2] == uid[2] && list_user[i].uid[3] == uid[3])
		{
			return i;
		}
	}
	return -1;
}