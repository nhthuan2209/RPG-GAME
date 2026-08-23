#include <string.h>
#include "button.h"
#include "button_config.h"

#define BUTTON_COUNT                                        4
#define BUTTON_DEBOUNCE_TICKS                               3
typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
	uint32_t active_time;
} ButtonInfo;

static ButtonInfo button_info[BUTTON_COUNT] = {
	{.port=BUTTON_UP_PORT, 		 .pin=BUTTON_UP_PIN, 		.active_time = 0},
	{.port=BUTTON_DOWN_PORT, 	 .pin=BUTTON_DOWN_PIN, 		.active_time = 0},
	{.port=BUTTON_SELECT_PORT,   .pin=BUTTON_SELECT_PIN, 	.active_time = 0},
	{.port=BUTTON_BACK_PORT, 	 .pin=BUTTON_BACK_PIN, 		.active_time = 0},
};

ButtonState HAL_ReadButtons(void)
{
	ButtonState button_state;
	memset(&button_state, 0, sizeof(button_state));
	
	for (size_t i = 0; i < BUTTON_COUNT; i++) {

		if (HAL_GPIO_ReadPin(button_info[i].port, button_info[i].pin) == GPIO_PIN_RESET) {
			button_info[i].active_time++;
			if (button_info[i].active_time >= BUTTON_DEBOUNCE_TICKS) {
				switch(i)
				{
					case 0:
						button_state.up 		= true;
						break;
					case 1:
						button_state.down 	= true;
						break;
					case 2:
						button_state.select = true;
						break;
					case 3:
						button_state.back 	= true;
						break;
				}
			}
		}
		else {
			button_info[i].active_time = 0;
		}
	}
	return button_state;
}

bool Button_Up(void)
{
	return HAL_ReadButtons().up;
}
 
bool Button_Down(void)
{
	return HAL_ReadButtons().down;
}
 
bool Button_Select(void)
{
	return HAL_ReadButtons().select;
}
 
bool Button_Back(void)
{
	return HAL_ReadButtons().back;
}