#include "../include/UI_handler.h"

namespace UI {
    Handler::Handler() {
        InitWindow(window_width, window_height, "Data Structure Visualization");

        SetTargetFPS(60);
        
        canvas[0] = new Menu_Canvas();
        
        current_state = UI_State::MENU;
        for (int i = 0; i < static_cast<int>(UI_State::COUNT); i++) {
            if (canvas[i]) {
                canvas[i]->set_current_state(&current_state);
            }
        }
    }

    void Handler::run() {
        while (!WindowShouldClose()) {
            canvas[static_cast<int>(current_state)]->run();
        }

        CloseWindow();
    }
}
