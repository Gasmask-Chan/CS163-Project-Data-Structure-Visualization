#ifndef HEAP_H
#define HEAP_H

#include <vector>

namespace Data_Structure {
class Heap { //Max heap
private:
    std::vector<int> arr;

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
};
}

#endif