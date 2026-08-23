#include "add_monster.h"

Monster monster_list[] =
{
    {"Slime", 50, 50, 10, 2, 20},
    {"Goblin", 80, 80, 15, 5, 35},
    {"Wolf", 100, 100, 20, 8, 50},
    {"Dragon", 300, 300, 40, 15, 200},
		{"Lord", 500, 500, 60, 25, 400}
};

uint8_t monster_count =
    sizeof(monster_list) / sizeof(monster_list[0]);