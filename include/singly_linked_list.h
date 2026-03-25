#ifndef SINGLY_LINKED_LIST_H
#define SINGLY_LINKED_LIST_H

#include "UI_config.h"

#include <vector>

namespace Data_Structure {

class Singly_Linked_List {
public:
    struct Node {
        int val, id;
        bool highlighted;
        float current_x, current_y;
        float target_x, target_y;
        Node* pNext;
        
        /**
         * @brief Construct a new `Node` object
         * 
         * @param val 
         */
        Node(int val = -1, int id = -1);
    };

private:
    int id_counter;
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
     * @brief Insert new value `x` to the linked list
     * 
     * This value will be inserted to the end of the linked list by default 
     * 
     * @param x 
     * @param insert_at_the_end 
     */
   void insert(int x, bool insert_at_the_end = true);

    /**
     * @brief Return the node which contains the value of `x`
     * 
     * If no such node exists, a `nullptr` will be returned
     * 
     * @param x 
     * @return Node* 
     */
    Node* find(int x);

    /**
     * @brief Delete the node which contains the value of `x`
     * 
     * If no such node exists, this function will do nothing
     * 
     * @param x 
     */
    void erase(int x);

    /**
     * @brief Clear the entire linked list
     * 
     * @return void 
     */
    void clear();

    //========================UI========================
    struct Snapshot_Data {
        Node *pHead, *pTail;
        int index; //Current code line
        UI::OPERATION op;
    };

    const Vector2 start_pos = {168, 150};
    const float x_distance = 87.0f;

    std::vector<Snapshot_Data> history;

    Node* clone_node(Node* cur, Node* &pTail);
    
    /**
     * @brief Return the Head and the Tail of a new copy of the current Linked List
     *
     * @param pHead
     * @param pTail
     *
     */
    void get_copy(Node* &pHead, Node* &pTail);
    

    /**
     * @brief Save snapshot of the current Linked List to `history` vector
     * 
     * @param index
     * @param op
     */
    void save_snapshot(int index, UI::OPERATION op);

    /**
     * @brief Recalculate postion of each node in the Linked List
     * 
     */
    void recalculate_position();
};
}

#endif