#ifndef MONSTER_H
#define MONSTER_H

#include "stdint.h"

typedef struct
{
    char* name;

    int32_t max_hp;
    int32_t hp;

    int16_t attack;
    int16_t defense;

    int16_t exp_reward;

} Monster;

extern Monster monster_list[];
extern uint8_t monster_count;

#endif