#include "../include/trie.h"

using namespace Data_Structure;

Trie::Node::Node() {
    cnt = exist = 0;
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
    for (char &tmp : str) {
        int c = tmp - 'a';
        if (cur->child[c] == nullptr) {
            cur->child[c] = new Node();
        }

        cur = cur->child[c];
        ++cur->cnt;
    }

    ++cur->exist;
}

Trie::Node* Trie::find(std::string str) {
    Node *cur = root;
    for (char &tmp : str) {
        int c = tmp - 'a';
        if (cur->child[c] == nullptr) { //No such string exists
            return nullptr;
        }

        cur = cur->child[c];
    }

    if (cur->exist == 0) { //No string ends here
        return nullptr;
    }

    return cur;
}

bool Trie::erase(Node *cur, int pos, std::string &str) {
    if (pos < str.size()) {
        int c = str[pos] - 'a';
        bool res = erase(cur->child[c], pos + 1, str);
        if (res) {
            cur->child[c] = nullptr;
        }
    }
    else {
        --cur->exist;
    }

    if (cur != root) {
        --cur->cnt;
        if (cur->cnt == 0) {
            delete(cur);
            return true;
        }
    }

    return false;
}

void Trie::erase(std::string str) {
    if (find(str)) {
        erase(root, 0, str);
    }
}

void Trie::clear(Node *cur) {
    for (int i = 0; i < 26; i++) {
        if (cur->child[i]) {
            clear(cur->child[i]);
            cur->child[i] = nullptr;
        }
    }

    delete(cur);
}

void Trie::clear() {
    clear(root);
    root = nullptr;
}

bool Trie::check_valid_input(std::string str) {
    for (char &c : str) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}