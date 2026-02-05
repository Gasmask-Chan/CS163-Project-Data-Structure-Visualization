#include "../include/avl_tree.h"

using namespace Data_Structure;

AVL_Tree::Node::Node(int val) {
    this->val = val;
    left = right = nullptr;
}

AVL_Tree::AVL_Tree() {
    root = nullptr;
}

AVL_Tree::~AVL_Tree() {
    clear();
}

void AVL_Tree::insert(int x) {

}