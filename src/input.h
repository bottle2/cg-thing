#ifndef __T1__INPUT__H__
#define __T1__INPUT__H__

// Human readable constants rather than magic numbers.

enum
{
    INPUT_KEY_ESC   = 0x1B,
    INPUT_KEY_SHIFT = 0xD4,
};

#define INPUT_IRRELEVANT -2

enum input_state
{
    INPUT_PRESSED  = 0,
    INPUT_RELEASED = 1,
};

enum input_mouse
{
    INPUT_MOUSE_LEFT   = 0,
    INPUT_MOUSE_MIDDLE = 1,
    INPUT_MOUSE_RIGHT  = 2,
};

enum input_scroll
{
    INPUT_SCROLL_YES = 0,
};

enum input_direction
{
    INPUT_UPWARDS   =  1,
    INPUT_DOWNWARDS = -1,
};

#endif // __T1__INPUT__H__
