#include "character.h"

Character character_list[] =
{
	{"Warrior", 100, 100, 20, 5, 1, 0},
	{"Wizard", 60, 60, 40, 2, 1, 0}
};

uint8_t character_count = sizeof(character_list) / sizeof(character_list[0]);

static void Character_Up_Level(Character *player)
{
	int16_t new_maxhp_player = player->max_hp + player->max_hp / 3;
	int16_t new_attack_player = player->attack + player->attack / 3;
	int16_t new_defense_player = player->defense + player->defense / 3;
	
	player->max_hp = new_maxhp_player;
	player->attack = new_attack_player;
	player->defense = new_defense_player;
	
	player->hp = player->max_hp;
	player->level++;
}

void Character_Gain_Exp(Character *player, int16_t exp)
{
	player->exp = player->exp + exp;
	while(player->exp >= EXP_UP_LEVEL)
	{
		player->exp = player->exp - EXP_UP_LEVEL;
		Character_Up_Level(player);
	}
}
