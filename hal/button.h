#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    bool up;
    bool down;
    bool select;
    bool back;
} ButtonState;

void HAL_ScanButtons(void);
bool HAL_Button_Up(void);
bool HAL_Button_Down(void);
bool HAL_Button_Select(void);
bool HAL_Button_Back(void);

#endif // _BUTTON_H_
