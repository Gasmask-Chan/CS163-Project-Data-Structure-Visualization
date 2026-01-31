#include "../include/singly_linked_list.h"

using namespace Data_Structure;

Singly_Linked_List::Singly_Linked_List() {
    pHead = pTail = nullptr;
}

Singly_Linked_List::~Singly_Linked_List() {
    for (Node *cur = pHead; cur != nullptr;) {
        Node *to_delete = cur;
        cur = cur->pNext;
        delete to_delete;
    }
    pHead = pTail = nullptr;
}

void Singly_Linked_List::insert(int x, bool insert_at_the_end) {
    if (pHead == nullptr) {
        pHead = new Node(x);
        pTail = pHead;
        pHead->pNext = pTail;
        return;
    }

    if (insert_at_the_end) {
        pTail->pNext = new Node(x);
        pTail = pTail->pNext;
    }
    else {
        Node *new_node = new Node(x);
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