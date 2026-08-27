#ifndef _BATTLE_H_
#define _BATTLE_H_

#include "stdint.h"
#include "player.h"
#include "monster.h"
#include "potion.h"

#define BATTLE_MAX_POTION_HEAL 		5
#define BATTLE_CRIT_CHANCE_PERCENT 	20
#define BATTLE_CRIT_DAMAGE		 	2

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
		uint8_t potion_count[3];
		uint8_t protect_active;
		uint8_t fire_active;

}Battle;

void Battle_Start(Player *player, Monster *monster);
void Battle_Defending(void);
void Battle_Attack_Monster();
void Battle_Attack_Player();
void Battle_Freeze(void);
void Battle_Counter(void);
void Battle_Update(void);
void Battle_Heal(void);
uint8_t Battle_Count_HealPotion();
uint8_t Battle_UsePotion(PotionType potion);
uint8_t Battle_CountPotion(PotionType potion);
BattleState Battle_GetState(void);




#endif // _BATTLE_H_