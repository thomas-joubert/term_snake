#include "keyboard.h"
#include <string.h>

struct keyboard set_layout(const char* layout)
{
    struct keyboard keyboard;

    // By default the layout is us, thus we can ignore this case
    if(!strcmp("fr", layout))
    {
        keyboard.UP = 'z';
        keyboard.DOWN = 's';
        keyboard.LEFT = 'q';
        keyboard.RIGHT = 'd';
    }
    if(!strcmp("us", layout))
    {
        keyboard.UP = 'w';
        keyboard.DOWN = 's';
        keyboard.LEFT = 'a';
        keyboard.RIGHT = 'd';
    }

    keyboard.ESC = 27;

    return keyboard;
}
