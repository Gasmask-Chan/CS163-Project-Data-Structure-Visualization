#include "../include/avl_tree.h"

#include <algorithm>

using namespace Data_Structure;

AVL_Tree::Node::Node(int val) {
    this->val = val;
    height = 1;
    current_x = current_y = target_x = target_y = 0;
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

AVL_Tree::Node* AVL_Tree::insert(Node *cur, int x) {
    if (cur == nullptr) {
        return new Node(x);
    }

    if (cur->val > x) {
        cur->left = insert(cur->left, x);
    }
    else if (cur->val < x) {
        cur->right = insert(cur->right, x);
    }
    else {
        return cur;
    }

    cur->height = std::max(get_height(cur->left), get_height(cur->right)) + 1;
    int balance = get_balance_factor(cur);

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

    return cur;
}

void AVL_Tree::insert(int x) {
    root = insert(root, x);
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
}