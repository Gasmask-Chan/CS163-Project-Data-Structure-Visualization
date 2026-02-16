#include "../include/heap.h"

#include <assert.h>

using namespace Data_Structure;

Heap::Heap() {}

Heap::~Heap() {
    clear();
}

int Heap::size() {
    return static_cast<int>(arr.size());
}

void Heap::heapify(int id) {
    int best = id;
    int left = (id << 1) + 1;
    int right = (id << 1) + 2;

    if (left < size() && arr[left] > arr[best]) {
        best = left;
    }

    if (right < size() && arr[right] > arr[best]) {
        best = right;
    }

    if (best != id) {
        std::swap(arr[id], arr[best]);
        heapify(best);
    }
}

void Heap::insert(int x) {
    arr.push_back(x);
    int id = size() - 1;

    while (id > 0 && arr[(id - 1) >> 1] < arr[id]) {
        std::swap(arr[(id - 1) >> 1], arr[id]);
        id = (id - 1) >> 1;
    }
}

int Heap::top() {
    assert(size() > 0);
    return arr[0];
}

void Heap::pop() {
    assert(size() > 0);

    std::swap(arr[0], arr[size() - 1]);
    arr.pop_back();
    heapify(0);
}

void Heap::clear() {
    arr.clear();
}