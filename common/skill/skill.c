#include "skill.h"

uint8_t Skill_Unlocked(Player *player, Skill skill)
{
	if (player == 0)
	{
		return 0;
	}

	if (skill == SKILL_FREEZE)
	{
		return player->level >= SKILL_FREEZE_LEVEL;
	}
    
	if (skill == SKILL_COUNTER)
	{
		return player->level >= SKILL_COUNTER_LEVEL;
	}
	return 0;
}