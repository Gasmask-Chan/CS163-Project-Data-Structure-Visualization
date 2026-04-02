#include "../include/trie.h"

using namespace Data_Structure;

Trie::Node::Node() {
    cnt = exist = 0;
    current_x = current_y = target_x = target_y = 0;
    tree_width = 0;
    highlighted = false;
    for (int i = 0; i < 26; i++) {
        child[i] = nullptr;
    }
}

Trie::Trie() {
    root = new Node();
}

Trie::~Trie() {
    clear();
}

void Trie::insert(std::string str) {
    save_snapshot(0, UI::OPERATION::INSERT);
    if (raw_find(str)) return; //Only store unique string

    Node *cur = root;
    save_snapshot(1, UI::OPERATION::INSERT);
    save_snapshot(2, UI::OPERATION::INSERT);
    
    for (char &tmp : str) {
        int c = tmp - 'A';
        save_snapshot(3, UI::OPERATION::INSERT);
        if (cur->child[c] == nullptr) {
            cur->child[c] = new Node();
            save_snapshot(4, UI::OPERATION::INSERT);
        }

        cur->highlighted = false;
        cur = cur->child[c];
        cur->highlighted = true;
        save_snapshot(5, UI::OPERATION::INSERT);

        ++cur->cnt;
        save_snapshot(2, UI::OPERATION::INSERT);
    }

    ++cur->exist;
    cur->highlighted = false;
    save_snapshot(-1, UI::OPERATION::NONE);
}

Trie::Node* Trie::raw_find(std::string str) {
    if (!root) return nullptr;

    Node *cur = root;
    for (char &tmp : str) {
        int c = tmp - 'A';
        if (cur->child[c] == nullptr) { //No such string exists
            return nullptr;
        }

        cur = cur->child[c];
    }

    if (cur->exist == 0) { //No string ends here
        return nullptr;
    }

    return cur;
}

Trie::Node* Trie::find(std::string str) {
    if (!root) return nullptr;

    Node *cur = root;
    for (char &tmp : str) {
        int c = tmp - 'A';
        if (cur->child[c] == nullptr) { //No such string exists
            return nullptr;
        }

        cur = cur->child[c];
    }

    if (cur->exist == 0) { //No string ends here
        return nullptr;
    }

    return cur;
}

bool Trie::erase(Node *cur, int pos, std::string &str) {
    cur->highlighted = true;
    save_snapshot(1, UI::OPERATION::ERASE);
    
    if (pos < (int)str.size()) {
        int c = str[pos] - 'A';
        save_snapshot(2, UI::OPERATION::ERASE);
        bool res = erase(cur->child[c], pos + 1, str);
        if (res) {
            cur->child[c] = nullptr;
        }
    }
    else {
        --cur->exist;
    }

    save_snapshot(3, UI::OPERATION::ERASE);
    
    if (cur != root) {
        --cur->cnt;
        if (cur->cnt == 0) {
            delete(cur);
            return true;
        }
    }
    
    cur->highlighted = false;
    return false;
}

bool Trie::erase(std::string str) {
    save_snapshot(0, UI::OPERATION::ERASE);
    if (raw_find(str)) {
        erase(root, 0, str);
        save_snapshot(-1, UI::OPERATION::NONE);
        return true;
    }
    return false;
}

void Trie::clear(Node *cur) {
    if (!cur) return;
    for (int i = 0; i < 26; i++) {
        if (cur->child[i]) {
            clear(cur->child[i]);
            cur->child[i] = nullptr;
        }
    }

    delete(cur);
}

void Trie::clear() {
    for (int i = 0; i < 26; i++) {
        clear(root->child[i]);
        root->child[i] = nullptr;
    }

    for (auto &snap : history) {
        clear(snap.tree_root);
        snap.tree_root = nullptr;
    }

    history.clear();
}

bool Trie::check_valid_input(std::string str) {
    for (char &c : str) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

//===========================UI===========================

Trie::Node* Trie::get_copy(Node *cur) {
    if (!cur) return nullptr;

    Node* new_node = new Node();
    new_node->cnt = cur->cnt;
    new_node->exist = cur->exist;
    new_node->current_x = cur->current_x;
    new_node->current_y = cur->current_y;
    new_node->target_x = cur->target_x;
    new_node->target_y = cur->target_y;
    new_node->highlighted = cur->highlighted;
    new_node->tree_width = cur->tree_width;
    
    for (int i = 0; i < 26; i++) {
        new_node->child[i] = get_copy(cur->child[i]);
    }

    return new_node;
}

Trie::Node* Trie::get_copy() {
    return get_copy(root);
}

void Trie::cal_initial_gap(Node* cur) {
    if (!cur) return;
    bool first_child = true;
    bool is_leaf = true;

    cur->tree_width = 0.0f;

    for (int i = 0; i < 26; i++) {
        if (cur->child[i]) {
            is_leaf = false;

            if (first_child) {
                first_child = false;
            }
            else {
                cur->tree_width += horizontal_gap;
            }

            cal_initial_gap(cur->child[i]);
            cur->tree_width += cur->child[i]->tree_width;
        }
    }

    if (is_leaf) {
        cur->tree_width = UI::node_radius * 2;
    }
}


void Trie::save_snapshot(int index, UI::OPERATION op) {
    recalculate_position();
    history.push_back({get_copy(), index, op});
}

void Trie::recalculate_position(Node* cur, float x, float y) {
    if (!cur) return;
    
    cur->target_x = x + (cur->tree_width / 2.0);
    cur->target_y = y;

    for (int i = 0; i < 26; i++) {
        if (cur->child[i]) {
            recalculate_position(cur->child[i], x, y + vertical_gap);
            x += cur->child[i]->tree_width + horizontal_gap;
        }
    }
}

void Trie::recalculate_position() {
    if (!root) return;
    cal_initial_gap(root);
    recalculate_position(root, (UI::window_width / 2.0) - (root->tree_width / 2.0), 38);
}



