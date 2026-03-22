#include "../include/heap.h"

#include <assert.h>

using namespace Data_Structure;

Heap::Node::Node(int val, int id) {
    this->val = val;
    this->id = id;
    current_x = current_y = target_x = target_y = 0.0f;
    highlighted = false;
}

Heap::Heap() {
    id_counter = 0;
}

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

    if (left < size() && arr[left].val > arr[best].val) {
        best = left;
    }

    if (right < size() && arr[right].val > arr[best].val) {
        best = right;
    }

    if (best != id) {
        std::swap(arr[id], arr[best]);
        heapify(best);
    }
}

void Heap::insert(int x) {
    save_snapshot(0, UI::OPERATION::INSERT);
    arr.push_back(Node(x, ++id_counter));
    arr.back().highlighted = true;
    save_snapshot(0, UI::OPERATION::INSERT);
    int id = size() - 1;
    save_snapshot(1, UI::OPERATION::INSERT);

    save_snapshot(2, UI::OPERATION::INSERT);
    while (id > 0 && arr[(id - 1) >> 1].val < arr[id].val) {
        arr[(id - 1) >> 1].highlighted = true;
        save_snapshot(3, UI::OPERATION::INSERT);
        std::swap(arr[(id - 1) >> 1], arr[id]);
        save_snapshot(3, UI::OPERATION::INSERT);
        arr[id].highlighted = false;
        id = (id - 1) >> 1;
        save_snapshot(4, UI::OPERATION::INSERT);
        save_snapshot(2, UI::OPERATION::INSERT);
    }

    arr[id].highlighted = false;
    save_snapshot(-1, UI::OPERATION::NONE);
}

int Heap::top() {
    assert(size() > 0);
    return arr[0].val;
}

void Heap::pop() {
    assert(size() > 0);

    std::swap(arr[0], arr[size() - 1]);
    arr.pop_back();
    heapify(0);
}

void Heap::clear() {
    arr.clear();
    history.clear();
}

//===========================UI===========================

std::vector<Heap::Node> Heap::get_copy() {
    recalculate_position();
    return arr;
}

int Heap::get_tree_height() {
    if (size() == 0) return 0;
    return std::__lg(size());
}

void Heap::save_snapshot(int index, UI::OPERATION op) {
    history.push_back({get_copy(), index, op});
}

int Heap::get_initial_gap() {
    if (size() == 0) return 0;
    return (1 << (get_tree_height())) * 25;
}

void Heap::recalculate_position(int id, float x, float y, int gap) {
    if (id >= size()) return;

    arr[id].target_x = x;
    arr[id].target_y = y;
    
    recalculate_position((id << 1) + 1, x - gap, y + vertical_gap, gap >> 1);
    recalculate_position((id << 1) + 2, x + gap, y + vertical_gap, gap >> 1);
}

void Heap::recalculate_position() {
    std::cout << "TREE HEIGHT " << get_tree_height() << " - " << size() << std::endl;
    recalculate_position(0, UI::window_width >> 1, 38, get_initial_gap());
}
