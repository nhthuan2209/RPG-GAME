#include "monster.h"

Monster monster_list[] =
{
    {"Slime", 45, 45, 8, 2, 15},
    {"Goblin", 70, 70, 13, 4, 25},
    {"Wolf", 95, 95, 18, 7, 40},
    {"Dragon", 180, 180, 30, 12, 80},
    {"Lord", 280, 280, 42, 17, 150},
    {"Bat", 55, 55, 11, 3, 20},
    {"Orc", 125, 125, 23, 9, 55},
    {"Troll", 190, 190, 30, 13, 85},
    {"Demon", 260, 260, 38, 17, 120},
    {"Titan", 360, 360, 50, 22, 200},
    {"Spider", 85, 85, 16, 5, 35},
    {"Golem", 320, 320, 36, 25, 140},
    {"Wraith", 230, 230, 45, 14, 130},
    {"Kraken", 430, 430, 58, 24, 260},
    {"Dragon King", 550, 550, 68, 30, 350}
};

uint8_t monster_count =
    sizeof(monster_list) / sizeof(monster_list[0]);