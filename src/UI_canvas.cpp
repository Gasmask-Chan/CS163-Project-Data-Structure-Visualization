#include "../include/UI_canvas.h"

namespace UI {
    void Canvas::set_current_state(UI_State *cur_state) {
        current_state = cur_state;
    }

    void Canvas::setup() {}
    void Canvas::run() {}

    void Menu_Canvas::setup() {
        avl_tree_button = {84, 288, 293, 74};
        linked_list_button = {500, 288, 293, 74};
        trie_button = {916, 288, 293, 74};
        heap_button = {84, 467, 293, 74};
        mst_button = {500, 467, 293, 74};
        sp_button = {916, 467, 293, 74};
    }

    void Menu_Canvas::run() {
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

    void AVL_Canvas::setup() {
        //Buttons setup
        input_text_field = {21, 653, 168, 45};
        insert_button = {198, 653, 100, 45};
        erase_button = {309, 653, 100, 45};
        find_button = {420, 653, 100, 45};
        undo_button = {531, 653, 100, 45};
        redo_button = {642, 653, 100, 45};
        clear_button = {753, 653, 100, 45};
        mode_button = {864, 653, 100, 45};
        file_button = {975, 653, 100, 45};
        exit_button = {1086, 653, 100, 45};
        random_button = {21, 599, 126, 45};

        //Input text field setup
        text_string[0] = '\0';
        letter_count = 0;
        frames_counter = 0;
    }

    void AVL_Canvas::run() {
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
        draw_button(random_button, "RANDOM", WHITE, BLACK);

        DrawText(text_string, input_text_field.x + 10, input_text_field.y + 13, 20, BLACK);

        if (mouse_on_input_text_field) {
            if (letter_count < MAX_INPUT_INT_CHAR) {
                if (((frames_counter / 20) & 1) == 0) {
                    DrawText("_", input_text_field.x + 13 + MeasureText(text_string, 20), input_text_field.y + 15, 20, BLACK);
                }
            }
            else {
                DrawText("MAXIMUM INPUT REACHED", 21, 562, 23, RED);
            }
        }
        EndDrawing();
    }
}
