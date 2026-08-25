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
bool Button_Up(void);
bool Button_Down(void);
bool Button_Select(void);
bool Button_Back(void);

#endif // _BUTTON_H_
