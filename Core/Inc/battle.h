#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "stdint.h"
#include "character.h"
#include "add_monster.h"

typedef enum{
	BATTLE_PLAYER_TURN,
	BATTLE_MONSTER_TURN,
	BATTLE_VICTORY,
	BATTLE_DEFEAT
}BattleState;

typedef struct
{
    Character *player;
    Monster *monster;

    BattleState state;

}Battle;

void Battle_Start(Character *player, Monster *monster);
void Battle_Attack_Monster();
void Battle_Attack_Player();
void Battle_Update(void);
BattleState Battle_GetState(void);




#endif // _BATTLE_H_