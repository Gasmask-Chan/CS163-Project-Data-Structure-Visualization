#ifndef AVL_TREE
#define AVL_TREE

namespace Data_Structure {

class AVL_Tree {
private:
    struct Node {
        int val;
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
     * @brief Insert new value `x` to the AVL tree
     * 
     * @param x 
     * @return void 
     */
    void insert(int x);

    /**
     * @brief Return the node which contains the value `x`
     * 
     * If no such node exists, a `nullptr` will be returned
     * 
     * @param x 
     * @return Node* 
     */
    Node* find(int x);

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
     * @return void
     */
    void clear();
};
};

#endif