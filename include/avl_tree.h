#ifndef AVL_TREE
#define AVL_TREE

namespace Data_Structure {

class AVL_Tree {
private:
    struct Node {
        int val, height;
        Node *left, *right;

        /**
         * @brief Construct a new `Node` object
         * 
         * @param val 
         */
        Node(int val = -1);
    };

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
     * @return Node* 
     */
    Node* insert(Node *cur, int x);

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
};
}

#endif