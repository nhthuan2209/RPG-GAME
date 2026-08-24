#include "battle.h"

static Battle current_battle;

void Battle_Start(Character *player, Monster *monster)
{
	current_battle.player = player;
	current_battle.monster = monster;
	current_battle.state = BATTLE_PLAYER_TURN;
	current_battle.player_defending = 0;
}

void Battle_Defending()
{
	current_battle.player_defending = 1;
	current_battle.state = BATTLE_MONSTER_TURN;
}

void Battle_Attack_Monster()
{
	int16_t dmg = current_battle.player->attack - current_battle.monster->defense;
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
	if(current_battle.player_defending)
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