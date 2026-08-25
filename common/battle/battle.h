#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "stdint.h"
#include "player.h"
#include "monster.h"

#define BATTLE_MAX_POTION_HEAL 		5

typedef enum{
	BATTLE_PLAYER_TURN,
	BATTLE_MONSTER_TURN,
	BATTLE_VICTORY,
	BATTLE_DEFEAT
}BattleState;

typedef struct
{
		Player *player;
    Monster *monster;

    BattleState state;
		uint16_t player_defending;
		uint16_t player_usedheal;

}Battle;

void Battle_Start(Player *player, Monster *monster);
void Battle_Defending(void);
void Battle_Attack_Monster();
void Battle_Attack_Player();
void Battle_Update(void);
void Battle_Heal(void);
uint8_t Battle_Count_HealPotion();
BattleState Battle_GetState(void);




#endif // _BATTLE_H_