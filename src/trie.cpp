#include "../include/trie.h"

using namespace Data_Structure;

Trie::Node::Node() {
    cnt = 0;
    for (int i = 0; i < 26; i++) {
        child[i] = nullptr;
    }
}

Trie::Trie() {
    root = new Node();
}

Trie::~Trie() {
    clear();
}

void Trie::insert(std::string str) {
    Node *cur = root;
    for (char &c : str) {
        if (cur->child[c] == nullptr) {
            cur->child[c] = new Node();
        }

        cur = cur->child[c];
    }

    cur->cnt++;
}

Trie::Node* Trie::find(std::string str) {
    Node *cur = root;
    for (char &c : str) {
        if (cur->child[c] == nullptr) { //No such string exists
            return nullptr;
        }

        cur = cur->child[c];
    }

    if (cur->cnt == 0) { //No string ends here
        return nullptr;
    }

    return cur;
}

bool Trie::erase(Node *cur, int pos, std::string &str) {
    if (pos == (int)str.size() - 1) {
        
    }
}

