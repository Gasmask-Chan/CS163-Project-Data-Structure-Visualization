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
        input_text_field = {21, 653, 168, 45};
        insert_button = {198, 653, 100, 45};
        erase_button = {309, 653, 100, 45};
        find_button = {420, 653, 100, 45};
        prev_button = {531, 653, 100, 45};
        next_button = {642, 653, 100, 45};
        skip_button = {753, 654, 100, 45};
        clear_button = {864, 653, 100, 45};
        file_button = {975, 653, 100, 45};
        exit_button = {1086, 653, 100, 45};
        random_button = {21, 599, 126, 45};
        speed_button = {21, 491, 143, 45};
        update_button = {21, 545, 126, 45};
        
        //Input text field setup
        text_string[0] = '\0';
        letter_count = 0;
        frames_counter = 0;
        
        //Animation setup
        current_step = -1;
        speed_multiplier = 1;
        pause_timer = time_between_steps;
        is_playing = false;

        //Code highlight setup
        current_operation = OPERATION::NONE;

        //insert highlight code
        insert_highlight.set_start_pos({window_width, 296});
        insert_highlight.set_code_name("INSERT");

        insert_highlight.add("if (cur == nullptr) create_node(x);");
        insert_highlight.add("if (cur->val > x) go_left();");
        insert_highlight.add("else if (cur->val < x) go_right();");
        insert_highlight.add("else return;");
        insert_highlight.add("balance_tree();");

        
        //erase highlight code
        erase_highlight.set_start_pos({window_width, 296});
        erase_highlight.set_code_name("ERASE");

        erase_highlight.add("if (cur == nullptr) return;");
        erase_highlight.add("if (cur->val == x) {");
        erase_highlight.add("   erase(cur);");
        erase_highlight.add("   replace_node();");
        erase_highlight.add("}");
        erase_highlight.add("if (cur->val > x) go_left();");
        erase_highlight.add("else if (cur->val < x) go_right();");
        erase_highlight.add("balance_tree();");

        //find highlight code
        find_highlight.set_start_pos({window_width, 296});
        find_highlight.set_code_name("FIND");

        find_highlight.add("if (cur == nullptr) return;");
        find_highlight.add("if (cur->val == x) return true;");
        find_highlight.add("if (cur->val > x) go_left()");
        find_highlight.add("else go_right();");
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
            std::cout << "SYNCING " << new_root->val << ": " << new_root->current_x << ' ' << new_root->current_y << " - " << old_node->target_x << ' ' << old_node->target_y << std::endl;
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

        if (current_operation == INSERT) {
            insert_highlight.set_highlighted_line(current_tree.index);
        }
        else if (current_operation == ERASE) {
            erase_highlight.set_highlighted_line(current_tree.index);
        }
        else if (current_operation == FIND) {
            find_highlight.set_highlighted_line(current_tree.index);
        }

        std::cout << "====================================================" << std::endl;
        std::cout << "Script " << current_step + 1 << " / " << tree.history.size() 
                  << " | Timer " << pause_timer 
                  << " | Is moving? " << (is_animating ? "Yes" : "No") << std::endl;

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
                    if (current_operation == INSERT) {
                        insert_highlight.set_highlighted_line(-1);
                    }
                    else if (current_operation == ERASE) {
                        erase_highlight.set_highlighted_line(-1);
                    }
                    else if (current_operation == FIND) {
                        find_highlight.set_highlighted_line(-1); 
                    }

                    current_operation = OPERATION::NONE;
                }
            }
        }
    }

    void AVL_Canvas::insert() {
        std::vector<int> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count; i++) {
            if (text_string[i] != ' ') {
                cur_num.push_back(text_string[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count = 0;
        text_string[0] = '\0';

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
    }

    void AVL_Canvas::erase() {
        std::vector<int> val_to_erase;
        std::string cur_num = "";

        for (int i = 0; i < letter_count; i++) {
            if (text_string[i] != ' ') {
                cur_num.push_back(text_string[i]);
            }
            else if (!cur_num.empty()) {
                val_to_erase.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_erase.push_back(std::stoi(cur_num));
        }

        letter_count = 0;
        text_string[0] = '\0';

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
    }

    void AVL_Canvas::clear() {
        tree.clear();

        //Animation
        current_step = -1;
        pause_timer = time_between_steps / speed_multiplier;
        is_playing = false;

        //Input text field
        text_string[0] = '\0';
        letter_count = 0;
        frames_counter = 0;

        //Code highlight
        current_operation = OPERATION::NONE;
    }

    void AVL_Canvas::next() {
        ++current_step;
        current_operation = tree.history[current_step].op;
        if (current_operation == INSERT) {
            insert_highlight.set_highlighted_line(tree.history[current_step].index);
        }
        else if (current_operation == ERASE) {
            erase_highlight.set_highlighted_line(tree.history[current_step].index);
        }
        else if (current_operation == FIND) {
            find_highlight.set_highlighted_line(tree.history[current_step].index);
        }
    }

    void AVL_Canvas::prev() {
        --current_step;
        current_operation = tree.history[current_step].op; 
        if (current_operation == INSERT) {
            insert_highlight.set_highlighted_line(tree.history[current_step].index);
        }
        else if (current_operation == ERASE) {
            erase_highlight.set_highlighted_line(tree.history[current_step].index);
        }
        else if (current_operation == FIND) {
            find_highlight.set_highlighted_line(tree.history[current_step].index);
        }
    }

    void AVL_Canvas::update() {
        std::vector<int> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count && val_to_insert.size() < 2; i++) {
            if (text_string[i] != ' ') {
                cur_num.push_back(text_string[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty() && val_to_insert.size() < 2) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count = 0;
        text_string[0] = '\0';

        if (!val_to_insert.empty()) {
            if (tree.erase(val_to_insert[0])) {
                tree.insert(val_to_insert[1]);
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
    }

    void AVL_Canvas::skip() {
        if (current_step < 0 || tree.history.empty()) return;
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

        for (int i = 0; i < letter_count; i++) {
            if (text_string[i] != ' ') {
                cur_num.push_back(text_string[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count = 0;
        text_string[0] = '\0';

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
    }

    void AVL_Canvas::open_file() {
        const char *filter[1] = {"*.txt"};
        const char *file_path = tinyfd_openFileDialog("Select input file", "", 1, filter, "Text files (*.txt)", 0);
        if (file_path != nullptr) { 
            std::ifstream file(file_path);

            if (file.is_open()) {
                letter_count = 0;
                text_string[letter_count] = '\0';

                char c;
                while (file.get(c) && letter_count < MAX_INPUT_INT_CHAR - 1) {
                    if ((((c > '0' || (c == '0' && letter_count > 0 && text_string[letter_count - 1] != ' ')) && c <= '9')) || c == ' ') {
                        text_string[letter_count] = c;
                        ++letter_count;
                    }
                    else if (c == '\n') {
                        text_string[letter_count] = ' ';
                        ++letter_count;
                    }
                }
                
                text_string[letter_count] = '\0';
                file.close();
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
                if (((((key > '0' || (key == '0' && letter_count > 0 && text_string[letter_count - 1] != ' ')) && key <= '9')) || key == ' ') && letter_count < MAX_INPUT_INT_CHAR) {
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
            clear();
            return;
        }
        else if (!is_playing) { //No animation is running
            if (is_clicked(insert_button) && letter_count > 0) {
                if (current_step != (int)tree.history.size() - 1) {
                    skip();
                }
                insert();
            }
            else if (is_clicked(prev_button) && current_step >= 0) {
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
                        text_string[letter_count++] = c;
                    }
                    text_string[letter_count++] = ' ';
                }
                insert();
            }
            else if (is_clicked(update_button) && letter_count > 0) {
                if (current_step != (int)tree.history.size() - 1) {
                    skip();
                }
                update();
            }
            else if (is_clicked(erase_button) && letter_count > 0) {
                if (current_step != (int)tree.history.size() - 1) {
                    skip();
                }
                erase();
            }
            else if (is_clicked(find_button) && letter_count > 0) {
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
        draw_button(input_text_field, "", WHITE, WHITE);
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

        int text_width = MeasureText(text_string, 20);
        int text_start_x = input_text_field.x + 10;

        if (text_width > input_text_field.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field.width - 2 * 10);
        }

        BeginScissorMode(input_text_field.x, input_text_field.y, input_text_field.width, input_text_field.height);

        DrawText(text_string, text_start_x, input_text_field.y + 13, 20, BLACK);

        if (mouse_on_input_text_field && !is_playing) {
            if (letter_count < MAX_INPUT_INT_CHAR) {
                if (((frames_counter / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field.y + 15, 20, BLACK);
                }
            }
        }

        EndScissorMode();

        DrawText("MOUSE WHEEL TO ZOOM IN-OUT", 9, 15, 20, GREEN);
        DrawText("PRESS R TO RESET ZOOM", 9, 45, 20, PURPLE);

        if (mouse_on_input_text_field && !is_playing && letter_count >= MAX_INPUT_INT_CHAR) {
            DrawText("MAXIMUM INPUT REACHED", 198, 607, 23, RED);
        
        }

        //Code highlight drawing
        if (current_operation == INSERT) {
            insert_highlight.draw_code();
        }
        else if (current_operation == ERASE) {
            erase_highlight.draw_code();
        }
        else if (current_operation == FIND) {
            find_highlight.draw_code();
        }

        EndDrawing();
    }

    //=================================HEAP==============================================================
    void Heap_Canvas::setup() {
        //Buttons setup
        input_text_field = {21, 653, 168, 45};
        insert_button = {198, 653, 100, 45};
        erase_button = {309, 653, 100, 45};
        find_button = {420, 653, 100, 45};
        prev_button = {531, 653, 100, 45};
        next_button = {642, 653, 100, 45};
        skip_button = {753, 654, 100, 45};
        clear_button = {864, 653, 100, 45};
        file_button = {975, 653, 100, 45};
        exit_button = {1086, 653, 100, 45};
        random_button = {21, 599, 126, 45};
        speed_button = {21, 491, 143, 45};
        update_button = {21, 545, 126, 45};
        
        //Input text field setup
        text_string[0] = '\0';
        letter_count = 0;
        frames_counter = 0;
        
        //Animation setup
        current_step = -1;
        speed_multiplier = 1;
        pause_timer = time_between_steps;
        is_playing = false;

        //Code highlight setup
        current_operation = OPERATION::NONE;

        //insert highlight code
        insert_highlight.set_start_pos({window_width, 296});
        insert_highlight.set_code_name("INSERT");

        insert_highlight.add("arr.add(x);");
        insert_highlight.add("id = arr.size() - 1;");
        insert_highlight.add("while (id > 0 && arr[parent(id)] < arr[id]) {");
        insert_highlight.add("  swap(arr[parent(id)], arr[id]);");
        insert_highlight.add("  id = parent(id);");
        insert_highlight.add("}");
        
        //erase highlight code
        erase_highlight.set_start_pos({window_width, 296});
        erase_highlight.set_code_name("ERASE");

        

        //find highlight code
        find_highlight.set_start_pos({window_width, 296});
        find_highlight.set_code_name("FIND");


    }
    
    bool Heap_Canvas::update_node_position(std::vector<Data_Structure::Heap::Node> &array) {
        bool is_moving = false;
        for (auto &cur : array) {
            std::cout << "ANIMATION UPDATING " << cur.val << ": " << cur.current_x << ' ' << cur.current_y << " - " << cur.target_x << ' ' << cur.target_y << std::endl;

            float distance = Vector2Distance((Vector2){cur.current_x, cur.current_y}, (Vector2){cur.target_x, cur.target_y});

            if (speed_multiplier == 5) { //Instant mode is on
                distance = 0.0f;
            }

            if (distance > 1.0f) {
                Vector2 new_pos = Vector2Lerp((Vector2){cur.current_x, cur.current_y}, (Vector2){cur.target_x, cur.target_y}, ani_speed * speed_multiplier);
                std::cout << "NEW POSITION " << cur.val << ": " << cur.current_x << ' ' << cur.current_y << " - " << new_pos.x << ' ' << new_pos.y << std::endl;

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

        if (current_operation == INSERT) {
            insert_highlight.set_highlighted_line(current_tree.index);
        }
        else if (current_operation == ERASE) {
            erase_highlight.set_highlighted_line(current_tree.index);
        }
        else if (current_operation == FIND) {
            find_highlight.set_highlighted_line(current_tree.index);
        }

        std::cout << "====================================================" << std::endl;
        std::cout << "Script " << current_step + 1 << " / " << heap.history.size() 
                  << " | Timer " << pause_timer 
                  << " | Is moving? " << (is_animating ? "Yes" : "No") << std::endl;

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
                    if (current_operation == INSERT) {
                        insert_highlight.set_highlighted_line(-1);
                    }
                    else if (current_operation == ERASE) {
                        erase_highlight.set_highlighted_line(-1);
                    }
                    else if (current_operation == FIND) {
                        find_highlight.set_highlighted_line(-1); 
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
        }
    }

    void Heap_Canvas::insert() {
        std::vector<int> val_to_insert;
        std::string cur_num = "";

        for (int i = 0; i < letter_count; i++) {
            if (text_string[i] != ' ') {
                cur_num.push_back(text_string[i]);
            }
            else if (!cur_num.empty()) {
                val_to_insert.push_back(std::stoi(cur_num));
                cur_num = "";
            }
        }

        if (!cur_num.empty()) {
            val_to_insert.push_back(std::stoi(cur_num));
        }

        letter_count = 0;
        text_string[0] = '\0';

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
    }

    void Heap_Canvas::erase() {

    }

    void Heap_Canvas::next() {

    }

    void Heap_Canvas::prev() {

    }

    void Heap_Canvas::clear() {

    }

    void Heap_Canvas::skip() {

    }

    void Heap_Canvas::update() {

    }

    void Heap_Canvas::find() {

    }

    void Heap_Canvas::open_file() {

    }

    void Heap_Canvas::run() {
        bool mouse_on_input_text_field = CheckCollisionPointRec(GetMousePosition(), input_text_field);

        if (mouse_on_input_text_field && !is_playing) {
            ++frames_counter;
            SetMouseCursor(MOUSE_CURSOR_IBEAM);

            int key = GetCharPressed();

            while (key > 0) {
                if (((((key > '0' || (key == '0' && letter_count > 0 && text_string[letter_count - 1] != ' ')) && key <= '9')) || key == ' ') && letter_count < MAX_INPUT_INT_CHAR) {
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
            clear();
            return;
        }
        else if (!is_playing) { //No animation is running
            if (is_clicked(insert_button) && letter_count > 0) {
                if (current_step != (int)heap.history.size() - 1) {
                    skip();
                }
                insert();
            }
            else if (is_clicked(prev_button) && current_step >= 0) {
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
                        text_string[letter_count++] = c;
                    }
                    text_string[letter_count++] = ' ';
                }
                insert();
            }
            else if (is_clicked(update_button) && letter_count > 0) {
                if (current_step != (int)heap.history.size() - 1) {
                    skip();
                }
                update();
            }
            else if (is_clicked(erase_button) && letter_count > 0) {
                if (current_step != (int)heap.history.size() - 1) {
                    skip();
                }
                erase();
            }
            else if (is_clicked(find_button) && letter_count > 0) {
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
        draw_button(input_text_field, "", WHITE, WHITE);
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

        int text_width = MeasureText(text_string, 20);
        int text_start_x = input_text_field.x + 10;

        if (text_width > input_text_field.width - 2 * 10) {
            text_start_x -= text_width - (input_text_field.width - 2 * 10);
        }

        BeginScissorMode(input_text_field.x, input_text_field.y, input_text_field.width, input_text_field.height);

        DrawText(text_string, text_start_x, input_text_field.y + 13, 20, BLACK);

        if (mouse_on_input_text_field && !is_playing) {
            if (letter_count < MAX_INPUT_INT_CHAR) {
                if (((frames_counter / 20) & 1) == 0) {
                    DrawText("_", text_start_x + text_width + 2, input_text_field.y + 15, 20, BLACK);
                }
            }
        }

        EndScissorMode();

        DrawText("MOUSE WHEEL TO ZOOM IN-OUT", 9, 15, 20, GREEN);
        DrawText("PRESS R TO RESET ZOOM", 9, 45, 20, PURPLE);

        if (mouse_on_input_text_field && !is_playing && letter_count >= MAX_INPUT_INT_CHAR) {
            DrawText("MAXIMUM INPUT REACHED", 198, 607, 23, RED);
        
        }

        //Code highlight drawing
        if (current_operation == INSERT) {
            insert_highlight.draw_code();
        }
        else if (current_operation == ERASE) {
            erase_highlight.draw_code();
        }
        else if (current_operation == FIND) {
            find_highlight.draw_code();
        }

        EndDrawing();
    }

    
}
