#ifndef TRIE_H
#define TRIE_H

#include "UI_config.h"
#include "UI_theme.h"
#include <string>
#include <vector>

namespace Data_Structure {
class Trie {
public:
    struct Node {
        int cnt; //Number of strings go through this node
        int exist; //Number of strings end at this node
        Node *child[26];

        float current_x, current_y;
        float target_x, target_y;
        float tree_width;
        bool highlighted;

        /**
         * @brief Construct a new Node object
         * 
         */
        Node();
    };

private:
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
     * @brief Return the node which the string ended at
     * 
     * If no such node exists, a 'nullptr' will be returned
     * 
     * Use this function if you don't want it to save snapshot
     * 
     * @return Node* 
     */
    Node* raw_find(std::string);

    /**
     * @brief Erase the string from the trie
     * 
     * If the string is not in trie initially, the function will do nothing
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
     * If the string is not in trie initially, the function will do nothing
     * 
     * Return `true` if 
     * 
     * @param str 
     * 
     * @return bool
     */
    bool erase(std::string str);

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

    //========================UI========================
    struct Snapshot_Data {
        Node* tree_root;
        int index; //Current code line
        UI::OPERATION op;
    };

    const float vertical_gap = 80.0f;
    const float horizontal_gap = 25.0f;

    std::vector<Snapshot_Data> history;

    Node* get_copy(Node* cur);

    /**
     * @brief Return the root of a new copy of the current tree
     * 
     * @return Node* 
     */
    Node* get_copy();

    /**
     * @brief Calculate the maximum horizontal gap we need for each node in the tree
     * 
     * @return void 
     */
    void cal_initial_gap(Node* cur);

    /**
     * @brief Save snapshot of the current tree to `history` vector
     * 
     * @param index
     * @param op
     */
    void save_snapshot(int index, UI::OPERATION op);

    void recalculate_position(Node* cur, float x, float y);

    /**
     * @brief Recalculate postion of each node in tree
     * 
     */
    void recalculate_position();
};
}

#endif