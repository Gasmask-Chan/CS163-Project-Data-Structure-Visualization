#include "../include/UI_handler.h"

namespace UI {
    Handler::Handler() {
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        InitWindow(window_width, window_height, "Data Structure Visualization");

        SetTargetFPS(60);
        
        //Font
        load_font();
        
        //UI state
        current_state = UI_State::MENU;

        //Camera
        camera = {0};
        camera.zoom = 1.0f;

        //Canvas
        canvas[UI_State::MENU] = new Menu_Canvas();
        canvas[UI_State::AVL] = new AVL_Canvas();
        canvas[UI_State::HEAP] = new Heap_Canvas();
        canvas[UI_State::LINKED_LIST] = new LinkedList_Canvas();
        canvas[UI_State::TRIE] = new Trie_Canvas();

        for (int i = 0; i < static_cast<int>(UI_State::COUNT); i++) {
            if (canvas[i]) {
                canvas[i]->set_current_state(&current_state);
                canvas[i]->set_camera(&camera);
                canvas[i]->setup();
            }
        }
    }

    Handler::~Handler() {
        for (int i = 0; i < static_cast<int>(UI_State::COUNT); i++) {
            if (canvas[i]) {
                delete canvas[i];
                canvas[i] = nullptr;
            }
        }
    }

    void Handler::update_camera() {
        //Mouse dragging to change camera target
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            Vector2 delta = Vector2Scale(GetMouseDelta(), -1.0f / camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        //Camera zooming using mouse scroll button
        float wheel = GetMouseWheelMove();
        if (wheel) {
            camera.target = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();

            camera.zoom *= 1.0f + 0.125f * wheel;
            if (camera.zoom < 0.1f) camera.zoom = 0.1f;
            if (camera.zoom > 10.0f) camera.zoom = 10.0f;
        }

        //Reset camera by pressing key `R`
        if (IsKeyPressed(KEY_R)) {
            camera.zoom = 1.0f;
            camera.target = camera.offset = {0, 0};
        }
    }

    void Handler::run() {
        while (!WindowShouldClose()) {
            update_camera();
            canvas[static_cast<int>(current_state)]->run();
        }

        unload_font();
        CloseWindow();
    }
}
