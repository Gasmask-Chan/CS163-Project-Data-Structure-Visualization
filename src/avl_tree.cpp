#include "../include/avl_tree.h"

#include <algorithm>

using namespace Data_Structure;

AVL_Tree::Node::Node(int val) {
    this->val = val;
    height = 1;
    current_x = current_y = target_x = target_y = 0;
    highlighted = false;
    left = right = nullptr;
}

AVL_Tree::AVL_Tree() {
    root = nullptr;
}

AVL_Tree::~AVL_Tree() {
    clear();
}

int AVL_Tree::get_height(Node *cur) {
    if (cur == nullptr) {
        return 0;
    }

    return cur->height;
}

int AVL_Tree::get_balance_factor(Node *cur) {
    if (cur == nullptr) {
        return 0;
    }

    return get_height(cur->left) - get_height(cur->right);
}

AVL_Tree::Node* AVL_Tree::left_rotate(Node *cur) {
    Node *x = cur->right;
    Node *y = x->left;

    x->left = cur;
    cur->right = y;

    cur->height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
    x->height = std::max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

AVL_Tree::Node* AVL_Tree::right_rotate(Node *cur) {
    Node *x = cur->left;
    Node *y = x->right;

    x->right = cur;
    cur->left = y;

    cur->height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
    x->height = std::max(get_height(x->left), get_height(x->right)) + 1;

    return x;
}

AVL_Tree::Node* AVL_Tree::insert(Node *cur, int x, Node *parent) {
    if (cur == nullptr) {
        Node* new_node = new Node(x);
        new_node->highlighted = true;
        return new_node;
    }

    cur->highlighted = true;
    history.push_back(get_copy(root));

    if (cur->val > x) {
        bool flag = cur->left == nullptr;
        cur->left = insert(cur->left, x, cur);
        cur->height = std::max(get_height(cur->left), get_height(cur->right)) + 1;

        if (flag) {
            history.push_back(get_copy());
            cur->left->highlighted = false;
        }
    }
    else if (cur->val < x) {
        bool flag = cur->right == nullptr;
        cur->right = insert(cur->right, x, cur);
        cur->height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
        
        if (flag) {
            history.push_back(get_copy());
            cur->right->highlighted = false;
        }
    }
    else {
        cur->highlighted = false;
        history.push_back(get_copy());
        return cur;
    }

    history.push_back(get_copy());
    cur->height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
    
    cur->highlighted = false;
    
    int balance = get_balance_factor(cur);
    
    bool rotated = abs(balance) <= 1;
    
    if (balance > 1) {
        if (x < cur->left->val) { //LL
            cur = right_rotate(cur);
        }
        else if (x > cur->left->val) { //LR
            cur->left = left_rotate(cur->left);
            cur = right_rotate(cur);
        }
    }
    else if (balance < -1) {
        if (x > cur->right->val) { //RR
            cur = left_rotate(cur);
        }
        else if (x < cur->right->val) { //RL
            cur->right = right_rotate(cur->right);
            cur = left_rotate(cur);
        }
    }
    
    if (rotated) {
        history.push_back(get_copy());
    }

    return cur;
}

void AVL_Tree::insert(int x) {
    if (!root) {
        root = new Node(x);
        root->highlighted = true;
        history.push_back(get_copy()); 
        root->highlighted = false;
        history.push_back(get_copy());
    }
    else {
        root = insert(root, x, root);
        root->highlighted = false;
        history.push_back(get_copy());
    }
}

AVL_Tree::Node* AVL_Tree::find(Node *cur, int x) {
    if (cur == nullptr) {
        return nullptr;
    }

    if (cur->val == x) {
        return cur;
    }
    else if (cur->val > x) {
        return find(cur->left, x);
    }
    else {
        return find(cur->right, x);
    }
}

AVL_Tree::Node* AVL_Tree::find(int x) {
    return find(root, x);
}

AVL_Tree::Node* AVL_Tree::erase(Node *cur, int x) {
    if (cur == nullptr) {
        return nullptr;
    }

    if (cur->val == x) {
        if (cur->left && cur->right) {
            Node* replacement = cur->right;
            while (replacement->left) {
                replacement = replacement->left;
            }

            cur->val = replacement->val;
            cur->right = erase(cur->right, replacement->val);
        }
        else {
            Node *replacement = cur;
            cur = cur->left ? cur->left : cur->right;
            delete replacement;
        }
    }
    else if (cur->val > x) {
        cur->left = erase(cur->left, x);
    }
    else {
        cur->right = erase(cur->right, x);
    }

    if (cur == nullptr) {
        return nullptr;
    }

    cur->height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
    int balance = get_balance_factor(cur);

    if (balance > 1) {
        if (get_balance_factor(cur->left) >= 0) {
            cur = right_rotate(cur);
        }
        else if (get_balance_factor(cur->left) < 0) {
            cur->left = left_rotate(cur->left);
            cur = right_rotate(cur);
        }
    }
    else if (balance < -1) {
        if (get_balance_factor(cur->right) <= 0) {
            cur = left_rotate(cur); 
        }
        else if (get_balance_factor(cur->right) > 0) {
            cur->right = right_rotate(cur->right);
            cur = left_rotate(cur);
        }
    }

    return cur;
}

void AVL_Tree::erase(int x) {
    root = erase(root, x);
    recalculate_position();
}

void AVL_Tree::clear(Node *cur) {
    if (cur == nullptr) return;
    clear(cur->left);
    clear(cur->right);
    cur->left = cur->right = nullptr;
    delete cur;
}

void AVL_Tree::clear() {
    clear(root);
    root = nullptr;

    for (Node* &node : history) {
        clear(node);
        node = nullptr;
    }

    history.clear();
}


//===========================UI===========================

AVL_Tree::Node* AVL_Tree::get_copy(Node* cur) {
    if (!cur) return nullptr;

    Node* new_node = new Node(cur->val);
    new_node->height = cur->height;
    new_node->current_x = cur->current_x;
    new_node->current_y = cur->current_y;
    new_node->target_x = cur->target_x;
    new_node->target_y = cur->target_y;
    new_node->highlighted = cur->highlighted;

    new_node->left = get_copy(cur->left);
    new_node->right = get_copy(cur->right);

    return new_node;
}

AVL_Tree::Node* AVL_Tree::get_copy() {
    recalculate_position();
    return get_copy(root);
}

AVL_Tree::Node* AVL_Tree::get_root() {
    return root;
}

int AVL_Tree::get_tree_height(Node* cur) {
    if (!cur) return 0;
    return std::max(get_tree_height(cur->left), get_tree_height(cur->right)) + 1;
}

int AVL_Tree::get_initial_gap() {
    if (!root) return 0;
    return (1 << (get_tree_height(root) - 1)) * 25;
}

void AVL_Tree::recalculate_position(Node* cur, float x, float y, int gap) {
    if (!cur) return;

    cur->target_x = x;
    cur->target_y = y;
    
    recalculate_position(cur->left, x - gap, y + vertical_gap, gap >> 1);
    recalculate_position(cur->right, x + gap, y + vertical_gap, gap >> 1);
}

void AVL_Tree::recalculate_position() {
    recalculate_position(root, UI::window_width >> 1, 38, get_initial_gap());
}