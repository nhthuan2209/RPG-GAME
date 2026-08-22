#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stm32f1xx_hal_gpio.h>

typedef struct {
    bool up;
    bool down;
    bool select;
    bool back;
} ButtonState;

ButtonState HAL_ReadButtons(void);

#endif // _BUTTON_H_
