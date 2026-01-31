#ifndef SINGLY_LINKED_LIST
#define SINGLY_LINKED_LIST

namespace Data_Structure {

class Singly_Linked_List {
private:
    struct Node {
        int val;
        Node* pNext;
        
        /**
         * @brief Construct a new `Node` object
         * 
         * @param val 
         */
        Node(int val = -1) {
            this->val = val;
            pNext = nullptr;
        }
    };

    Node *pHead, *pTail;

public:  
    /**
     * @brief Construct a new `Singly_Linked_List` object
     * 
     */
    Singly_Linked_List();

    /**
     * @brief Destroy the `Singly_Linked_List` object
     * 
     */
    ~Singly_Linked_List();
   
    /**
     * @brief Insert new value `x` to the linked list. this value will be insert to the end of the linked list by default 
     * 
     * @param x 
     * @param insert_at_the_end 
     */
   void insert(int x, bool insert_at_the_end = true);

    /**
     * @brief Return the node which contains the value of `x`. If no such node exists, the function will return `nullptr`
     * 
     * @param x 
     * @return Node* 
     */
    Node* find(int x);

    /**
     * @brief Delete the node which contains the value of `x`. if no such node exists, the function will do nothing
     * 
     * @param x 
     */
    void erase(int x);
};

}

#endif