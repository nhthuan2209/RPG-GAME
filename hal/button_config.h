#ifndef _BUTTON_CONFIG_H_
#define _BUTTON_CONFIG_H_

#include <stm32f1xx_hal_gpio.h>

#define BUTTON_UP_PORT									GPIOA
#define BUTTON_UP_PIN										GPIO_PIN_11
#define BUTTON_DOWN_PORT								GPIOA
#define BUTTON_DOWN_PIN									GPIO_PIN_10
#define BUTTON_SELECT_PORT							GPIOA
#define BUTTON_SELECT_PIN								GPIO_PIN_15
#define BUTTON_BACK_PORT								GPIOB
#define BUTTON_BACK_PIN									GPIO_PIN_3 

#endif // _BUTTON_CONFIG_H_
