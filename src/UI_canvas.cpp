#include "../include/UI_canvas.h"

#include <fstream>

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

    //=================================MENU==============================================================
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

    //=================================AVL==============================================================

    void AVL_Canvas::setup() {
        //Buttons setup
        input_text_field_1 = {21, 653, 168, 45};
        input_text_field_2 = {21, 595, 80, 45};
        input_text_field_3 = {107, 595, 80, 45};
        insert_button = {198, 653, 100, 45};
        erase_button = {309, 653, 100, 45};
        find_button = {420, 653, 100, 45};
        prev_button = {531, 653, 100, 45};
        next_button = {642, 653, 100, 45};
        skip_button = {753, 654, 100, 45};
        clear_button = {864, 653, 100, 45};
        file_button = {975, 653, 100, 45};
        exit_button = {1086, 653, 100, 45};
        random_button = {334, 595, 126, 45};
        speed_button = {471, 595, 143, 45};
        update_button = {197, 595, 126, 45};
        
        //Input text field setup
        text_string_1[0] = '\0';
        letter_count_1 = 0;
        frames_counter_1 = 0;

        text_string_2[0] = '\0';
        letter_count_2 = 0;
        frames_counter_2 = 0;
        
        text_string_3[0] = '\0';
        letter_count_3 = 0;
        frames_counter_3 = 0;

        //Animation setup
        current_step = -1;
        speed_multiplier = 1;
        pause_timer = time_between_steps;
        is_playing = false;

        //Code highlight setup
        current_operation = OPERATION::NONE;

        //insert highlight code
        highlighter[OPERATION::INSERT].set_start_pos({window_width, 296});
        highlighter[OPERATION::INSERT].set_code_name("INSERT");

        highlighter[OPERATION::INSERT].add("if (cur == nullptr) create_node(x);");
        highlighter[OPERATION::INSERT].add("if (cur->val > x) go_left();");
        highlighter[OPERATION::INSERT].add("else if (cur->val < x) go_right();");
        highlighter[OPERATION::INSERT].add("else return;");
        highlighter[OPERATION::INSERT].add("balance_tree();");
        
        //erase highlight code
        highlighter[OPERATION::ERASE].set_start_pos({window_width, 296});
        highlighter[OPERATION::ERASE].set_code_name("ERASE");

        highlighter[OPERATION::ERASE].add("if (cur == nullptr) return;");
        highlighter[OPERATION::ERASE].add("if (cur->val == x) {");
        highlighter[OPERATION::ERASE].add("   erase(cur);");
        highlighter[OPERATION::ERASE].add("   replace_node();");
        highlighter[OPERATION::ERASE].add("}");
        highlighter[OPERATION::ERASE].add("if (cur->val > x) go_left();");
        highlighter[OPERATION::ERASE].add("else if (cur->val < x) go_right();");
        highlighter[OPERATION::ERASE].add("balance_tree();");

        //find highlight code
        highlighter[OPERATION::FIND].set_start_pos({window_width, 296});
        highlighter[OPERATION::FIND].set_code_name("FIND");

        highlighter[OPERATION::FIND].add("if (cur == nullptr) return;");
        highlighter[OPERATION::FIND].add("if (cur->val == x) return true;");
        highlighter[OPERATION::FIND].add("if (cur->val > x) go_left()");
        highlighter[OPERATION::FIND].add("else go_right();");
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

        // std::cout << "ANIMATION UPDATING " << cur->val << ": " << cur->current_x << ' ' << cur->current_y << " - " << cur->target_x << ' ' << cur->target_y << std::endl;

        float distance = Vector2Distance((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->target_x, cur->target_y});

        if (speed_multiplier == 5) { //Instant mode is on
            distance = 0.0f;
        }

        if (distance > 1.0f) {
            Vector2 new_pos = Vector2Lerp((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->target_x, cur->target_y}, ani_speed * speed_multiplier);
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

    void AVL_Canvas::sync_position(Data_Structure::AVL_Tree::Node* new_root, Data_Structure::AVL_Tree::Node* new_root_parent, Data_Structure::AVL_Tree::Node* &old_root) {
        if (!new_root) return;

        auto old_node = tree.normal_find(old_root, new_root->val);

        if (old_node) {
            // std::cout << "SYNCING " << new_root->val << ": " << new_root->current_x << ' ' << new_root->current_y << " - " << old_node->target_x << ' ' << old_node->target_y << std::endl;
            new_root->current_x = old_node->current_x;
            new_root->current_y = old_node->current_y;
        }
        else {
            if (new_root_parent) {
                new_root->current_x = new_root_parent->current_x;
                new_root->current_y = new_root_parent->current_y;
            }
        }

        sync_position(new_root->left, new_root, old_root);
        sync_position(new_root->right, new_root, old_root);
    }

    void AVL_Canvas::update_animation() {
        if (tree.history.empty() || !is_playing || current_step < 0) return;

        auto current_tree = tree.history[current_step];

        bool is_animating = update_node_position(current_tree.tree_root);

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(current_tree.index);
        }

        // std::cout << "====================================================" << std::endl;
        // std::cout << "Script " << current_step + 1 << " / " << tree.history.size() 
        //           << " | Timer " << pause_timer 
        //           << " | Is moving? " << (is_animating ? "Yes" : "No") << std::endl;

        if (is_animating) {
            pause_timer = time_between_steps / speed_multiplier;
        }
        else {
            pause_timer -= GetFrameTime();

            if (pause_timer <= 0.0f || speed_multiplier == 5) {
                pause_timer = time_between_steps / speed_multiplier;

                if (current_step + 1 < (int)tree.history.size()) {
                    sync_position(tree.history[current_step + 1].tree_root, nullptr, tree.history[current_step].tree_root);
                    ++current_step;
                    current_operation = tree.history[current_step].op;
                }
                else {
                    is_playing = false;
                    if (current_operation != NONE) {
                        highlighter[current_operation].set_highlighted_line(-1);
                    }

                    current_operation = OPERATION::NONE;
                }
            }
        }
    }

    void AVL_Canvas::insert() {
        std::vector<int> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_insert.empty()) {
            for (auto to_insert : val_to_insert) {
                tree.insert(to_insert);
            }

            is_playing = true;
            current_operation = OPERATION::INSERT;

            ++current_step;
            if (current_step > 0) {
                sync_position(tree.history[current_step].tree_root, nullptr, tree.history[current_step - 1].tree_root);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void AVL_Canvas::erase() {
        std::vector<int> val_to_erase;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_erase.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_erase.push_back(std::stoi(cur_num));
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_erase.empty()) {
            for (auto to_insert : val_to_erase) {
                tree.erase(to_insert);
            }

            is_playing = true;
            current_operation = OPERATION::ERASE;

            ++current_step;
            if (current_step > 0) {
                sync_position(tree.history[current_step].tree_root, nullptr, tree.history[current_step - 1].tree_root);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void AVL_Canvas::clear() {
        tree.clear();

        //Animation
        current_step = -1;
        pause_timer = time_between_steps / speed_multiplier;
        is_playing = false;

        //Input text field
        text_string_1[0] = '\0';
        letter_count_1 = 0;
        frames_counter_1 = 0;

        text_string_2[0] = '\0';
        letter_count_2 = 0;
        frames_counter_2 = 0;
        
        text_string_3[0] = '\0';
        letter_count_3 = 0;
        frames_counter_3 = 0;

        //Code highlight
        current_operation = OPERATION::NONE;
    }

    void AVL_Canvas::next() {
        ++current_step;
        current_operation = tree.history[current_step].op;

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(tree.history[current_step].index);
        }
    }

    void AVL_Canvas::prev() {
        --current_step;
        current_operation = tree.history[current_step].op;

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(tree.history[current_step].index);
        }
    }

    void AVL_Canvas::update() {
        std::vector<int> old_val;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_2 && old_val.size() < 1; i++) {
            if (text_string_2[i] != ' ') {
                cur_num.push_back(text_string_2[i]);
            }
            else if (!cur_num.empty()) {
                old_val.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty() && old_val.size() < 1) {
            old_val.push_back(std::stoi(cur_num));
        }

        
        if (old_val.size() != 1) {
            tinyfd_messageBox("ERROR", "Please type in the old value of the data you want to update!", "ok", "error", 1);
            return;
        }
    
        std::vector<int> new_val;
        cur_num = "";
        
        for (int i = 0; i < letter_count_3 && new_val.size() < 1; i++) {
            if (text_string_3[i] != ' ') {
                cur_num.push_back(text_string_3[i]);
            }
            else if (!cur_num.empty()) {
                new_val.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }
        
        if (!cur_num.empty() && new_val.size() < 1) {
            new_val.push_back(std::stoi(cur_num));
        }
        
        if (new_val.size() != 1) {
            tinyfd_messageBox("ERROR", "Please type in the new value of the data you want to update!", "ok", "error", 1);
            return;
        }
        
        letter_count_2 = 0;
        text_string_2[0] = '\0';

        letter_count_3 = 0;
        text_string_3[0] = '\0';

        if (tree.erase(old_val[0])) {
            tree.insert(new_val[0]);
        }

        is_playing = true;
        current_operation = OPERATION::ERASE;

        ++current_step;
        if (current_step > 0) {
            sync_position(tree.history[current_step].tree_root, nullptr, tree.history[current_step - 1].tree_root);
        }

        if (speed_multiplier == 5) { //Instant mode is on
            skip();
        }
    }

    void AVL_Canvas::skip() {
        if (current_step < 0 || tree.history.empty()) return;
        if (!is_playing) {
            current_step = (int)tree.history.size() - 1;
            current_operation = tree.history[current_step].op; 

            if (current_operation != NONE) {
                highlighter[current_operation].set_highlighted_line(tree.history[current_step].index);
            }

            return;
        }

        int prev_speed = speed_multiplier;

        speed_multiplier = 5;
        is_playing = true;
        while (current_step + 1 < (int)tree.history.size()) {
            update_animation();
        }

        speed_multiplier = prev_speed;
        pause_timer = time_between_steps / speed_multiplier;
    }

    void AVL_Canvas::find() {
        std::vector<int> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_insert.empty()) {
            for (auto to_insert : val_to_insert) {
                tree.find(to_insert);
            }

            is_playing = true;
            current_operation = OPERATION::FIND;

            ++current_step;
            if (current_step > 0) {
                sync_position(tree.history[current_step].tree_root, nullptr, tree.history[current_step - 1].tree_root);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void AVL_Canvas::open_file() {
        const char *filter[1] = {"*.txt"};
        const char *file_path = tinyfd_openFileDialog("Select input file", "", 1, filter, "Text files (*.txt)", 0);
        if (file_path != nullptr) { 
            std::ifstream file(file_path);

            if (file.is_open()) {
                letter_count_1 = 0;
                text_string_1[letter_count_1] = '\0';

                char c;
                while (file.get(c) && letter_count_1 < MAX_INPUT_INT_CHAR - 1) {
                    if ((('0' <= c && c <= '9')) || c == ' ') {
                        text_string_1[letter_count_1] = c;
                        ++letter_count_1;
                    }
                    else if (c == '\n') {
                        text_string_1[letter_count_1] = ' ';
                        ++letter_count_1;
                    }
                }
                
                text_string_1[letter_count_1] = '\0';
                file.close();
            }
            else {
                tinyfd_messageBox("ERROR", "Failed to open the file. Please try again!", "ok", "error", 1);
            }
        }
    }

    void AVL_Canvas::run() {
        bool mouse_on_itf_1 = CheckCollisionPointRec(GetMousePosition(), input_text_field_1);
        bool mouse_on_itf_2 = CheckCollisionPointRec(GetMousePosition(), input_text_field_2);
        bool mouse_on_itf_3 = CheckCollisionPointRec(GetMousePosition(), input_text_field_3);

        if (mouse_on_itf_1 && !is_playing) {
            ++frames_counter_1;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('0' <= key && key <= '9') || key == ' ') && letter_count_1 < MAX_INPUT_INT_CHAR) {
                    text_string_1[letter_count_1] = static_cast<char>(key);
                    text_string_1[letter_count_1 + 1] = '\0';
                    ++letter_count_1;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_1;
                if (letter_count_1 < 0) letter_count_1 = 0;
                text_string_1[letter_count_1] = '\0';
            }
        }
        else if (mouse_on_itf_2 && !is_playing) {
            ++frames_counter_2;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('0' <= key && key <= '9') || key == ' ') && letter_count_2 < MAX_INPUT_INT_CHAR) {
                    text_string_2[letter_count_2] = static_cast<char>(key);
                    text_string_2[letter_count_2 + 1] = '\0';
                    ++letter_count_2;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_2;
                if (letter_count_2 < 0) letter_count_2 = 0;
                text_string_2[letter_count_2] = '\0';
            }
        }
        else if (mouse_on_itf_3 && !is_playing) {
            ++frames_counter_3;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('0' <= key && key <= '9') || key == ' ') && letter_count_3 < MAX_INPUT_INT_CHAR) {
                    text_string_3[letter_count_3] = static_cast<char>(key);
                    text_string_3[letter_count_3 + 1] = '\0';
                    ++letter_count_3;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_3;
                if (letter_count_3 < 0) letter_count_3 = 0;
                text_string_3[letter_count_3] = '\0';
            }
        }
        else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            frames_counter_1 = 0;
            frames_counter_2 = 0;
            frames_counter_3 = 0;
        }
        
        if (is_clicked(exit_button)) {
            *current_state = UI_State::MENU;
            clear();
            return;
        }
        else if (!is_playing) { //No animation is running
            if (is_clicked(insert_button)) {
                if (current_step != (int)tree.history.size() - 1) {
                    skip();
                }
                insert();
            }
            else if (is_clicked(prev_button) && current_step > 0) {
                prev();
            }
            else if (is_clicked(next_button) && current_step + 1 < (int)tree.history.size()) {
                next();
            }
            else if (is_clicked(clear_button)) {
                clear();
            }
            else if (is_clicked(speed_button)) {
                switch (speed_multiplier) {
                case 1:
                    speed_multiplier = 2;
                    break;
                case 2:
                    speed_multiplier = 3;
                    break;
                case 3:
                    speed_multiplier = 4;
                    break;
                case 4:
                    speed_multiplier = 5;
                    break;
                
                default:
                    speed_multiplier = 1;
                    break;
                }
            }
            else if (is_clicked(random_button)) {
                clear();
                for (int i = 0; i < 10; i++) {
                    std::string cur_str = std::to_string(get_random_int(1, 100));
                    for (const auto &c : cur_str) {
                        text_string_1[letter_count_1++] = c;
                    }
                    text_string_1[letter_count_1++] = ' ';
                }
                insert();
            }
            else if (is_clicked(update_button)) {
                if (current_step != (int)tree.history.size() - 1) {
                    skip();
                }
                update();
            }
            else if (is_clicked(erase_button)) {
                if (current_step != (int)tree.history.size() - 1) {
                    skip();
                }
                erase();
            }
            else if (is_clicked(find_button)) {
                if (current_step != (int)tree.history.size() - 1) {
                    skip();
                }
                find();
            }
            else if (is_clicked(file_button)) {
                open_file();
            }
        }

        if (is_clicked(skip_button)) {
            skip();
        }

        update_animation();

        BeginDrawing();
        ClearBackground(main_background_color);

        BeginMode2D(*camera);

        if (current_step >= 0) {
            draw_tree(tree.history[current_step].tree_root);
        }

        EndMode2D();

        //Buttons
        draw_button(input_text_field_1, "", WHITE, WHITE);
        draw_button(input_text_field_2, "", WHITE, WHITE);
        draw_button(input_text_field_3, "", WHITE, WHITE);
        draw_button(insert_button, "INSERT", WHITE, is_playing ? GRAY : BLACK);
        draw_button(erase_button, "ERASE", WHITE, is_playing ? GRAY : BLACK);
        draw_button(find_button, "FIND", WHITE, is_playing ? GRAY : BLACK);
        draw_button(prev_button, "PREV", WHITE, is_playing ? GRAY : BLACK);
        draw_button(next_button, "NEXT", WHITE, is_playing ? GRAY : BLACK);
        draw_button(skip_button, "SKIP", WHITE, BLACK);
        draw_button(clear_button, "CLEAR", WHITE, is_playing ? GRAY : BLACK);
        draw_button(file_button, "FILE", WHITE, is_playing ? GRAY : BLACK);
        draw_button(exit_button, "EXIT", WHITE, BLACK);
        draw_button(random_button, "RANDOM", WHITE, is_playing ? GRAY : BLACK);
        draw_button(update_button, "UPDATE", WHITE, is_playing ? GRAY : BLACK);

        std::string speed_text = "SPEED " + (speed_multiplier != 5 ? "x" + std::to_string(speed_multiplier) : "TA`Y");

        draw_button(speed_button, speed_text.c_str(), WHITE, is_playing ? GRAY : BLACK);

        //Input text field drawing

        { //Scope for one time use
        int text_width = MeasureText(text_string_1, 20);
        int text_start_x = input_text_field_1.x + 10;

        if (text_width > input_text_field_1.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_1.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_1.x, input_text_field_1.y, input_text_field_1.width, input_text_field_1.height);

        DrawText(text_string_1, text_start_x, input_text_field_1.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_1 && !is_playing) {
            if (letter_count_1 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_1 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_1.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        { //Scope for one time use
        int text_width = MeasureText(text_string_2, 20);
        int text_start_x = input_text_field_2.x + 10;

        if (text_width > input_text_field_2.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_2.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_2.x, input_text_field_2.y, input_text_field_2.width, input_text_field_2.height);

        DrawText(text_string_2, text_start_x, input_text_field_2.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_2 && !is_playing) {
            if (letter_count_2 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_2 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_2.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        { //Scope for one time use
        int text_width = MeasureText(text_string_3, 20);
        int text_start_x = input_text_field_3.x + 10;

        if (text_width > input_text_field_3.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_3.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_3.x, input_text_field_3.y, input_text_field_3.width, input_text_field_3.height);

        DrawText(text_string_3, text_start_x, input_text_field_3.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_3 && !is_playing) {
            if (letter_count_3 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_3 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_3.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        DrawText("MOUSE WHEEL TO ZOOM IN-OUT", 9, 15, 20, GREEN);
        DrawText("PRESS R TO RESET ZOOM", 9, 45, 20, PURPLE);

        if (!is_playing && ((mouse_on_itf_1 && letter_count_1 >= MAX_INPUT_INT_CHAR) || (mouse_on_itf_2 && letter_count_2 >= MAX_INPUT_INT_CHAR) || (mouse_on_itf_3 && letter_count_3 >= MAX_INPUT_INT_CHAR))) {
            DrawText("MAXIMUM INPUT REACHED", 21, 540, 23, RED);
        }

        //Code highlight drawing
        if (current_operation != NONE) {
            highlighter[current_operation].draw_code();
        }

        EndDrawing();
    }

    //=================================HEAP==============================================================
    void Heap_Canvas::setup() {
        //Buttons setup
        input_text_field_1 = {21, 653, 168, 45};
        input_text_field_2 = {21, 595, 80, 45};
        input_text_field_3 = {107, 595, 80, 45};
        insert_button = {198, 653, 100, 45};
        erase_button = {309, 653, 100, 45};
        find_button = {420, 653, 100, 45};
        prev_button = {531, 653, 100, 45};
        next_button = {642, 653, 100, 45};
        skip_button = {753, 654, 100, 45};
        clear_button = {864, 653, 100, 45};
        file_button = {975, 653, 100, 45};
        exit_button = {1086, 653, 100, 45};
        random_button = {334, 595, 126, 45};
        speed_button = {471, 595, 143, 45};
        update_button = {197, 595, 126, 45};
        
        //Input text field setup
        text_string_1[0] = '\0';
        letter_count_1 = 0;
        frames_counter_1 = 0;

        text_string_2[0] = '\0';
        letter_count_2 = 0;
        frames_counter_2 = 0;
        
        text_string_3[0] = '\0';
        letter_count_3 = 0;
        frames_counter_3 = 0;
        
        //Animation setup
        current_step = -1;
        speed_multiplier = 1;
        pause_timer = time_between_steps;
        is_playing = false;

        //Code highlight setup
        current_operation = OPERATION::NONE;

        //insert highlight code
        highlighter[OPERATION::INSERT].set_start_pos({window_width, 296});
        highlighter[OPERATION::INSERT].set_code_name("INSERT");

        highlighter[OPERATION::INSERT].add("arr.add(x);");
        highlighter[OPERATION::INSERT].add("id = arr.size() - 1;");
        highlighter[OPERATION::INSERT].add("while (id > 0 && arr[parent(id)] < arr[id]) {");
        highlighter[OPERATION::INSERT].add("  swap(arr[parent(id)], arr[id]);");
        highlighter[OPERATION::INSERT].add("  id = parent(id);");
        highlighter[OPERATION::INSERT].add("}");
        
        //erase highlight code
        highlighter[OPERATION::ERASE].set_start_pos({window_width, 296});
        highlighter[OPERATION::ERASE].set_code_name("POP");

        highlighter[OPERATION::ERASE].add("swap(arr[0], arr[arr.size() - 1]);");
        highlighter[OPERATION::ERASE].add("arr.delete_back();");
        highlighter[OPERATION::ERASE].add("heapify(0);");

        //heapify highlight code
        highlighter[OPERATION::HEAPIFY].set_start_pos({window_width, 296});
        highlighter[OPERATION::HEAPIFY].set_code_name("HEAPIFY");

        highlighter[OPERATION::HEAPIFY].add("max_id = get_max();");
        highlighter[OPERATION::HEAPIFY].add("if (max_id != current_id) {");
        highlighter[OPERATION::HEAPIFY].add("  swap(arr[current_id], arr[max_id]);");
        highlighter[OPERATION::HEAPIFY].add("  heapify(max_id);");
        highlighter[OPERATION::HEAPIFY].add("}");

        //update hightlight code
        highlighter[OPERATION::UPDATE].set_start_pos({window_width, 296});
        highlighter[OPERATION::UPDATE].set_code_name("UPDATE");
        
        highlighter[OPERATION::UPDATE].add("int old_val = arr[id]");
        highlighter[OPERATION::UPDATE].add("arr[id] = new_val");
        highlighter[OPERATION::UPDATE].add("if (old_val > new_val) {");
        highlighter[OPERATION::UPDATE].add("  heapify(id);");
        highlighter[OPERATION::UPDATE].add("}");
        highlighter[OPERATION::UPDATE].add("else if (old_val < new_val) {");
        highlighter[OPERATION::UPDATE].add("  while (id > 0 && arr[parent(id)] < arr[id]) {");
        highlighter[OPERATION::UPDATE].add("     swap(arr[parent(id)], arr[id]);");
        highlighter[OPERATION::UPDATE].add("     id = parent(id);");
        highlighter[OPERATION::UPDATE].add("  }");
        highlighter[OPERATION::UPDATE].add("}");

        //find highlight code
        highlighter[OPERATION::FIND].set_start_pos({window_width, 296});
        highlighter[OPERATION::FIND].set_code_name("FIND");

        highlighter[OPERATION::FIND].add("for (int i = 0; i < arr.size(); i++) {");
        highlighter[OPERATION::FIND].add("  if (arr[i] == val) return i;");
        highlighter[OPERATION::FIND].add("}");
        highlighter[OPERATION::FIND].add("return -1;");
    }
    
    bool Heap_Canvas::update_node_position(std::vector<Data_Structure::Heap::Node> &array) {
        bool is_moving = false;
        for (auto &cur : array) {
            // std::cout << "ANIMATION UPDATING " << cur.val << ": " << cur.current_x << ' ' << cur.current_y << " - " << cur.target_x << ' ' << cur.target_y << std::endl;

            float distance = Vector2Distance((Vector2){cur.current_x, cur.current_y}, (Vector2){cur.target_x, cur.target_y});

            if (speed_multiplier == 5) { //Instant mode is on
                distance = 0.0f;
            }

            if (distance > 1.0f) {
                Vector2 new_pos = Vector2Lerp((Vector2){cur.current_x, cur.current_y}, (Vector2){cur.target_x, cur.target_y}, ani_speed * speed_multiplier);
                // std::cout << "NEW POSITION " << cur.val << ": " << cur.current_x << ' ' << cur.current_y << " - " << new_pos.x << ' ' << new_pos.y << std::endl;

                cur.current_x = new_pos.x;
                cur.current_y = new_pos.y;
                is_moving = true;
            } 
            else {
                cur.current_x = cur.target_x;
                cur.current_y = cur.target_y;
            }
        }
        return is_moving;
    }

    void Heap_Canvas::sync_position(std::vector<Data_Structure::Heap::Node> &new_array, std::vector<Data_Structure::Heap::Node> &old_array) {
        for (int i = 0; i < (int)new_array.size(); i++) {
            bool exist = false;
            for (int j = 0; j < (int)old_array.size(); j++) {
                if (new_array[i].id == old_array[j].id) {
                    exist = true;
                    new_array[i].current_x = old_array[j].current_x;
                    new_array[i].current_y = old_array[j].current_y;
                    break;
                }
            }

            if (!exist) {
                if (i > 0) {
                    int par = (i - 1) >> 1;
                    new_array[i].current_x = new_array[par].current_x;
                    new_array[i].current_y = new_array[par].current_y;
                }
            }
        }
    }

    void Heap_Canvas::update_animation() {
        if (heap.history.empty() || !is_playing || current_step < 0) return;

        auto &current_tree = heap.history[current_step];

        bool is_animating = update_node_position(current_tree.array);

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(current_tree.index);
        }

        // std::cout << "====================================================" << std::endl;
        // std::cout << "Script " << current_step + 1 << " / " << heap.history.size() 
        //           << " | Timer " << pause_timer 
        //           << " | Is moving? " << (is_animating ? "Yes" : "No") << std::endl;

        if (is_animating) {
            pause_timer = time_between_steps / speed_multiplier;
        }
        else {
            pause_timer -= GetFrameTime();

            if (pause_timer <= 0.0f || speed_multiplier == 5) {
                pause_timer = time_between_steps / speed_multiplier;

                if (current_step + 1 < (int)heap.history.size()) {
                    sync_position(heap.history[current_step + 1].array, heap.history[current_step].array);
                    ++current_step;
                    current_operation = heap.history[current_step].op;
                }
                else {
                    is_playing = false;
                    if (current_operation != NONE) {
                        highlighter[current_operation].set_highlighted_line(-1);
                    }

                    current_operation = OPERATION::NONE;
                }
            }
        }
    }

    void Heap_Canvas::draw_tree(const std::vector<Data_Structure::Heap::Node> &array) {
        for (int i = 0; i < (int)array.size(); i++) {
            const auto &cur = array[i];

            if ((i << 1) + 1 < (int)array.size()) {
                const auto &child = array[(i << 1) + 1];
                DrawLineEx((Vector2){cur.current_x, cur.current_y}, (Vector2){child.current_x, child.current_y}, 3.0f, (cur.highlighted && child.highlighted) ? RED : BLACK);
            }

            if ((i << 1) + 2 < (int)array.size()) {
                const auto &child = array[(i << 1) + 2];
                DrawLineEx((Vector2){cur.current_x, cur.current_y}, (Vector2){child.current_x, child.current_y}, 3.0f, (cur.highlighted && child.highlighted) ? RED : BLACK);
            }

            draw_node(cur.current_x, cur.current_y, node_radius, cur.highlighted, std::to_string(cur.val).c_str());

            std::string id_label = std::to_string(i);
            auto text_size = MeasureText(id_label.c_str(), 20);
            DrawText(id_label.c_str(), cur.current_x - (text_size >> 1), cur.current_y + 37, 25, BLUE);
        }
    }

    void Heap_Canvas::insert() {
        std::vector<int> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_insert.empty()) {
            for (auto to_insert : val_to_insert) {
                heap.insert(to_insert);
            }

            is_playing = true;
            current_operation = OPERATION::INSERT;

            ++current_step;
            if (current_step > 0) {
                sync_position(heap.history[current_step].array, heap.history[current_step - 1].array);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void Heap_Canvas::erase() {
        if (heap.size() == 0) {
            tinyfd_messageBox("ERROR", "Cannot process the action since the current heap is empty!", "ok", "error", 1);
            return;
        }

        heap.pop();

        is_playing = true;
        current_operation = OPERATION::ERASE;

        ++current_step;
        if (current_step > 0) {
            sync_position(heap.history[current_step].array, heap.history[current_step - 1].array);
        }

        if (speed_multiplier == 5) { //Instant mode is on
            skip();
        }
    }

    void Heap_Canvas::next() {
        ++current_step;
        current_operation = heap.history[current_step].op;

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(heap.history[current_step].index);
        }
    }

    void Heap_Canvas::prev() {
         --current_step;
        current_operation = heap.history[current_step].op;

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(heap.history[current_step].index);
        }
    }

    void Heap_Canvas::clear() {
        heap.clear();

        //Animation
        current_step = -1;
        pause_timer = time_between_steps / speed_multiplier;
        is_playing = false;

        //Input text field
        text_string_1[0] = '\0';
        letter_count_1 = 0;
        frames_counter_1 = 0;

        text_string_2[0] = '\0';
        letter_count_2 = 0;
        frames_counter_2 = 0;
        
        text_string_3[0] = '\0';
        letter_count_3 = 0;
        frames_counter_3 = 0;

        //Code highlight
        current_operation = OPERATION::NONE;
    }

    void Heap_Canvas::skip() {
        if (current_step < 0 || heap.history.empty()) return;
        if (!is_playing) {
            current_step = (int)heap.history.size() - 1;
            current_operation = heap.history[current_step].op; 

            if (current_operation != NONE) {
                highlighter[current_operation].set_highlighted_line(heap.history[current_step].index);
            }

            return;
        }

        int prev_speed = speed_multiplier;

        speed_multiplier = 5;
        is_playing = true;
        while (current_step + 1 < (int)heap.history.size()) {
            update_animation();
        }

        speed_multiplier = prev_speed;
        pause_timer = time_between_steps / speed_multiplier;
    }

    void Heap_Canvas::update() {
        std::vector<int> index_val;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_2 && index_val.size() < 1; i++) {
            if (text_string_2[i] != ' ') {
                cur_num.push_back(text_string_2[i]);
            }
            else if (!cur_num.empty()) {
                index_val.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty() && index_val.size() < 1) {
            index_val.push_back(std::stoi(cur_num));
        }

        
        if (index_val.size() != 1) {
            tinyfd_messageBox("ERROR", "Please type in the index of the data you want to update!", "ok", "error", 1);
            return;
        }
        else if (index_val[0] >= heap.size()) {
            tinyfd_messageBox("ERROR", "The given index is out of range!", "ok", "error", 1);
            return;
        }
    
        std::vector<int> new_val;
        cur_num = "";
        
        for (int i = 0; i < letter_count_3 && new_val.size() < 1; i++) {
            if (text_string_3[i] != ' ') {
                cur_num.push_back(text_string_3[i]);
            }
            else if (!cur_num.empty()) {
                new_val.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }
        
        if (!cur_num.empty() && new_val.size() < 1) {
            new_val.push_back(std::stoi(cur_num));
        }
        
        if (new_val.size() != 1) {
            tinyfd_messageBox("ERROR", "Please type in the new value of the data you want to update!", "ok", "error", 1);
            return;
        }
        
        letter_count_2 = 0;
        text_string_2[0] = '\0';

        letter_count_3 = 0;
        text_string_3[0] = '\0';

        heap.update(index_val[0], new_val[0]); 

        is_playing = true;
        current_operation = OPERATION::UPDATE;

        ++current_step;
        if (current_step > 0) {
            sync_position(heap.history[current_step].array, heap.history[current_step - 1].array);
        }

        if (speed_multiplier == 5) { //Instant mode is on
            skip();
        }
    }

    void Heap_Canvas::find() {
        std::vector<int> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_insert.empty()) {
            for (auto to_insert : val_to_insert) {
                heap.find(to_insert);
            }

            is_playing = true;
            current_operation = OPERATION::FIND;

            ++current_step;
            if (current_step > 0) {
                sync_position(heap.history[current_step].array, heap.history[current_step - 1].array);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void Heap_Canvas::open_file() {
        const char *filter[1] = {"*.txt"};
        const char *file_path = tinyfd_openFileDialog("Select input file", "", 1, filter, "Text files (*.txt)", 0);
        if (file_path != nullptr) { 
            std::ifstream file(file_path);

            if (file.is_open()) {
                letter_count_1 = 0;
                text_string_1[letter_count_1] = '\0';

                char c;
                while (file.get(c) && letter_count_1 < MAX_INPUT_INT_CHAR - 1) {
                    if (('0' <= c && c <= '9') || c == ' ') {
                        text_string_1[letter_count_1] = c;
                        ++letter_count_1;
                    }
                    else if (c == '\n') {
                        text_string_1[letter_count_1] = ' ';
                        ++letter_count_1;
                    }
                }
                
                text_string_1[letter_count_1] = '\0';
                file.close();
            }
            else {
                tinyfd_messageBox("ERROR", "Failed to open the file. Please try again!", "ok", "error", 1);
            }
        }
    }

    void Heap_Canvas::run() {
        bool mouse_on_itf_1 = CheckCollisionPointRec(GetMousePosition(), input_text_field_1);
        bool mouse_on_itf_2 = CheckCollisionPointRec(GetMousePosition(), input_text_field_2);
        bool mouse_on_itf_3 = CheckCollisionPointRec(GetMousePosition(), input_text_field_3);

        if (mouse_on_itf_1 && !is_playing) {
            ++frames_counter_1;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('0' <= key && key <= '9') || key == ' ') && letter_count_1 < MAX_INPUT_INT_CHAR) {
                    text_string_1[letter_count_1] = static_cast<char>(key);
                    text_string_1[letter_count_1 + 1] = '\0';
                    ++letter_count_1;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_1;
                if (letter_count_1 < 0) letter_count_1 = 0;
                text_string_1[letter_count_1] = '\0';
            }
        }
        else if (mouse_on_itf_2 && !is_playing) {
            ++frames_counter_2;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('0' <= key && key <= '9') || key == ' ') && letter_count_2 < MAX_INPUT_INT_CHAR) {
                    text_string_2[letter_count_2] = static_cast<char>(key);
                    text_string_2[letter_count_2 + 1] = '\0';
                    ++letter_count_2;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_2;
                if (letter_count_2 < 0) letter_count_2 = 0;
                text_string_2[letter_count_2] = '\0';
            }
        }
        else if (mouse_on_itf_3 && !is_playing) {
            ++frames_counter_3;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('0' <= key && key <= '9') || key == ' ') && letter_count_3 < MAX_INPUT_INT_CHAR) {
                    text_string_3[letter_count_3] = static_cast<char>(key);
                    text_string_3[letter_count_3 + 1] = '\0';
                    ++letter_count_3;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_3;
                if (letter_count_3 < 0) letter_count_3 = 0;
                text_string_3[letter_count_3] = '\0';
            }
        }
        else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            frames_counter_1 = 0;
            frames_counter_2 = 0;
            frames_counter_3 = 0;
        }
        
        if (is_clicked(exit_button)) {
            *current_state = UI_State::MENU;
            clear();
            return;
        }
        else if (!is_playing) { //No animation is running
            if (is_clicked(insert_button)) {
                if (current_step != (int)heap.history.size() - 1) {
                    skip();
                }
                insert();
            }
            else if (is_clicked(prev_button) && current_step > 0) {
                prev();
            }
            else if (is_clicked(next_button) && current_step + 1 < (int)heap.history.size()) {
                next();
            }
            else if (is_clicked(clear_button)) {
                clear();
            }
            else if (is_clicked(speed_button)) {
                switch (speed_multiplier) {
                case 1:
                    speed_multiplier = 2;
                    break;
                case 2:
                    speed_multiplier = 3;
                    break;
                case 3:
                    speed_multiplier = 4;
                    break;
                case 4:
                    speed_multiplier = 5;
                    break;
                
                default:
                    speed_multiplier = 1;
                    break;
                }
            }
            else if (is_clicked(random_button)) {
                clear();
                for (int i = 0; i < 10; i++) {
                    std::string cur_str = std::to_string(get_random_int(1, 100));
                    for (const auto &c : cur_str) {
                        text_string_1[letter_count_1++] = c;
                    }
                    text_string_1[letter_count_1++] = ' ';
                }
                insert();
            }
            else if (is_clicked(update_button)) {
                if (current_step != (int)heap.history.size() - 1) {
                    skip();
                }
                update();
            }
            else if (is_clicked(erase_button)) {
                if (current_step != (int)heap.history.size() - 1) {
                    skip();
                }
                erase();
            }
            else if (is_clicked(find_button)) {
                if (current_step != (int)heap.history.size() - 1) {
                    skip();
                }
                find();
            }
            else if (is_clicked(file_button)) {
                open_file();
            }
        }

        if (is_clicked(skip_button)) {
            skip();
        }

        update_animation();

        BeginDrawing();
        ClearBackground(main_background_color);

        BeginMode2D(*camera);

        if (current_step >= 0) {
            draw_tree(heap.history[current_step].array);
        }

        EndMode2D();

        //Buttons
        draw_button(input_text_field_1, "", WHITE, WHITE);
        draw_button(input_text_field_2, "", WHITE, WHITE);
        draw_button(input_text_field_3, "", WHITE, WHITE);
        draw_button(insert_button, "INSERT", WHITE, is_playing ? GRAY : BLACK);
        draw_button(erase_button, "POP", WHITE, is_playing ? GRAY : BLACK);
        draw_button(find_button, "FIND", WHITE, is_playing ? GRAY : BLACK);
        draw_button(prev_button, "PREV", WHITE, is_playing ? GRAY : BLACK);
        draw_button(next_button, "NEXT", WHITE, is_playing ? GRAY : BLACK);
        draw_button(skip_button, "SKIP", WHITE, BLACK);
        draw_button(clear_button, "CLEAR", WHITE, is_playing ? GRAY : BLACK);
        draw_button(file_button, "FILE", WHITE, is_playing ? GRAY : BLACK);
        draw_button(exit_button, "EXIT", WHITE, BLACK);
        draw_button(random_button, "RANDOM", WHITE, is_playing ? GRAY : BLACK);
        draw_button(update_button, "UPDATE", WHITE, is_playing ? GRAY : BLACK);

        std::string speed_text = "SPEED " + (speed_multiplier != 5 ? "x" + std::to_string(speed_multiplier) : "TA`Y");

        draw_button(speed_button, speed_text.c_str(), WHITE, is_playing ? GRAY : BLACK);

        //Input text field drawing

        { //Scope for one time use
        int text_width = MeasureText(text_string_1, 20);
        int text_start_x = input_text_field_1.x + 10;

        if (text_width > input_text_field_1.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_1.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_1.x, input_text_field_1.y, input_text_field_1.width, input_text_field_1.height);

        DrawText(text_string_1, text_start_x, input_text_field_1.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_1 && !is_playing) {
            if (letter_count_1 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_1 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_1.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        { //Scope for one time use
        int text_width = MeasureText(text_string_2, 20);
        int text_start_x = input_text_field_2.x + 10;

        if (text_width > input_text_field_2.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_2.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_2.x, input_text_field_2.y, input_text_field_2.width, input_text_field_2.height);

        DrawText(text_string_2, text_start_x, input_text_field_2.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_2 && !is_playing) {
            if (letter_count_2 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_2 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_2.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        { //Scope for one time use
        int text_width = MeasureText(text_string_3, 20);
        int text_start_x = input_text_field_3.x + 10;

        if (text_width > input_text_field_3.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_3.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_3.x, input_text_field_3.y, input_text_field_3.width, input_text_field_3.height);

        DrawText(text_string_3, text_start_x, input_text_field_3.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_3 && !is_playing) {
            if (letter_count_3 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_3 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_3.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        DrawText("MOUSE WHEEL TO ZOOM IN-OUT", 9, 15, 20, GREEN);
        DrawText("PRESS R TO RESET ZOOM", 9, 45, 20, PURPLE);

        if (!is_playing && ((mouse_on_itf_1 && letter_count_1 >= MAX_INPUT_INT_CHAR) || (mouse_on_itf_2 && letter_count_2 >= MAX_INPUT_INT_CHAR) || (mouse_on_itf_3 && letter_count_3 >= MAX_INPUT_INT_CHAR))) {
            DrawText("MAXIMUM INPUT REACHED", 21, 540, 23, RED);
        }

        //Code highlight drawing
        if (current_operation != NONE) {
            highlighter[current_operation].draw_code();
        }

        EndDrawing();
    }

    //=================================Linked List==============================================================

    void LinkedList_Canvas::setup() {
        //Buttons setup
        input_text_field_1 = {21, 653, 168, 45};
        input_text_field_2 = {21, 595, 80, 45};
        input_text_field_3 = {107, 595, 80, 45};
        insert_button = {198, 653, 100, 45};
        erase_button = {309, 653, 100, 45};
        find_button = {420, 653, 100, 45};
        prev_button = {531, 653, 100, 45};
        next_button = {642, 653, 100, 45};
        skip_button = {753, 654, 100, 45};
        clear_button = {864, 653, 100, 45};
        file_button = {975, 653, 100, 45};
        exit_button = {1086, 653, 100, 45};
        random_button = {334, 595, 126, 45};
        speed_button = {471, 595, 143, 45};
        update_button = {197, 595, 126, 45};
        head_button = {197 + 7, 528 + 5, 100, 45};
        tail_button = {197 + 122, 528 + 5, 100, 45};
        
        //Input text field setup
        text_string_1[0] = '\0';
        letter_count_1 = 0;
        frames_counter_1 = 0;

        text_string_2[0] = '\0';
        letter_count_2 = 0;
        frames_counter_2 = 0;
        
        text_string_3[0] = '\0';
        letter_count_3 = 0;
        frames_counter_3 = 0;
        
        //Animation setup
        current_step = -1;
        speed_multiplier = 1;
        pause_timer = time_between_steps;
        is_playing = false;

        //Code highlight setup
        current_operation = OPERATION::NONE;

        //insert highlight code
        highlighter[OPERATION::INSERT].set_start_pos({window_width, 296});
        highlighter[OPERATION::INSERT].set_code_name("INSERT");

        highlighter[OPERATION::INSERT].add("if (pHead == nullptr) new_head(x);");
        highlighter[OPERATION::INSERT].add("else if (insert_at_the_end) add_to_end(x);");
        highlighter[OPERATION::INSERT].add("else add_to_head(x);");
        
        //erase highlight code
        highlighter[OPERATION::ERASE].set_start_pos({window_width, 296});
        highlighter[OPERATION::ERASE].set_code_name("ERASE");

        highlighter[OPERATION::ERASE].add("if (pHead == nullptr) return;");
        highlighter[OPERATION::ERASE].add("cur = pHead;");
        highlighter[OPERATION::ERASE].add("while (cur != nullptr) {");
        highlighter[OPERATION::ERASE].add("  if (cur->val == x) delete cur, break;");
        highlighter[OPERATION::ERASE].add("  cur = cur->pNext;");
        highlighter[OPERATION::ERASE].add("}");


        //find highlight code
        highlighter[OPERATION::FIND].set_start_pos({window_width, 296});
        highlighter[OPERATION::FIND].set_code_name("FIND");

        highlighter[OPERATION::FIND].add("cur = pHead;");
        highlighter[OPERATION::FIND].add("while (cur != nullptr) {");
        highlighter[OPERATION::FIND].add("  if (cur->val == x) return cur;");
        highlighter[OPERATION::FIND].add("  cur = cur->pNext;");
        highlighter[OPERATION::FIND].add("}");
        highlighter[OPERATION::FIND].add("return nullptr;");

        //Pop up
        is_popup_open = false;
    }

    bool LinkedList_Canvas::update_node_position(Data_Structure::Singly_Linked_List::Node* &pHead) {
        bool is_moving = false;
        auto cur = pHead;
        while (cur != nullptr) {
            // std::cout << "ANIMATION UPDATING " << cur.val << ": " << cur.current_x << ' ' << cur.current_y << " - " << cur.target_x << ' ' << cur.target_y << std::endl;

            float distance = Vector2Distance((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->target_x, cur->target_y});

            if (speed_multiplier == 5) { //Instant mode is on
                distance = 0.0f;
            }

            if (distance > 1.0f) {
                Vector2 new_pos = Vector2Lerp((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->target_x, cur->target_y}, ani_speed * speed_multiplier);
                // std::cout << "NEW POSITION " << cur->val << ": " << cur->current_x << ' ' << cur->current_y << " - " << new_pos->x << ' ' << new_pos->y << std::endl;

                cur->current_x = new_pos.x;
                cur->current_y = new_pos.y;
                is_moving = true;
            } 
            else {
                cur->current_x = cur->target_x;
                cur->current_y = cur->target_y;
            }

            cur = cur->pNext;
        }
        return is_moving;
    }

    void LinkedList_Canvas::sync_position(Data_Structure::Singly_Linked_List::Node* &new_pHead, Data_Structure::Singly_Linked_List::Node* &old_pHead) {
        Data_Structure::Singly_Linked_List::Node* pre = nullptr;
        for (auto i = new_pHead; i; i = i->pNext) {
            bool found = false;
            for (auto j = old_pHead; j; j = j->pNext) {
                if (i->id == j->id) {
                    found = true;
                    i->current_x = j->current_x;
                    i->current_y = j->current_y;
                    break;
                }
            }

            if (!found) {
                if (pre) {
                    i->current_x = pre->current_x;
                    i->current_y = pre->current_y;
                }
            }

            pre = i;
        }
    }

    void LinkedList_Canvas::update_animation() {
        if (linked_list.history.empty() || !is_playing || current_step < 0) return;

        auto current_tree = linked_list.history[current_step];

        bool is_animating = update_node_position(current_tree.pHead);

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(current_tree.index);
        }

        // std::cout << "====================================================" << std::endl;
        // std::cout << "Script " << current_step + 1 << " / " << linked_list.history.size() 
                //   << " | Timer " << pause_timer 
                //   << " | Is moving? " << (is_animating ? "Yes" : "No") << std::endl;

        if (is_animating) {
            pause_timer = time_between_steps / speed_multiplier;
        }
        else {
            pause_timer -= GetFrameTime();

            if (pause_timer <= 0.0f || speed_multiplier == 5) {
                pause_timer = time_between_steps / speed_multiplier;

                if (current_step + 1 < (int)linked_list.history.size()) {
                    sync_position(linked_list.history[current_step + 1].pHead, linked_list.history[current_step].pHead);
                    ++current_step;
                    current_operation = linked_list.history[current_step].op;
                }
                else {
                    is_playing = false;
                    if (current_operation != NONE) {
                        highlighter[current_operation].set_highlighted_line(-1);
                    }

                    current_operation = OPERATION::NONE;
                }
            }
        }
    }

    void LinkedList_Canvas::draw_tree(Data_Structure::Singly_Linked_List::Node* pHead) {
        if (!pHead) return;

        while (pHead != nullptr) {
            if (pHead->pNext) {
                DrawLineEx((Vector2){pHead->current_x, pHead->current_y}, (Vector2){pHead->pNext->current_x, pHead->pNext->current_y}, 3.0f, (pHead->highlighted && pHead->pNext->highlighted) ? RED : BLACK);

                float dx = pHead->pNext->current_x - pHead->current_x;
                float dy = pHead->pNext->current_y - pHead->current_y;
                float length = std::sqrt(dx * dx + dy * dy);

                Vector2 end_pos = {pHead->pNext->current_x - node_radius * (dx / length), pHead->pNext->current_y - node_radius * (dy / length)};

                float angle = std::atan2(dy, dx);

                //Left
                DrawLineEx(end_pos, (Vector2){end_pos.x - 15 * std::cos(angle - PI / 6.0f), end_pos.y - 15 * std::sin(angle - PI / 6.0f)}, 3.0f, (pHead->highlighted && pHead->pNext->highlighted) ? RED : BLACK);

                //Right
                DrawLineEx(end_pos, (Vector2){end_pos.x - 15 * std::cos(angle + PI / 6.0f), end_pos.y - 15 * std::sin(angle + PI / 6.0f)}, 3.0f, (pHead->highlighted && pHead->pNext->highlighted) ? RED : BLACK);
            }
            
            std::string val_label = std::to_string(pHead->val);
            draw_node(pHead->current_x, pHead->current_y, node_radius, pHead->highlighted, val_label.c_str());

            pHead = pHead->pNext;
        }
    }

    void LinkedList_Canvas::insert(bool insert_at_the_end) {
        std::vector<int> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_insert.empty()) {
            for (auto to_insert : val_to_insert) {
                linked_list.insert(to_insert, insert_at_the_end);
            }

            is_playing = true;
            current_operation = OPERATION::INSERT;

            ++current_step;
            if (current_step > 0) {
                sync_position(linked_list.history[current_step].pHead, linked_list.history[current_step - 1].pHead);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void LinkedList_Canvas::erase() {
        std::vector<int> val_to_erase;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_erase.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_erase.push_back(std::stoi(cur_num));
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_erase.empty()) {
            for (auto to_insert : val_to_erase) {
                linked_list.erase(to_insert);
            }

            is_playing = true;
            current_operation = OPERATION::ERASE;

            ++current_step;
            if (current_step > 0) {
                sync_position(linked_list.history[current_step].pHead, linked_list.history[current_step - 1].pHead);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void LinkedList_Canvas::next() {
        ++current_step;
        current_operation = linked_list.history[current_step].op;

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(linked_list.history[current_step].index);
        }
    }

    void LinkedList_Canvas::prev() {
        --current_step;
        current_operation = linked_list.history[current_step].op;

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(linked_list.history[current_step].index);
        }
    }

    void LinkedList_Canvas::clear() {
        linked_list.clear();

        //Animation
        current_step = -1;
        pause_timer = time_between_steps / speed_multiplier;
        is_playing = false;

        //Input text field
        text_string_1[0] = '\0';
        letter_count_1 = 0;
        frames_counter_1 = 0;

        text_string_2[0] = '\0';
        letter_count_2 = 0;
        frames_counter_2 = 0;
        
        text_string_3[0] = '\0';
        letter_count_3 = 0;
        frames_counter_3 = 0;

        //Code highlight
        current_operation = OPERATION::NONE;
    }

    void LinkedList_Canvas::skip() {
        if (current_step < 0 || linked_list.history.empty()) return;
        if (!is_playing) {
            current_step = (int)linked_list.history.size() - 1;
            current_operation = linked_list.history[current_step].op; 

            if (current_operation != NONE) {
                highlighter[current_operation].set_highlighted_line(linked_list.history[current_step].index);
            }

            return;
        }

        int prev_speed = speed_multiplier;

        speed_multiplier = 5;
        is_playing = true;
        while (current_step + 1 < (int)linked_list.history.size()) {
            update_animation();
        }

        speed_multiplier = prev_speed;
        pause_timer = time_between_steps / speed_multiplier;
    }

    void LinkedList_Canvas::update() {
        std::vector<int> old_val;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_2 && old_val.size() < 1; i++) {
            if (text_string_2[i] != ' ') {
                cur_num.push_back(text_string_2[i]);
            }
            else if (!cur_num.empty()) {
                old_val.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty() && old_val.size() < 1) {
            old_val.push_back(std::stoi(cur_num));
        }

        
        if (old_val.size() != 1) {
            tinyfd_messageBox("ERROR", "Please type in the old value of the data you want to update!", "ok", "error", 1);
            return;
        }
    
        std::vector<int> new_val;
        cur_num = "";
        
        for (int i = 0; i < letter_count_3 && new_val.size() < 1; i++) {
            if (text_string_3[i] != ' ') {
                cur_num.push_back(text_string_3[i]);
            }
            else if (!cur_num.empty()) {
                new_val.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }
        
        if (!cur_num.empty() && new_val.size() < 1) {
            new_val.push_back(std::stoi(cur_num));
        }
        
        if (new_val.size() != 1) {
            tinyfd_messageBox("ERROR", "Please type in the new value of the data you want to update!", "ok", "error", 1);
            return;
        }
        
        letter_count_2 = 0;
        text_string_2[0] = '\0';

        letter_count_3 = 0;
        text_string_3[0] = '\0';

        auto pos = linked_list.find(old_val[0]);
        if (pos != nullptr) {
            pos->val = new_val[0];
            linked_list.save_snapshot(-1, OPERATION::NONE);
            pos->highlighted = false;
            linked_list.save_snapshot(-1, OPERATION::NONE);
        }

        is_playing = true;
        current_operation = OPERATION::FIND;

        ++current_step;
        if (current_step > 0) {
            sync_position(linked_list.history[current_step].pHead, linked_list.history[current_step - 1].pHead);
        }

        if (speed_multiplier == 5) { //Instant mode is on
            skip();
        }
    }

    void LinkedList_Canvas::find() {
        std::vector<int> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_insert.empty()) {
            for (auto to_insert : val_to_insert) {
                auto res = linked_list.find(to_insert);
                if (res) {
                    res->highlighted = false;
                    linked_list.save_snapshot(-1, UI::OPERATION::NONE);
                }
            }

            is_playing = true;
            current_operation = OPERATION::FIND;

            ++current_step;
            if (current_step > 0) {
                sync_position(linked_list.history[current_step].pHead, linked_list.history[current_step - 1].pHead);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

   void LinkedList_Canvas::open_file() {
    const char *filter[1] = {"*.txt"};
        const char *file_path = tinyfd_openFileDialog("Select input file", "", 1, filter, "Text files (*.txt)", 0);
        if (file_path != nullptr) { 
            std::ifstream file(file_path);

            if (file.is_open()) {
                letter_count_1 = 0;
                text_string_1[letter_count_1] = '\0';

                char c;
                while (file.get(c) && letter_count_1 < MAX_INPUT_INT_CHAR - 1) {
                    if ((((c > '0' || (c == '0' && letter_count_1 > 0 && text_string_1[letter_count_1 - 1] != ' ')) && c <= '9')) || c == ' ') {
                        text_string_1[letter_count_1] = c;
                        ++letter_count_1;
                    }
                    else if (c == '\n') {
                        text_string_1[letter_count_1] = ' ';
                        ++letter_count_1;
                    }
                }
                
                text_string_1[letter_count_1] = '\0';
                file.close();
            }
            else {
                tinyfd_messageBox("ERROR", "Failed to open the file. Please try again!", "ok", "error", 1);
            }
        }
   } 

    void LinkedList_Canvas::run() {
        bool mouse_on_itf_1 = CheckCollisionPointRec(GetMousePosition(), input_text_field_1);
        bool mouse_on_itf_2 = CheckCollisionPointRec(GetMousePosition(), input_text_field_2);
        bool mouse_on_itf_3 = CheckCollisionPointRec(GetMousePosition(), input_text_field_3);

        if (mouse_on_itf_1 && !is_playing) {
            ++frames_counter_1;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('0' <= key && key <= '9') || key == ' ') && letter_count_1 < MAX_INPUT_INT_CHAR) {
                    text_string_1[letter_count_1] = static_cast<char>(key);
                    text_string_1[letter_count_1 + 1] = '\0';
                    ++letter_count_1;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_1;
                if (letter_count_1 < 0) letter_count_1 = 0;
                text_string_1[letter_count_1] = '\0';
            }
        }
        else if (mouse_on_itf_2 && !is_playing) {
            ++frames_counter_2;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('0' <= key && key <= '9') || key == ' ') && letter_count_2 < MAX_INPUT_INT_CHAR) {
                    text_string_2[letter_count_2] = static_cast<char>(key);
                    text_string_2[letter_count_2 + 1] = '\0';
                    ++letter_count_2;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_2;
                if (letter_count_2 < 0) letter_count_2 = 0;
                text_string_2[letter_count_2] = '\0';
            }
        }
        else if (mouse_on_itf_3 && !is_playing) {
            ++frames_counter_3;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('0' <= key && key <= '9') || key == ' ') && letter_count_3 < MAX_INPUT_INT_CHAR) {
                    text_string_3[letter_count_3] = static_cast<char>(key);
                    text_string_3[letter_count_3 + 1] = '\0';
                    ++letter_count_3;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_3;
                if (letter_count_3 < 0) letter_count_3 = 0;
                text_string_3[letter_count_3] = '\0';
            }
        }
        else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            frames_counter_1 = 0;
            frames_counter_2 = 0;
            frames_counter_3 = 0;
        }
        
        if (is_clicked(exit_button)) {
            *current_state = UI_State::MENU;
            clear();
            return;
        }
        else if (!is_playing) { //No animation is running
            if (is_clicked(insert_button)) {
                is_popup_open ^= 1;
            }
            else if (is_popup_open) {
                if (is_clicked(head_button)) {
                    if (current_step != (int)linked_list.history.size() - 1) {
                        skip();
                    }

                    insert(false);
                    is_popup_open = false;
                }
                else if (is_clicked(tail_button)) {
                    if (current_step != (int)linked_list.history.size() - 1) {
                        skip();
                    }

                    insert(true);
                    is_popup_open = false;
                }
            }
            else if (is_clicked(prev_button) && current_step > 0) {
                prev();
            }
            else if (is_clicked(next_button) && current_step + 1 < (int)linked_list.history.size()) {
                next();
            }
            else if (is_clicked(clear_button)) {
                clear();
            }
            else if (is_clicked(speed_button)) {
                switch (speed_multiplier) {
                case 1:
                    speed_multiplier = 2;
                    break;
                case 2:
                    speed_multiplier = 3;
                    break;
                case 3:
                    speed_multiplier = 4;
                    break;
                case 4:
                    speed_multiplier = 5;
                    break;
                
                default:
                    speed_multiplier = 1;
                    break;
                }
            }
            else if (is_clicked(random_button)) {
                clear();
                for (int i = 0; i < 10; i++) {
                    std::string cur_str = std::to_string(get_random_int(1, 100));
                    for (const auto &c : cur_str) {
                        text_string_1[letter_count_1++] = c;
                    }
                    text_string_1[letter_count_1++] = ' ';
                }
                insert(true);
            }
            else if (is_clicked(update_button)) {
                if (current_step != (int)linked_list.history.size() - 1) {
                    skip();
                }
                update();
            }
            else if (is_clicked(erase_button)) {
                if (current_step != (int)linked_list.history.size() - 1) {
                    skip();
                }
                erase();
            }
            else if (is_clicked(find_button)) {
                if (current_step != (int)linked_list.history.size() - 1) {
                    skip();
                }
                find();
            }
            else if (is_clicked(file_button)) {
                open_file();
            }
        }

        if (is_clicked(skip_button)) {
            skip();
        }

        update_animation();

        BeginDrawing();
        ClearBackground(main_background_color);

        BeginMode2D(*camera);

        if (current_step >= 0) {
            draw_tree(linked_list.history[current_step].pHead);
        }

        EndMode2D();

        //Buttons
        draw_button(input_text_field_1, "", WHITE, WHITE);
        draw_button(input_text_field_2, "", WHITE, WHITE);
        draw_button(input_text_field_3, "", WHITE, WHITE);
        draw_button(insert_button, "INSERT", WHITE, is_playing ? GRAY : BLACK);
        draw_button(erase_button, "ERASE", WHITE, is_playing ? GRAY : BLACK);
        draw_button(find_button, "FIND", WHITE, is_playing ? GRAY : BLACK);
        draw_button(prev_button, "PREV", WHITE, is_playing ? GRAY : BLACK);
        draw_button(next_button, "NEXT", WHITE, is_playing ? GRAY : BLACK);
        draw_button(skip_button, "SKIP", WHITE, BLACK);
        draw_button(clear_button, "CLEAR", WHITE, is_playing ? GRAY : BLACK);
        draw_button(file_button, "FILE", WHITE, is_playing ? GRAY : BLACK);
        draw_button(exit_button, "EXIT", WHITE, BLACK);
        draw_button(random_button, "RANDOM", WHITE, is_playing ? GRAY : BLACK);
        draw_button(update_button, "UPDATE", WHITE, is_playing ? GRAY : BLACK);

        //Pop up box drawing
        if (is_popup_open) {
            DrawRectangle(197, 528, 229, 54, (Color){137, 111, 217, 255});
            draw_button(head_button, "HEAD", WHITE, BLACK);
            draw_button(tail_button, "TAIL", WHITE, BLACK);
        }

        std::string speed_text = "SPEED " + (speed_multiplier != 5 ? "x" + std::to_string(speed_multiplier) : "TA`Y");

        draw_button(speed_button, speed_text.c_str(), WHITE, is_playing ? GRAY : BLACK);

        //Input text field drawing

        { //Scope for one time use
        int text_width = MeasureText(text_string_1, 20);
        int text_start_x = input_text_field_1.x + 10;

        if (text_width > input_text_field_1.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_1.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_1.x, input_text_field_1.y, input_text_field_1.width, input_text_field_1.height);

        DrawText(text_string_1, text_start_x, input_text_field_1.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_1 && !is_playing) {
            if (letter_count_1 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_1 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_1.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        { //Scope for one time use
        int text_width = MeasureText(text_string_2, 20);
        int text_start_x = input_text_field_2.x + 10;

        if (text_width > input_text_field_2.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_2.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_2.x, input_text_field_2.y, input_text_field_2.width, input_text_field_2.height);

        DrawText(text_string_2, text_start_x, input_text_field_2.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_2 && !is_playing) {
            if (letter_count_2 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_2 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_2.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        { //Scope for one time use
        int text_width = MeasureText(text_string_3, 20);
        int text_start_x = input_text_field_3.x + 10;

        if (text_width > input_text_field_3.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_3.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_3.x, input_text_field_3.y, input_text_field_3.width, input_text_field_3.height);

        DrawText(text_string_3, text_start_x, input_text_field_3.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_3 && !is_playing) {
            if (letter_count_3 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_3 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_3.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        DrawText("MOUSE WHEEL TO ZOOM IN-OUT", 9, 15, 20, GREEN);
        DrawText("PRESS R TO RESET ZOOM", 9, 45, 20, PURPLE);

        if (!is_playing && ((mouse_on_itf_1 && letter_count_1 >= MAX_INPUT_INT_CHAR) || (mouse_on_itf_2 && letter_count_2 >= MAX_INPUT_INT_CHAR) || (mouse_on_itf_3 && letter_count_3 >= MAX_INPUT_INT_CHAR))) {
            DrawText("MAXIMUM INPUT REACHED", 21, 540, 23, RED);
        }

        //Code highlight drawing
        if (current_operation != NONE) {
            highlighter[current_operation].draw_code();
        }

        EndDrawing();
    }

    //=================================Trie==============================================================
    void Trie_Canvas::setup() {
        //Buttons setup
        input_text_field_1 = {21, 653, 168, 45};
        input_text_field_2 = {21, 595, 80, 45};
        input_text_field_3 = {107, 595, 80, 45};
        insert_button = {198, 653, 100, 45};
        erase_button = {309, 653, 100, 45};
        find_button = {420, 653, 100, 45};
        prev_button = {531, 653, 100, 45};
        next_button = {642, 653, 100, 45};
        skip_button = {753, 654, 100, 45};
        clear_button = {864, 653, 100, 45};
        file_button = {975, 653, 100, 45};
        exit_button = {1086, 653, 100, 45};
        random_button = {334, 595, 126, 45};
        speed_button = {471, 595, 143, 45};
        update_button = {197, 595, 126, 45};
        
        //Input text field setup
        text_string_1[0] = '\0';
        letter_count_1 = 0;
        frames_counter_1 = 0;

        text_string_2[0] = '\0';
        letter_count_2 = 0;
        frames_counter_2 = 0;
        
        text_string_3[0] = '\0';
        letter_count_3 = 0;
        frames_counter_3 = 0;

        //Animation setup
        current_step = -1;
        speed_multiplier = 1;
        pause_timer = time_between_steps;
        is_playing = false;

        //Code highlight setup
        current_operation = OPERATION::NONE;

        //insert highlight code
        highlighter[OPERATION::INSERT].set_start_pos({window_width, 296});
        highlighter[OPERATION::INSERT].set_code_name("INSERT");

        highlighter[OPERATION::INSERT].add("if (find(str) == true) return;");
        highlighter[OPERATION::INSERT].add("cur = root;");
        highlighter[OPERATION::INSERT].add("for (char c : str) {");
        highlighter[OPERATION::INSERT].add("  if (cur->child[c] == nullptr)");
        highlighter[OPERATION::INSERT].add("    cur->child[c] = new Node();");
        highlighter[OPERATION::INSERT].add("  cur = cur->child[c];");
        highlighter[OPERATION::INSERT].add("}");
        
        //erase highlight code
        highlighter[OPERATION::ERASE].set_start_pos({window_width, 296});
        highlighter[OPERATION::ERASE].set_code_name("ERASE");

        highlighter[OPERATION::ERASE].add("if (find(str) == false) return;");
        highlighter[OPERATION::ERASE].add("for (char c : str)");
        highlighter[OPERATION::ERASE].add("  go_to(cur->child[c]);");
        highlighter[OPERATION::ERASE].add("delete(cur);");

        //find highlight code
        highlighter[OPERATION::FIND].set_start_pos({window_width, 296});
        highlighter[OPERATION::FIND].set_code_name("FIND");

        highlighter[OPERATION::FIND].add("cur = root;");
        highlighter[OPERATION::FIND].add("for (char c : str) {");
        highlighter[OPERATION::FIND].add("  if (cur->child[c] == nullptr)");
        highlighter[OPERATION::FIND].add("    return false;");
        highlighter[OPERATION::FIND].add("  cur = cur->child[c];");
        highlighter[OPERATION::FIND].add("}");
        highlighter[OPERATION::FIND].add("if (cur->string_end_here == 0) return false;");
        highlighter[OPERATION::FIND].add("return true");
    }

    void Trie_Canvas::sync_position(Data_Structure::Trie::Node* new_root, Data_Structure::Trie::Node* new_root_parent , Data_Structure::Trie::Node* old_root) {
        if (!new_root) return;

        if (old_root) {
            new_root->current_x = old_root->current_x;
            new_root->current_y = old_root->current_y;
        }
        else if (new_root_parent) {
            new_root->current_x = new_root_parent->current_x;
            new_root->current_y = new_root_parent->current_y;
        }

        for (int i = 0; i < 26; i++) {
            if (new_root->child[i]) {
                auto next_old = old_root ? old_root->child[i] : nullptr;
                sync_position(new_root->child[i], new_root, next_old);
            }
        }
    }

    bool Trie_Canvas::update_node_position(Data_Structure::Trie::Node* cur) {
        if (!cur) return false;

        bool is_moving = false;

        // std::cout << "ANIMATION UPDATING " << cur->val << ": " << cur->current_x << ' ' << cur->current_y << " - " << cur->target_x << ' ' << cur->target_y << std::endl;

        float distance = Vector2Distance((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->target_x, cur->target_y});

        if (speed_multiplier == 5) { //Instant mode is on
            distance = 0.0f;
        }

        if (distance > 1.0f) {
            Vector2 new_pos = Vector2Lerp((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->target_x, cur->target_y}, ani_speed * speed_multiplier);
            cur->current_x = new_pos.x;
            cur->current_y = new_pos.y;
            is_moving = true;
        } 
        else {
            cur->current_x = cur->target_x;
            cur->current_y = cur->target_y;
        }

        for (int i = 0; i < 26; i++) {
            if (cur->child[i]) {
                is_moving |= update_node_position(cur->child[i]);
            }
        }

        return is_moving;
    }

    void Trie_Canvas::update_animation() {
        if (trie.history.empty() || !is_playing || current_step < 0) return;

        auto current_tree = trie.history[current_step];

        bool is_animating = update_node_position(current_tree.tree_root);

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(current_tree.index);
        }

        // std::cout << "====================================================" << std::endl;
        // std::cout << "Script " << current_step + 1 << " / " << tree.history.size() 
        //           << " | Timer " << pause_timer 
        //           << " | Is moving? " << (is_animating ? "Yes" : "No") << std::endl;

        if (is_animating) {
            pause_timer = time_between_steps / speed_multiplier;
        }
        else {
            pause_timer -= GetFrameTime();

            if (pause_timer <= 0.0f || speed_multiplier == 5) {
                pause_timer = time_between_steps / speed_multiplier;

                if (current_step + 1 < (int)trie.history.size()) {
                    sync_position(trie.history[current_step + 1].tree_root, nullptr, trie.history[current_step].tree_root);
                    ++current_step;
                    current_operation = trie.history[current_step].op;
                }
                else {
                    is_playing = false;
                    if (current_operation != NONE) {
                        highlighter[current_operation].set_highlighted_line(-1);
                    }

                    current_operation = OPERATION::NONE;
                }
            }
        }
    }

    void Trie_Canvas::draw_tree(Data_Structure::Trie::Node* cur, std::string label) {
        if (!cur) return;

        for (int i = 0; i < 26; i++) {
            if (cur->child[i]) {

                DrawLineEx((Vector2){cur->current_x, cur->current_y}, (Vector2){cur->child[i]->current_x, cur->child[i]->current_y}, 3.0f, (cur->highlighted && cur->child[i]->highlighted) ? RED : BLACK);

                char c = 'A';
                c += i;
                std::string new_label = "";
                new_label += c;
                draw_tree(cur->child[i], new_label);
            }
        }
        
        draw_node(cur->current_x, cur->current_y, node_radius, cur->highlighted, label.c_str(), 20, cur->exist ? (Color){251, 255, 140, 255} : WHITE);
    }

    void Trie_Canvas::insert() {
        std::vector<std::string> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(cur_num);
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(cur_num);
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_insert.empty()) {
            for (auto to_insert : val_to_insert) {
                trie.insert(to_insert);
            }

            is_playing = true;
            current_operation = OPERATION::INSERT;

            ++current_step;
            if (current_step > 0) {
                sync_position(trie.history[current_step].tree_root, nullptr, trie.history[current_step - 1].tree_root);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void Trie_Canvas::erase() {
        std::vector<std::string> val_to_erase;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_erase.push_back(cur_num);
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_erase.push_back(cur_num);
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_erase.empty()) {
            for (auto to_insert : val_to_erase) {
                trie.erase(to_insert);
            }

            is_playing = true;
            current_operation = OPERATION::ERASE;

            ++current_step;
            if (current_step > 0) {
                sync_position(trie.history[current_step].tree_root, nullptr, trie.history[current_step - 1].tree_root);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void Trie_Canvas::clear() {
        trie.clear();

        //Animation
        current_step = -1;
        pause_timer = time_between_steps / speed_multiplier;
        is_playing = false;

        //Input text field
        text_string_1[0] = '\0';
        letter_count_1 = 0;
        frames_counter_1 = 0;

        text_string_2[0] = '\0';
        letter_count_2 = 0;
        frames_counter_2 = 0;
        
        text_string_3[0] = '\0';
        letter_count_3 = 0;
        frames_counter_3 = 0;

        //Code highlight
        current_operation = OPERATION::NONE;
    }

    void Trie_Canvas::next() {
        ++current_step;
        current_operation = trie.history[current_step].op;

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(trie.history[current_step].index);
        }
    }

    void Trie_Canvas::prev() {
        --current_step;
        current_operation = trie.history[current_step].op;

        if (current_operation != NONE) {
            highlighter[current_operation].set_highlighted_line(trie.history[current_step].index);
        }
    }

    void Trie_Canvas::update() {
        std::vector<std::string> old_val;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_2 && old_val.size() < 1; i++) {
            if (text_string_2[i] != ' ') {
                cur_num.push_back(text_string_2[i]);
            }
            else if (!cur_num.empty()) {
                old_val.push_back(cur_num);
                cur_num = "";
            }
        }

        if (!cur_num.empty() && old_val.size() < 1) {
            old_val.push_back(cur_num);
        }

        
        if (old_val.size() != 1) {
            tinyfd_messageBox("ERROR", "Please type in the old value of the data you want to update!", "ok", "error", 1);
            return;
        }
    
        std::vector<std::string> new_val;
        cur_num = "";
        
        for (int i = 0; i < letter_count_3 && new_val.size() < 1; i++) {
            if (text_string_3[i] != ' ') {
                cur_num.push_back(text_string_3[i]);
            }
            else if (!cur_num.empty()) {
                new_val.push_back(cur_num);
                cur_num = "";
            }
        }
        
        if (!cur_num.empty() && new_val.size() < 1) {
            new_val.push_back(cur_num);
        }
        
        if (new_val.size() != 1) {
            tinyfd_messageBox("ERROR", "Please type in the new value of the data you want to update!", "ok", "error", 1);
            return;
        }
        
        letter_count_2 = 0;
        text_string_2[0] = '\0';

        letter_count_3 = 0;
        text_string_3[0] = '\0';

        if (trie.erase(old_val[0])) {
            trie.insert(new_val[0]);
        }

        is_playing = true;
        current_operation = OPERATION::ERASE;

        ++current_step;
        if (current_step > 0) {
            sync_position(trie.history[current_step].tree_root, nullptr, trie.history[current_step - 1].tree_root);
        }

        if (speed_multiplier == 5) { //Instant mode is on
            skip();
        }
    }

    void Trie_Canvas::skip() {
        if (current_step < 0 || trie.history.empty()) return;
        if (!is_playing) {
            current_step = (int)trie.history.size() - 1;
            current_operation = trie.history[current_step].op; 

            if (current_operation != NONE) {
                highlighter[current_operation].set_highlighted_line(trie.history[current_step].index);
            }

            return;
        }

        int prev_speed = speed_multiplier;

        speed_multiplier = 5;
        is_playing = true;
        while (current_step + 1 < (int)trie.history.size()) {
            update_animation();
        }

        speed_multiplier = prev_speed;
        pause_timer = time_between_steps / speed_multiplier;
    }

    void Trie_Canvas::find() {
        std::vector<std::string> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count_1; i++) {
            if (text_string_1[i] != ' ') {
                cur_num.push_back(text_string_1[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(cur_num);
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(cur_num);
        }

        letter_count_1 = 0;
        text_string_1[0] = '\0';

        if (!val_to_insert.empty()) {
            for (auto to_insert : val_to_insert) {
                trie.find(to_insert);
            }

            is_playing = true;
            current_operation = OPERATION::FIND;

            ++current_step;
            if (current_step > 0) {
                sync_position(trie.history[current_step].tree_root, nullptr, trie.history[current_step - 1].tree_root);
            }

            if (speed_multiplier == 5) { //Instant mode is on
                skip();
            }
        }
        else {
            tinyfd_messageBox("ERROR", "The input is invalid. Please try again!", "ok", "error", 1);
        }
    }

    void Trie_Canvas::open_file() {
        const char *filter[1] = {"*.txt"};
        const char *file_path = tinyfd_openFileDialog("Select input file", "", 1, filter, "Text files (*.txt)", 0);
        if (file_path != nullptr) { 
            std::ifstream file(file_path);

            if (file.is_open()) {
                letter_count_1 = 0;
                text_string_1[letter_count_1] = '\0';

                char c;
                while (file.get(c) && letter_count_1 < MAX_INPUT_INT_CHAR - 1) {
                    if ((('a' <= c && c <= 'z')) || ('A' <= c && c <= 'Z') || c == ' ') {
                        c = toupper(c);
                        text_string_1[letter_count_1] = c;
                        ++letter_count_1;
                    }
                    else if (c == '\n') {
                        text_string_1[letter_count_1] = ' ';
                        ++letter_count_1;
                    }
                }
                
                text_string_1[letter_count_1] = '\0';
                file.close();
            }
            else {
                tinyfd_messageBox("ERROR", "Failed to open the file. Please try again!", "ok", "error", 1);
            }
        }
    }

    void Trie_Canvas::run() {
        bool mouse_on_itf_1 = CheckCollisionPointRec(GetMousePosition(), input_text_field_1);
        bool mouse_on_itf_2 = CheckCollisionPointRec(GetMousePosition(), input_text_field_2);
        bool mouse_on_itf_3 = CheckCollisionPointRec(GetMousePosition(), input_text_field_3);

        if (mouse_on_itf_1 && !is_playing) {
            ++frames_counter_1;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('a' <= key && key <= 'z') || ('A' <= key && key <= 'Z') || key == ' ') && letter_count_1 < MAX_INPUT_INT_CHAR) {
                    char c = static_cast<char>(key);

                    if (c != ' ') {
                        c = toupper(c);
                    }

                    text_string_1[letter_count_1] = c;
                    text_string_1[letter_count_1 + 1] = '\0';
                    ++letter_count_1;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_1;
                if (letter_count_1 < 0) letter_count_1 = 0;
                text_string_1[letter_count_1] = '\0';
            }
        }
        else if (mouse_on_itf_2 && !is_playing) {
            ++frames_counter_2;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('a' <= key && key <= 'z') || ('A' <= key && key <= 'Z') || key == ' ') && letter_count_2 < MAX_INPUT_INT_CHAR) {
                    char c = static_cast<char>(key);
                    
                    if (c != ' ') {
                        c = toupper(c);
                    }

                    text_string_2[letter_count_2] = c;
                    text_string_2[letter_count_2 + 1] = '\0';
                    ++letter_count_2;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_2;
                if (letter_count_2 < 0) letter_count_2 = 0;
                text_string_2[letter_count_2] = '\0';
            }
        }
        else if (mouse_on_itf_3 && !is_playing) {
            ++frames_counter_3;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if ((('a' <= key && key <= 'z') || ('A' <= key && key <= 'Z') || key == ' ') && letter_count_3 < MAX_INPUT_INT_CHAR) {
                    char c = static_cast<char>(key);
                    
                    if (c != ' ') {
                        c = toupper(c);
                    }

                    text_string_3[letter_count_3] = c;
                    text_string_3[letter_count_3 + 1] = '\0';
                    ++letter_count_3;
                }

                key = GetCharPressed();
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                --letter_count_3;
                if (letter_count_3 < 0) letter_count_3 = 0;
                text_string_3[letter_count_3] = '\0';
            }
        }
        else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            frames_counter_1 = 0;
            frames_counter_2 = 0;
            frames_counter_3 = 0;
        }
        
        if (is_clicked(exit_button)) {
            *current_state = UI_State::MENU;
            clear();
            return;
        }
        else if (!is_playing) { //No animation is running
            if (is_clicked(insert_button)) {
                if (current_step != (int)trie.history.size() - 1) {
                    skip();
                }
                insert();
            }
            else if (is_clicked(prev_button) && current_step > 0) {
                prev();
            }
            else if (is_clicked(next_button) && current_step + 1 < (int)trie.history.size()) {
                next();
            }
            else if (is_clicked(clear_button)) {
                clear();
            }
            else if (is_clicked(speed_button)) {
                switch (speed_multiplier) {
                case 1:
                    speed_multiplier = 2;
                    break;
                case 2:
                    speed_multiplier = 3;
                    break;
                case 3:
                    speed_multiplier = 4;
                    break;
                case 4:
                    speed_multiplier = 5;
                    break;
                
                default:
                    speed_multiplier = 1;
                    break;
                }
            }
            else if (is_clicked(random_button)) {
                clear();
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        text_string_1[letter_count_1++] = static_cast<char>(get_random_int('A', 'Z'));
                    }
                    text_string_1[letter_count_1++] = ' ';
                }
                insert();
            }
            else if (is_clicked(update_button)) {
                if (current_step != (int)trie.history.size() - 1) {
                    skip();
                }
                update();
            }
            else if (is_clicked(erase_button)) {
                if (current_step != (int)trie.history.size() - 1) {
                    skip();
                }
                erase();
            }
            else if (is_clicked(find_button)) {
                if (current_step != (int)trie.history.size() - 1) {
                    skip();
                }
                find();
            }
            else if (is_clicked(file_button)) {
                open_file();
            }
        }

        if (is_clicked(skip_button)) {
            skip();
        }

        update_animation();

        BeginDrawing();
        ClearBackground(main_background_color);

        BeginMode2D(*camera);

        if (current_step >= 0) {
            draw_tree(trie.history[current_step].tree_root, "ROOT");
        }

        EndMode2D();

        //Buttons
        draw_button(input_text_field_1, "", WHITE, WHITE);
        draw_button(input_text_field_2, "", WHITE, WHITE);
        draw_button(input_text_field_3, "", WHITE, WHITE);
        draw_button(insert_button, "INSERT", WHITE, is_playing ? GRAY : BLACK);
        draw_button(erase_button, "ERASE", WHITE, is_playing ? GRAY : BLACK);
        draw_button(find_button, "FIND", WHITE, is_playing ? GRAY : BLACK);
        draw_button(prev_button, "PREV", WHITE, is_playing ? GRAY : BLACK);
        draw_button(next_button, "NEXT", WHITE, is_playing ? GRAY : BLACK);
        draw_button(skip_button, "SKIP", WHITE, BLACK);
        draw_button(clear_button, "CLEAR", WHITE, is_playing ? GRAY : BLACK);
        draw_button(file_button, "FILE", WHITE, is_playing ? GRAY : BLACK);
        draw_button(exit_button, "EXIT", WHITE, BLACK);
        draw_button(random_button, "RANDOM", WHITE, is_playing ? GRAY : BLACK);
        draw_button(update_button, "UPDATE", WHITE, is_playing ? GRAY : BLACK);

        std::string speed_text = "SPEED " + (speed_multiplier != 5 ? "x" + std::to_string(speed_multiplier) : "TA`Y");

        draw_button(speed_button, speed_text.c_str(), WHITE, is_playing ? GRAY : BLACK);

        //Input text field drawing

        { //Scope for one time use
        int text_width = MeasureText(text_string_1, 20);
        int text_start_x = input_text_field_1.x + 10;

        if (text_width > input_text_field_1.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_1.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_1.x, input_text_field_1.y, input_text_field_1.width, input_text_field_1.height);

        DrawText(text_string_1, text_start_x, input_text_field_1.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_1 && !is_playing) {
            if (letter_count_1 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_1 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_1.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        { //Scope for one time use
        int text_width = MeasureText(text_string_2, 20);
        int text_start_x = input_text_field_2.x + 10;

        if (text_width > input_text_field_2.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_2.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_2.x, input_text_field_2.y, input_text_field_2.width, input_text_field_2.height);

        DrawText(text_string_2, text_start_x, input_text_field_2.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_2 && !is_playing) {
            if (letter_count_2 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_2 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_2.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        { //Scope for one time use
        int text_width = MeasureText(text_string_3, 20);
        int text_start_x = input_text_field_3.x + 10;

        if (text_width > input_text_field_3.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field_3.width - 2 * 10);
        }

        BeginScissorMode(input_text_field_3.x, input_text_field_3.y, input_text_field_3.width, input_text_field_3.height);

        DrawText(text_string_3, text_start_x, input_text_field_3.y + 13, 20, is_playing ? GRAY : BLACK);

        if (mouse_on_itf_3 && !is_playing) {
            if (letter_count_3 < MAX_INPUT_INT_CHAR) {
                if (((frames_counter_3 / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field_3.y + 15, 20, BLACK);
                }
            }
        }
        EndScissorMode();
        }

        DrawText("MOUSE WHEEL TO ZOOM IN-OUT", 9, 15, 20, GREEN);
        DrawText("PRESS R TO RESET ZOOM", 9, 45, 20, PURPLE);

        if (!is_playing && ((mouse_on_itf_1 && letter_count_1 >= MAX_INPUT_INT_CHAR) || (mouse_on_itf_2 && letter_count_2 >= MAX_INPUT_INT_CHAR) || (mouse_on_itf_3 && letter_count_3 >= MAX_INPUT_INT_CHAR))) {
            DrawText("MAXIMUM INPUT REACHED", 21, 540, 23, RED);
        }

        //Code highlight drawing
        if (current_operation != NONE) {
            highlighter[current_operation].draw_code();
        }

        EndDrawing();
    }
}