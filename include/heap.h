#ifndef HEAP_H
#define HEAP_H

#include "UI_config.h"
#include "avl_tree.h"

#include <vector>

namespace Data_Structure {
class Heap { //Max heap
public:
    struct Node {
        int val, id;
        float current_x, current_y;
        float target_x, target_y;
        bool highlighted;

        /**
         * @brief Construct a new `Node` object
         * 
         * @param val 
         */
        Node(int val = -1, int id = -1);
    };

private:
    int id_counter;
    std::vector<Node> arr;

public:
    /**
     * @brief Construct a new Heap object
     * 
     */
    Heap();

    /**
     * @brief Destroy the Heap object
     * 
     */
    ~Heap();

    /**
     * @brief Return the current size of the heap
     * 
     * @return int 
     */
    int size();

    /**
     * @brief Heapify a subtree rooted at id
     * 
     * @param id 
     * 
     * @return void
     */
    void heapify(int id);

    /**
     * @brief Insert new value `x` to the heap
     * 
     * @param x 
     * @return void 
     */
    void insert(int x);

    /**
     * @brief Return the value of the max heap
     * 
     * If the heap is empty, the program will be
     * 
     * @return int 
     */
    int top();

    /**
     * @brief Remove the value at the top of the heap
     * 
     * If the heap is empty, the program will be asserted
     *
     * @return void
     */
    void pop();

    /**
     * @brief Clear the entire heap
     * 
     * @return void
     */
    void clear();

    /**
     * @brief Change the value at the id-th index to new_val
     * 
     * @param id 
     * @param new_val 
     */
    void update(int id, int new_val);

    /**
     * @brief Return the index of the element that has the value equal to `val`
     * 
     * If no such index exists, the function will return `-1`
     * 
     * @param val 
     * @return int 
     */
    int find(int val);

    //========================UI========================
    struct Snapshot_Data {
        std::vector<Node> array;
        int index; //Current code line
        UI::OPERATION op;
    };

    const int vertical_gap = 80;

    std::vector<Snapshot_Data> history;

    /**
     * @brief Return a copy of the current arr
     * 
     * @return std::vector<Node>
     */
    std::vector<Node> get_copy();

    /**
     * @brief Get the current height of the tree
     * 
     * @return int 
     */
    int get_tree_height();

    /**
     * @brief Save snapshot of the current tree to `history` vector
     * 
     * @param index
     * @param op
     */
    void save_snapshot(int index, UI::OPERATION op);

    /**
     * @brief Return the maximum horizontal gap we need based on the tree's height
     * 
     * @return int 
     */
    int get_initial_gap();

    void recalculate_position(int id, float x, float y, int gap);

    /**
     * @brief Recalculate postion of each node in tree
     * 
     */
    void recalculate_position();
};
}

#endif