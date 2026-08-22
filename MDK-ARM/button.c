#include "button.h"

#include <stdint.h>

#include "definition.h"

uint8_t Button_Up()
{
	return HAL_GPIO_ReadPin(BUTTON_UP_PORT, BUTTON_UP_PIN) == GPIO_PIN_RESET;
}
uint8_t Button_Down()
{
	return HAL_GPIO_ReadPin(BUTTON_DOWN_PORT, BUTTON_DOWN_PIN) == GPIO_PIN_RESET;
}
uint8_t Button_Select()
{
	return HAL_GPIO_ReadPin(BUTTON_SELECT_PORT, BUTTON_SELECT_PIN) == GPIO_PIN_RESET;
}
uint8_t Button_Back()
{
	return HAL_GPIO_ReadPin(BUTTON_BACK_PORT, BUTTON_BACK_PIN) == GPIO_PIN_RESET;
}