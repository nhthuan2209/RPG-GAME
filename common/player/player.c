#include "player.h"

Player player_list[] =
{
	{"Warrior", 90, 90, 18, 5, 1, 0, 0, {0, 0, 0, 0}},
	{"Wizard", 60, 60, 32, 2, 1, 0, 0, {0, 0, 0, 0}}
};

Player player_list_default[] =
{
	{"Warrior", 90, 90, 18, 5, 1, 0, 0, {0, 0, 0, 0}},
	{"Wizard", 60, 60, 32, 2, 1, 0, 0, {0, 0, 0, 0}}
};

uint8_t player_count = sizeof(player_list_default) / sizeof(player_list_default[0]);

static void Player_UpLevel(Player *player)
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

void Player_GainExp(Player *player, int16_t exp)
{
	player->exp = player->exp + exp;
	while(player->exp >= EXP_UP_LEVEL)
	{
		player->exp = player->exp - EXP_UP_LEVEL;
		Player_UpLevel(player);
	}
}

void Player_Reset(Player *player)
{
    uint8_t i;

    for (i = 0; i < player_count; i++)
    {
        if (player == &player_list_default[i])
        {
            player_list[i] = player_list_default[i];
            return;
        }
    }
}
