#include "battle.h"
#include <stdlib.h>

static Battle current_battle;

void Battle_Start(Player *player, Monster *monster)
{
	current_battle.player = player;
	current_battle.monster = monster;
	current_battle.state = BATTLE_PLAYER_TURN;
	current_battle.player_defending = 0;
	current_battle.player_usedheal = 0;
	current_battle.potion_count[POTION_HEAL - 1] = POTION_HEAL_COUNT;
	current_battle.potion_count[POTION_PROTECT - 1] = POTION_PROTECT_COUNT;
	current_battle.potion_count[POTION_FIRE - 1] = POTION_FIRE_COUNT;
	current_battle.potion_count[POTION_POWER - 1] = POTION_POWER_COUNT;
	current_battle.protect_active = 0;
	current_battle.fire_active = 0;
	current_battle.power_active = 0;
	monster->hp = monster->max_hp;
}

void Battle_Defending()
{
	current_battle.player_defending = 1;
	current_battle.state = BATTLE_MONSTER_TURN;
}

uint8_t Battle_CountPotion(PotionType potion)
{
	return current_battle.potion_count[potion - 1];
}

uint8_t Battle_UsePotion(PotionType potion)
{
	if (potion < POTION_HEAL || potion >= POTION_COUNT ||
		current_battle.potion_count[potion - 1] == 0)
	{
		return 0;
	}

	current_battle.potion_count[potion - 1]--;
	if (potion == POTION_HEAL)
	{
		current_battle.player->hp = current_battle.player->max_hp;
	}
	else if (potion == POTION_PROTECT)
	{
		current_battle.protect_active = 1;
	}
	else if (potion == POTION_FIRE)
	{
		current_battle.fire_active = 1;
	}
	else if (potion == POTION_POWER)
	{
		current_battle.power_active = 1;
	}
	current_battle.state = BATTLE_MONSTER_TURN;
	return 1;
}

void Battle_Attack_Monster()
{
	int16_t dmg = current_battle.player->attack - current_battle.monster->defense;
	if (dmg < 1)
	{
		dmg = 1;
	}
	if ((rand() % 100) < BATTLE_CRIT_CHANCE_PERCENT)
	{
		dmg = dmg * BATTLE_CRIT_DAMAGE;
	}
	if (current_battle.power_active)
	{
		dmg = dmg * 2;
		current_battle.power_active = 0;
	}
	current_battle.monster->hp = current_battle.monster->hp - dmg;
	if(current_battle.monster->hp <= 0)
	{
		current_battle.monster->hp = 0;
		current_battle.state = BATTLE_VICTORY;
	}
	else
	{
		current_battle.state = BATTLE_MONSTER_TURN;
	}
}
void Battle_Attack_Player()
{
	int16_t dmg = current_battle.monster->attack - current_battle.player->defense;

	if (current_battle.protect_active)
	{
		dmg = 0;
		current_battle.protect_active = 0;
	}
	else if(current_battle.player_defending)
	{
		dmg = dmg / 2;
		current_battle.player_defending = 0;
	}
	current_battle.player->hp = current_battle.player->hp - dmg;
	if(current_battle.player->hp <= 0)
	{
		current_battle.player->hp = 0;
		current_battle.state = BATTLE_DEFEAT;
	}
	else
	{
		if (current_battle.fire_active)
		{
			current_battle.monster->hp -= POTION_FIRE_DAMAGE;
			current_battle.fire_active = 0;
			if (current_battle.monster->hp <= 0)
			{
				current_battle.monster->hp = 0;
				current_battle.state = BATTLE_VICTORY;
				return;
			}
		}
		current_battle.state = BATTLE_PLAYER_TURN;
	}
	if(current_battle.power_active)
	{
		current_battle.player->attack = current_battle.player->attack * 2;
		current_battle.power_active = 0;
		if(current_battle.player->attack < 1)
		{
			current_battle.player->attack = 1;
		}
	}
}

void Battle_Freeze(void)
{
	if (current_battle.state == BATTLE_MONSTER_TURN)
	{
		current_battle.state = BATTLE_PLAYER_TURN;
	}
}

void Battle_Counter(void)
{
	int16_t dmg = current_battle.monster->attack - current_battle.player->defense;

	if (current_battle.player_defending)
	{
		dmg = dmg / 2;
		current_battle.player_defending = 0;
	}

	current_battle.player->hp = current_battle.player->hp - dmg;
	if (current_battle.player->hp <= 0)
	{
		current_battle.player->hp = 0;
		current_battle.state = BATTLE_DEFEAT;
		return;
	}

	current_battle.monster->hp = current_battle.monster->hp - dmg;
	if (current_battle.monster->hp <= 0)
	{
		current_battle.monster->hp = 0;
		current_battle.state = BATTLE_VICTORY;
	}
	else
	{
		current_battle.state = BATTLE_PLAYER_TURN;
	}
}

void Battle_Update(void)
{
    if(current_battle.state == BATTLE_MONSTER_TURN)
    {
        Battle_Attack_Player();
    }
}

BattleState Battle_GetState(void)
{
    return current_battle.state;
}