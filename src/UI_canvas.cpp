#include "../include/UI_canvas.h"

namespace UI {
    void Canvas::set_current_state(UI_State *cur_state) {
        current_state = cur_state;
    }

    void Canvas::run() {}

    void Menu_Canvas::run() {
        Rectangle avl_tree_button = {84, 288, 293, 74};
        Rectangle linked_list_button = {500, 288, 293, 74};
        Rectangle trie_button = {916, 288, 293, 74};
        Rectangle heap_button = {84, 467, 293, 74};
        Rectangle mst_button = {500, 467, 293, 74};
        Rectangle sp_button = {916, 467, 293, 74};

        BeginDrawing();
        ClearBackground({255, 255, 136});

        DrawText("DATA STRUCTURE VISUALIZATION", 210, 116, 48, BLACK);

        EndDrawing();
    }
}