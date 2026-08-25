#ifndef _CAMPAIGN_H_
#define _CAMPAIGN_H_

#include "stdint.h"

typedef struct
{
	char name[20];
	char map[20];
	
	uint8_t monster_id;
	
}CampaignStage;

extern CampaignStage campaign[];

extern uint8_t current_state;
extern uint8_t campaign_state_count;

void Campaign_NextStage(void);
void Campaign_Reset(void);
#endif // _CAMPAIGN_H_