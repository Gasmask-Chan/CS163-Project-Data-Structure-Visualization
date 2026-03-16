#ifndef AVL_TREE_H
#define AVL_TREE_H

#include "UI_config.h"

#include <vector>

namespace Data_Structure {

class AVL_Tree {
public:
    struct Node {
        int val, height;
        float current_x, current_y;
        float target_x, target_y;
        bool highlighted;

        Node *left, *right;

        /**
         * @brief Construct a new `Node` object
         * 
         * @param val 
         */
        Node(int val = -1);
    };

private:
    Node *root;

public:
    /**
     * @brief Construct a new `AVL_Tree` object
     * 
     */
    AVL_Tree();

    /**
     * @brief Destroy the `AVL_Tree` object
     * 
     */
    ~AVL_Tree();

    /**
     * @brief Return the height of the given node
     * 
     * @param cur 
     * @return int 
     */
    int get_height(Node *cur);

    /**
     * @brief Return the balance factor of the given node
     * 
     * @param cur 
     * @return int 
     */
    int get_balance_factor(Node *cur);

    /**
     * @brief Perform a left rotation on the given node
     * 
     * @param cur 
     * @return Node* 
     */
    Node* left_rotate(Node *cur);

    /**
     * @brief Perform a right rotation on the given node
     * 
     * @param cur 
     * @return Node* 
     */
    Node* right_rotate(Node *cur);

    /**
     * @brief Insert new value `x` to the AVL tree 
     * 
     * @param cur 
     * @param x 
     * @param parent
     * @return Node* 
     */
    Node* insert(Node *cur, int x, Node *parent);

    /**
     * @brief Insert new value `x` to the AVL tree
     * 
     * @param x 
     * @return void 
     */
    void insert(int x);

    /**
     * @brief Return the node which contains the value `x`
     * 
     * @param cur 
     * @param x 
     * @return Node* 
     */
    Node* find(Node *cur, int x);

    /**
     * @brief Return the node which contains the value `x`
     * 
     * If no such node exists, a `nullptr` will be returned
     * 
     * @param x 
     * @return Node* 
     */
    Node* find(int x);

    Node* erase(Node *cur, int x);

    /**
     * @brief Delete the node which contains the value `x`
     * 
     * If no such node exists, this function will do nothing
     * 
     * @param x 
     * @return void
     */
    void erase(int x);

    /**
     * @brief Clear the entire AVL tree
     * 
     * @param cur 
     */
    void clear(Node *cur);

    /**
     * @brief Clear the entire AVL tree
     * 
     * @return void
     */
    void clear();

    //========================UI========================
    const int vertical_gap = 80;

    std::vector<Node*> history;

    Node* get_copy(Node* cur);

    /**
     * @brief Return the root of a new copy of the current tree
     * 
     * @return Node* 
     */
    Node* get_copy();

    /**
     * @brief Return a `std::vector<Node*>` contains tree nodes in order.
     * 
     * @return std::vector<Node*> 
     */
    Node* get_root();

    /**
     * @brief Get the current height of the tree
     * 
     * @param cur 
     * @return int 
     */
    int get_tree_height(Node* cur);

    /**
     * @brief Return the maximum horizontal gap we need based on the tree's height
     * 
     * @return int 
     */
    int get_initial_gap();

    void recalculate_position(Node* cur, float x, float y, int gap);

    /**
     * @brief Recalculate postion of each node in tree
     * 
     */
    void recalculate_position();
};
}

#endif