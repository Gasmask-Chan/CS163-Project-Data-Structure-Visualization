#ifndef TRIE_H
#define TRIE_H

#include <string>

namespace Data_Structure {
class Trie {
private:
    struct Node {
        int cnt; //Number of strings go through this node
        int exist; //Number of strings end at this node
        Node *child[26];

        /**
         * @brief Construct a new Node object
         * 
         */
        Node();
    };
    
    Node *root;

public:
    /**
     * @brief Construct a new Trie object
     * 
     */
    Trie();

    /**
     * @brief Destroy the Trie object
     * 
     */
    ~Trie();

    /**
     * @brief Insert new string into the trie
     * 
     * @param str
     */
    void insert(std::string str);

    /**
     * @brief Return the node which the string ended at
     * 
     * If no such node exists, a `nullptr` will be returned
     * 
     * @return Node* 
     */
    Node* find(std::string);

    /**
     * @brief Erase the string from the trie
     * 
     * If the string initially is not in trie, the function will do nothing
     * 
     * @param cur 
     * @param pos
     * @param str 
     * @return true 
     * @return false 
     */
    bool erase(Node *cur, int pos, std::string &str);

    /**
     * @brief Erase the string from the trie
     * 
     * If the string is not in trie initiallys, the function will do nothing
     * 
     * 
     * @param str 
     */
    void erase(std::string str);

    /**
     * @brief Clear the entire trie
     * 
     * @param cur 
     */
    void clear(Node *cur);

    /**
     * @brief Clear the entire trie
     * 
     * @return void
     */
    void clear();

    /**
     * @brief Return true if the string is safe to use with the trie
     * 
     * @param str 
     * @return true 
     * @return false 
     */
    bool check_valid_input(std::string str);
};
}

#endif