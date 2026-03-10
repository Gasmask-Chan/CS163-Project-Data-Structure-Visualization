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
        
        
        const Color button_color = {255, 255, 136, 255};

        BeginDrawing();
        ClearBackground(main_background_color);
        
        DrawText("DATA STRUCTURE VISUALIZATION", 210, 116, 48, BLACK);
        
        draw_button(avl_tree_button, "AVL TREE", button_color, BLACK, 32);
        draw_button(linked_list_button, "LINKED LIST", button_color, BLACK, 32);
        draw_button(trie_button, "TRIE", button_color, BLACK, 32);
        draw_button(heap_button, "HEAP", button_color, BLACK, 32);
        draw_button(mst_button, "MST", button_color, BLACK, 32);
        draw_button(sp_button, "SP", button_color, BLACK, 32);

        if (is_clicked(avl_tree_button)) *current_state = UI_State::AVL;
        if (is_clicked(linked_list_button)) *current_state = UI_State::LINKED_LIST;
        if (is_clicked(trie_button)) *current_state = UI_State::TRIE;
        if (is_clicked(heap_button)) *current_state = UI_State::HEAP;
        if (is_clicked(mst_button)) *current_state = UI_State::MST;
        if (is_clicked(sp_button)) *current_state = UI_State::SP;

        EndDrawing();
    }

    void AVL_Canvas::run() {
        Rectangle input_text_field = {21, 653, 168, 45};
        Rectangle insert_button = {198, 653, 100, 45};
        Rectangle erase_button = {309, 653, 100, 45};
        Rectangle find_button = {420, 653, 100, 45};
        Rectangle undo_button = {531, 653, 100, 45};
        Rectangle redo_button = {642, 653, 100, 45};
        Rectangle clear_button = {753, 653, 100, 45};
        Rectangle mode_button = {864, 653, 100, 45};
        Rectangle file_button = {975, 653, 100, 45};
        Rectangle exit_button = {1086, 653, 100, 45};

        Data_Structure::AVL_Tree tree;

        char text_string[MAX_INPUT_INT_CHAR + 1] = "\0";
        int letter_count = 0;
        int frames_counter = 0;
 
        while (!WindowShouldClose()) {

            bool mouse_on_input_text_field = CheckCollisionPointRec(GetMousePosition(), input_text_field);

            if (mouse_on_input_text_field) {
                ++frames_counter;
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();

                while (key > 0) {
                    if ((key >= '0' && key <= '9') && letter_count < MAX_INPUT_INT_CHAR) {
                        text_string[letter_count] = static_cast<char>(key);
                        text_string[letter_count + 1] = '\0';
                        ++letter_count;
                    }

                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE)) {
                    --letter_count;
                    if (letter_count < 0) letter_count = 0;
                    text_string[letter_count] = '\0';
                }
            }
            else {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                frames_counter = 0;
            }
            
            if (is_clicked(exit_button)) {
                *current_state = UI_State::MENU;
                return;
            }

            BeginDrawing();
            ClearBackground(main_background_color);

            draw_button(input_text_field, "", WHITE, WHITE);
            draw_button(insert_button, "INSERT", WHITE, BLACK);
            draw_button(erase_button, "ERASE", WHITE, BLACK);
            draw_button(find_button, "FIND", WHITE, BLACK);
            draw_button(undo_button, "UNDO", WHITE, BLACK);
            draw_button(redo_button, "REDO", WHITE, BLACK);
            draw_button(clear_button, "CLEAR", WHITE, BLACK);
            draw_button(mode_button, "MODE", WHITE, BLACK);
            draw_button(file_button, "FILE", WHITE, BLACK);
            draw_button(exit_button, "EXIT", WHITE, BLACK);

            DrawText(text_string, input_text_field.x + 5, input_text_field.y + 13, 20, BLACK);

            if (mouse_on_input_text_field) {
                if (letter_count < MAX_INPUT_INT_CHAR) {
                    if (((frames_counter / 20) & 1) == 0) {
                        DrawText("_", input_text_field.x + 8 + MeasureText(text_string, 20), input_text_field.y + 12, 20, BLACK);
                    }
                }
                else {
                    DrawText("MAXIMUM INPUT REACHED", 21, 612, 23, RED);
                }
            }
            EndDrawing();
        }
    }
}
