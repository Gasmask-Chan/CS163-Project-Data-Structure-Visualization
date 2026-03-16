#include "../include/UI_canvas.h"

namespace UI {
    void Canvas::set_current_state(UI_State *current_state) {
        this->current_state = current_state;
    }

    void Canvas::set_camera(Camera2D *camera) {
        this->camera = camera;
    }

    void Canvas::setup() {}
    void Canvas::run() {}
    void Canvas::update_animation() {}

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
        prev_button = {531, 653, 100, 45};
        next_button = {642, 653, 100, 45};
        clear_button = {753, 653, 100, 45};
        mode_button = {864, 653, 100, 45};
        file_button = {975, 653, 100, 45};
        exit_button = {1086, 653, 100, 45};
        random_button = {21, 599, 126, 45};
        skip_button = {642, 599, 100, 45};

        //Input text field setup
        text_string[0] = '\0';
        letter_count = 0;
        frames_counter = 0;

        //Animation setup
        current_step = -1;
        ani_speed = 0.05f;
        pause_timer = time_between_steps;
        is_playing = false;
    }

    void AVL_Canvas::draw_tree(Data_Structure::AVL_Tree::Node* cur) {
        if (!cur) return;
        
        if (cur->left) {
            DrawLineEx((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->left->current_x, cur->left->current_y}, 3.0f, (cur->highlighted && cur->left->highlighted) ? RED : BLACK);
            draw_tree(cur->left);
        }
        
        if (cur->right) {
            DrawLineEx((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->right->current_x, cur->right->current_y}, 3.0f, (cur->highlighted && cur->right->highlighted) ? RED : BLACK);
            draw_tree(cur->right);
        }

        std::string label = std::to_string(cur->val);
        draw_node(cur->current_x, cur->current_y, node_radius, cur->highlighted, label.c_str());
    }

    bool AVL_Canvas::update_node_position(Data_Structure::AVL_Tree::Node* cur) {
        if (!cur) return false;

        bool is_moving = false;

        std::cout << "ANIMATION UPDATING " << cur->val << ": " << cur->current_x << ' ' << cur->current_y << " - " << cur->target_x << ' ' << cur->target_y << std::endl;

        float distance = Vector2Distance((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->target_x, cur->target_y});

        if (distance > 1.0f) {
            Vector2 new_pos = Vector2Lerp((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->target_x, cur->target_y}, ani_speed);
            cur->current_x = new_pos.x;
            cur->current_y = new_pos.y;
            is_moving = true;
        } 
        else {
            cur->current_x = cur->target_x;
            cur->current_y = cur->target_y;
        }

        is_moving |= update_node_position(cur->left);
        is_moving |= update_node_position(cur->right);

        return is_moving;
    }

    void AVL_Canvas::sync_position(Data_Structure::AVL_Tree::Node* new_root, Data_Structure::AVL_Tree::Node* &old_root) {
        if (!new_root) return;

        auto old_node = tree.find(old_root, new_root->val);

        if (old_node) {
            std::cout << "MODIFING " << new_root->val << ": " << new_root->current_x << ' ' << new_root->current_y << " - " << old_node->target_x << ' ' << old_node->target_y << std::endl;
            new_root->current_x = old_node->current_x;
            new_root->current_y = old_node->current_y;
        }

        sync_position(new_root->left, old_root);
        sync_position(new_root->right, old_root);
    }

    void AVL_Canvas::update_animation() {
        if (tree.history.empty() || !is_playing || current_step < 0) return;
        
        auto current_tree = tree.history[current_step];
        
        bool is_animating = update_node_position(current_tree);
        std::cout << "====================================================" << std::endl;
        std::cout << "Script " << current_step + 1 << " / " << tree.history.size() 
                  << " | Timer " << pause_timer 
                  << " | Is moving? " << (is_animating ? "Yes" : "No") << std::endl;

        if (is_animating) {
            pause_timer = time_between_steps;
        }
        else {
            pause_timer -= GetFrameTime();

            if (pause_timer <= 0.0f) {
                pause_timer = time_between_steps;

                if (current_step + 1 < (int)tree.history.size()) {
                    sync_position(tree.history[current_step + 1], tree.history[current_step]);
                    ++current_step;
                }
                else {
                    is_playing = false;
                }
            }
        }
    }

    void AVL_Canvas::run() {
        bool mouse_on_input_text_field = CheckCollisionPointRec(GetMousePosition(), input_text_field);

        if (mouse_on_input_text_field && !is_playing) {
            ++frames_counter;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if (((key > '0' || (key == '0' && letter_count > 0)) && key <= '9') && letter_count < MAX_INPUT_INT_CHAR) {
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
        else if (!is_playing) { //No animation is running
            if (is_clicked(insert_button) && letter_count > 0 && current_step == (int)tree.history.size() - 1) {
                int val_to_insert = 0;
                for (int __i = 0; __i < letter_count; __i++) {
                    val_to_insert = val_to_insert * 10 + (text_string[__i] - '0');
                }

                letter_count = 0;
                text_string[0] = '\0';

                tree.insert(val_to_insert);
                is_playing = true;

                ++current_step;
                if (current_step > 0) {
                    sync_position(tree.history[current_step], tree.history[current_step - 1]);
                }
            }
            else if (is_clicked(prev_button) && current_step >= 0) {
                --current_step;
            }
            else if (is_clicked(next_button) && current_step + 1 < (int)tree.history.size()) {
                ++current_step;
            }
            else if (is_clicked(clear_button)) {
                tree.clear();
                current_step = -1;
                pause_timer = time_between_steps;
                is_playing = false;
            }
            else if (is_clicked(skip_button)) {
                current_step = (int)tree.history.size() - 1;
            }
        }

        update_animation();

        BeginDrawing();
        ClearBackground(main_background_color);

        BeginMode2D(*camera);

        if (current_step >= 0) {
            draw_tree(tree.history[current_step]);
        }

        EndMode2D();

        draw_button(input_text_field, "", WHITE, WHITE);
        draw_button(insert_button, "INSERT", WHITE, BLACK);
        draw_button(erase_button, "ERASE", WHITE, BLACK);
        draw_button(find_button, "FIND", WHITE, BLACK);
        draw_button(prev_button, "PREV", WHITE, BLACK);
        draw_button(next_button, "NEXT", WHITE, BLACK);
        draw_button(clear_button, "CLEAR", WHITE, BLACK);
        draw_button(mode_button, "MODE", WHITE, BLACK);
        draw_button(file_button, "FILE", WHITE, BLACK);
        draw_button(exit_button, "EXIT", WHITE, BLACK);
        draw_button(random_button, "RANDOM", WHITE, BLACK);
        draw_button(skip_button, "SKIP", WHITE, BLACK);

        DrawText(text_string, input_text_field.x + 10, input_text_field.y + 13, 20, BLACK);

        if (mouse_on_input_text_field && !is_playing) {
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
