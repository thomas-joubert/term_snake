#ifndef DEF_KEYBOARD
#define DEF_KEYBOARD

// We have to pass by a structure since we cannot create a Union with
// enums
struct keyboard
{
    char UP;     // w
    char DOWN;   // s
    char RIGHT;  // d
    char LEFT;   // a
    char ESC;
};

struct keyboard set_layout(const char* layout);

#endif // end of keyboard.h
