#include "../include/singly_linked_list.h"

using namespace Data_Structure;

Singly_Linked_List::Node::Node(int val, int id) {
    this->val = val;
    this->id = id;
    highlighted = false;
    current_x = current_y = target_x = target_y = 0;
    pNext = nullptr;
}

Singly_Linked_List::Singly_Linked_List() {
    id_counter = 0;
    pHead = pTail = nullptr;
}

Singly_Linked_List::~Singly_Linked_List() {
    clear();
}

void Singly_Linked_List::insert(int x, bool insert_at_the_end) {
    if (pHead == nullptr) {
        pHead = new Node(x, id_counter++);
        pTail = pHead;
        pHead->pNext = pTail;
        return;
    }

    if (insert_at_the_end) {
        pTail->pNext = new Node(x, id_counter++);
        pTail = pTail->pNext;
    }
    else {
        Node *new_node = new Node(x, id_counter++);
        new_node->pNext = pHead->pNext;
        pHead = new_node;
    }
}

Singly_Linked_List::Node* Singly_Linked_List::find(int x) {
    for (Node *cur = pHead; cur != nullptr; cur = cur->pNext) {
        if (cur->val == x) {
            return cur;
        }
    }
    return nullptr;
}

void Singly_Linked_List::erase(int x) {
    if (pHead != nullptr && pHead->val == x) {
        Node *to_delete = pHead;
        pHead = pHead->pNext;
        delete to_delete;
        return;
    }

    for (Node *cur = pHead; cur != nullptr; cur = cur->pNext) {
        if (cur->pNext != nullptr && cur->pNext->val == x) {
            Node *to_delete = cur->pNext;
            cur->pNext = to_delete->pNext;
            if (to_delete == pTail) {
                pTail = cur;
            }
            delete to_delete;
        }
    }
}

void Singly_Linked_List::clear() {
    for (Node *cur = pHead; cur != nullptr;) {
        Node *to_delete = cur;
        cur = cur->pNext;
        delete to_delete;
    }
    pHead = pTail = nullptr;
}

//===========================UI===========================

Singly_Linked_List::Node* Singly_Linked_List::get_copy(Node* cur, Node* &pHead, Node* &pTail) {
    if (!cur) return;

    Node* new_node = new Node(cur->val, cur->id);

    new_node->highlighted = cur->highlighted;
    new_node->current_x = cur->current_x;
    new_node->current_y = cur->current_y;
    new_node->target_x = cur->target_x;
    new_node->target_y = cur->target_y;

    
    new_node->pNext = get_copy(cur->pNext, pHead, pTail);
    
    if (pHead == nullptr) {
        pHead = new_node;
    }

    if (cur->pNext == nullptr) {
        pTail = new_node;
    }

    return new_node;
}

void Singly_Linked_List::get_copy(Node* &pHead, Node* &pTail) {
    pHead = pTail = nullptr;
    get_copy(this->pHead, pHead, pTail);
}

void Singly_Linked_List::save_snapshot(int index, UI::OPERATION op) {
    recalculate_position();
    Node* pHead, *pTail;
    get_copy(pHead, pTail);
    history.push_back({pHead, pTail, index, op});
}

void Singly_Linked_List::recalculate_position() {
    Node* cur = this->pHead;
    int cnt = 0;
    while (cur != nullptr) {
        cur->target_x = start_pos.x + cnt * x_distance;
        cur->target_y = start_pos.y;

        ++cnt;
        cur = cur->pNext;
    }
}