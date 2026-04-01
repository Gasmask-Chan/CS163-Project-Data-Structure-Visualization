#include "../include/trie.h"

using namespace Data_Structure;

Trie::Node::Node() {
    cnt = exist = 0;
    current_x = current_y = target_x = target_y = 0;
    tree_width = 0;
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
    Node *cur = root;
    for (char &tmp : str) {
        int c = tmp - 'a';
        if (cur->child[c] == nullptr) {
            cur->child[c] = new Node();
        }

        cur = cur->child[c];
        ++cur->cnt;
    }

    ++cur->exist;
}

Trie::Node* Trie::find(std::string str) {
    Node *cur = root;
    for (char &tmp : str) {
        int c = tmp - 'a';
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
    if (pos < (int)str.size()) {
        int c = str[pos] - 'a';
        bool res = erase(cur->child[c], pos + 1, str);
        if (res) {
            cur->child[c] = nullptr;
        }
    }
    else {
        --cur->exist;
    }

    if (cur != root) {
        --cur->cnt;
        if (cur->cnt == 0) {
            delete(cur);
            return true;
        }
    }

    return false;
}

void Trie::erase(std::string str) {
    if (find(str)) {
        erase(root, 0, str);
    }
}

void Trie::clear(Node *cur) {
    for (int i = 0; i < 26; i++) {
        if (cur->child[i]) {
            clear(cur->child[i]);
            cur->child[i] = nullptr;
        }
    }

    delete(cur);
}

void Trie::clear() {
    clear(root);
    root = nullptr;
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
        cur->tree_width = UI::node_radius;
    }
}


void Trie::save_snapshot(int index, UI::OPERATION op) {
    recalculate_position();
    history.push_back({get_copy(), index, op});
}

void Trie::recalculate_position(Node* cur, float x, float y) {
    if (!cur) return;
    
    cur->target_x = x;
    cur->target_y = y;

    float width = cur->tree_width;

    for (int i = 0; i < 26; i++) {
        if (cur->child[i]) {
            recalculate_position(cur->child[i], x - (width / 2), y + vertical_gap);
            width -= cur->child[i]->tree_width;
        }
    }
}

void Trie::recalculate_position() {
    cal_initial_gap(root);
    recalculate_position(root, UI::window_width / 2.0, 38);
}



