#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "UI_config.h"

#include <queue>
#include <unordered_map>

namespace Data_Structure {
class Shortest_Path {
public:
    struct Node {
        int id;
        float current_x, current_y;
        float target_x, target_y;
        int highlighted; //0 = BLACK, 1 = RED, 2 = ORANGE

        /**
         * @brief Construct a new Node object
         * 
         * @param id 
         */
        Node(int id = -1);
    };

    struct Edge {
        int u, v, w; //u and v is the index of the node in the `nodes` vector

        int status; //0 = Neutral, 1 = Considering, 2 = Ignored

        /**
         * @brief Construct a new Edge object
         * 
         * @param u 
         * @param v 
         * @param w 
         */
        Edge(int u = 0, int v = 0, int w = 0, int status = 0) : u(u), v(v), w(w), status(status) {}
    };

private:
    int n;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> q;
    
    std::unordered_map<int, int> mp; //Use for node numbering
    
    public:
    std::vector<int> dis;
    std::vector<Node> nodes;
    std::vector<Edge> edges;
    std::vector<std::vector<int>> adj;

    /**
     * @brief Construct a new Shortest_Path object
     * 
     */
    Shortest_Path();

    /**
     * @brief Destroy the Shortest_Path object
     * 
     */
    ~Shortest_Path();

    /**
     * @brief Get the mapping index of the given vertex
     * 
     * @param u 
     * 
     * @return int
     */
    int get_index(int u);

    /**
     * @brief Add an edge weighted `w` from `u` toward `v` to the graph
     * 
     * @param u 
     * @param v 
     * @param w 
     * 
     * @return void
     */
    void insert(int u, int v, int w);

    /**
     * @brief Find all of shortest paths from `s`
     * 
     * If the given vertex `s` exceeds the size of the graph, the function will return `false`
     * 
     * Otherwise, return `true` if run sucessfully
     * 
     * @param s
     * 
     * @return bool
     * 
     */
    bool find_sp(int s);

    /**
     * @brief Clear the entire `Shortest_Path`
     * 
     */
    void clear();

    /**
     * @brief Reset data to allow rerun
     * 
     */
    void reset();

    //========================UI========================
    struct Snapshot_Data {
        std::vector<Node> nodes;
        std::vector<Edge> edges;
        std::vector<int> dis;
        int index; //Current code line
        UI::OPERATION op;
    };

    std::vector<Snapshot_Data> history;

    /**
     * @brief Save snapshot of the current graph to `history` vector
     * 
     * @param index
     * @param op
     */
    void save_snapshot(int index, UI::OPERATION op);
};
}

#endif