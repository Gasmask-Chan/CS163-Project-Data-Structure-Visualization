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
    save_snapshot(0, UI::OPERATION::INSERT);
    if (pHead == nullptr) {
        pHead = new Node(x, id_counter++);
        pTail = pHead;
        pHead->pNext = nullptr;

        pHead->highlighted = true;
        save_snapshot(0, UI::OPERATION::INSERT);
        pHead->highlighted = false;

        save_snapshot(-1, UI::OPERATION::NONE);
        return;
    }

    save_snapshot(1, UI::OPERATION::INSERT);
    if (insert_at_the_end) {
        pTail->pNext = new Node(x, id_counter++);
        pTail = pTail->pNext;

        pTail->highlighted = true;
        save_snapshot(1, UI::OPERATION::INSERT);
        pTail->highlighted = false;
    }
    else {
        save_snapshot(2, UI::OPERATION::INSERT);
        Node *new_node = new Node(x, id_counter++);
        new_node->pNext = pHead;
        pHead = new_node;

        pHead->highlighted = true;
        save_snapshot(0, UI::OPERATION::INSERT);
        pHead->highlighted = false;
    }

    save_snapshot(-1, UI::OPERATION::NONE);
}

Singly_Linked_List::Node* Singly_Linked_List::find(int x) {
    save_snapshot(0, UI::OPERATION::FIND);
    save_snapshot(1, UI::OPERATION::FIND);

    for (Node *cur = pHead; cur != nullptr; cur = cur->pNext) {
        cur->highlighted = true;
        save_snapshot(2, UI::OPERATION::FIND);
        
        if (cur->val == x) {
            return cur;
        }
        
        cur->highlighted = false;
        save_snapshot(3, UI::OPERATION::FIND);
        save_snapshot(1, UI::OPERATION::FIND);
    }

    save_snapshot(5, UI::OPERATION::FIND);
    save_snapshot(-1, UI::OPERATION::FIND);
    return nullptr;
}

void Singly_Linked_List::erase(int x) {
    save_snapshot(0, UI::OPERATION::ERASE);
    if (pHead == nullptr) return;

    save_snapshot(1, UI::OPERATION::ERASE);

    pHead->highlighted = true;
    save_snapshot(2, UI::OPERATION::ERASE);
    save_snapshot(3, UI::OPERATION::ERASE);

    if (pHead->val == x) {
        Node *to_delete = pHead;
        pHead = pHead->pNext;
        delete to_delete;

        if (pHead == nullptr) {
            pTail = nullptr;
        }

        save_snapshot(3, UI::OPERATION::ERASE);
        save_snapshot(-1, UI::OPERATION::NONE);
        return;
    }

    pHead->highlighted = false;

    for (Node *cur = pHead; cur != nullptr; cur = cur->pNext) {
        save_snapshot(2, UI::OPERATION::ERASE);

        if (cur->pNext != nullptr) {
            cur->pNext->highlighted = true;
            save_snapshot(3, UI::OPERATION::ERASE);

            if (cur->pNext->val == x) {
                Node *to_delete = cur->pNext;
                cur->pNext = to_delete->pNext;
                if (to_delete == pTail) {
                    pTail = cur;
                }
                delete to_delete;

                save_snapshot(3, UI::OPERATION::ERASE);

                break;
            }

            cur->pNext->highlighted = false;
            save_snapshot(4, UI::OPERATION::ERASE);
        }
    }

    save_snapshot(-1, UI::OPERATION::NONE);
}

void Singly_Linked_List::clear() {
    for (Node *cur = pHead; cur != nullptr;) {
        Node *to_delete = cur;
        cur = cur->pNext;
        delete to_delete;
    }
    pHead = pTail = nullptr;
    
    for (auto &snap : history) {
        for (Node *cur = snap.pHead; cur != nullptr;) {
            Node *to_delete = cur;
            cur = cur->pNext;
            delete to_delete;
        }
    }

    history.clear();
}

//===========================UI===========================

Singly_Linked_List::Node* Singly_Linked_List::clone_node(Node* cur, Node* &pTail) {
    if (!cur) return nullptr;

    Node* new_node = new Node(cur->val, cur->id);

    new_node->highlighted = cur->highlighted;
    new_node->current_x = cur->current_x;
    new_node->current_y = cur->current_y;
    new_node->target_x = cur->target_x;
    new_node->target_y = cur->target_y;
    
    new_node->pNext = clone_node(cur->pNext, pTail);

    if (cur->pNext == nullptr) {
        pTail = new_node;
    }

    return new_node;
}

void Singly_Linked_List::get_copy(Node* &pHead, Node* &pTail) {
    pHead = pTail = nullptr;
    pHead = clone_node(this->pHead, pTail);
}

void Singly_Linked_List::save_snapshot(int index, UI::OPERATION op) {
    recalculate_position();
    Node *pHead, *pTail;
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