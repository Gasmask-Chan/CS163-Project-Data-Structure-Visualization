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

        const Color button_color ={255, 255, 136, 255};

        BeginDrawing();
        ClearBackground({255, 255, 212});

        DrawText("DATA STRUCTURE VISUALIZATION", 210, 116, 48, BLACK);

        draw_button(avl_tree_button, "AVL TREE", button_color, BLACK);
        draw_button(linked_list_button, "LINKED LIST", button_color, BLACK);
        draw_button(trie_button, "TRIE", button_color, BLACK);
        draw_button(heap_button, "HEAP", button_color, BLACK);
        draw_button(mst_button, "MST", button_color, BLACK);
        draw_button(sp_button, "SP", button_color, BLACK);

        if (is_clicked(avl_tree_button)) *current_state = UI_State::AVL;
        if (is_clicked(linked_list_button)) *current_state = UI_State::LINKED_LIST;
        if (is_clicked(trie_button)) *current_state = UI_State::TRIE;
        if (is_clicked(heap_button)) *current_state = UI_State::HEAP;
        if (is_clicked(mst_button)) *current_state = UI_State::MST;
        if (is_clicked(sp_button)) *current_state = UI_State::SP;

        EndDrawing();
    }

    void AVL_Canvas::run() {
        BeginDrawing();
        ClearBackground({255, 255, 212});
        EndDrawing();
    }
}
