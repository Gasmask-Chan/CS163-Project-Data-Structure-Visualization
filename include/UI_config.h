#ifndef UI_CONFIG
#define UI_CONFIG

#include "raylib.h"
#include "raymath.h"

#include <iostream> //For debugging

namespace UI {
    enum OPERATION {
        INSERT,
        ERASE,
        FIND,
        NONE
    };
    
    const int window_height = 720;
    const int window_width = 1280;
    extern Font main_font;
    extern Font code_font;

    void load_font();
    void unload_font();

}

#endif