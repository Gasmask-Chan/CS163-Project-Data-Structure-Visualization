#ifndef MINIMUM_SPANNING_TREE_H
#define MINIMUM_SPANNING_TREE_H

#include "UI_config.h"

#include <vector>
#include <unordered_map>

namespace Data_Structure {
struct DSU {
    std::vector<int> p;
   
    /**
     * @brief Construct a new DSU object
     * 
     * @param n 
     */
    DSU(int n = 0);

    /**
     * @brief Return the size of the current DSU
     * 
     * @return int 
     */
    int size();

    /**
     * @brief Destroy the DSU object
     * 
     */
    ~DSU();

    /**
     * @brief Return the root of node `u`
     * 
     * @param u 
     * @return int 
     */
    int find(int u);

    /**
     * @brief Join two components `u` and `v`
     * 
     * Return `true` if the joining is successful
     * 
     * @param u 
     * @param v 
     * @return true 
     * @return false 
     */
    bool join(int u, int v);

    /**
     * @brief Clear the entire DSU
     * 
     * @return void
     */
    void clear();
};

class MST {
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

        int status; //0 = Neutral, 1 = Belongs to MST, 2 = Ignored

        /**
         * @brief Construct a new Edge object
         * 
         * @param u 
         * @param v 
         * @param w 
         */
        Edge(int u = 0, int v = 0, int w = 0, int status = 0) : u(u), v(v), w(w), status(status) {}

        bool operator < (Edge &other) const;
    };

private:

    DSU dsu;
    std::unordered_map<int, int> mp; //Use for node numbering
    
public:
    std::vector<Node> nodes;
    std::vector<Edge> edges;

    /**
     * @brief Construct a new MST object
     * 
     */
    MST();

    /**
     * @brief Destroy the MST object
     * 
     */
    ~MST();

    /**
     * @brief Insert new edge to the MST
     * 
     * @param edge 
     * 
     * @return void
     */
    void insert(Edge edge);

    /**
     * @brief Get the mapping index of the given vertex
     * 
     * @param u 
     * 
     * @return int
     */
    int get_index(int u);

    /**
     * @brief Insert new edge to the MST
     * 
     * @param u 
     * @param v 
     * @param w 
     * 
     * @return void
     */
    void insert(int u, int v, int w);

    /**
     * @brief Build the Minimum Spanning Tree
     * 
     * The function return a list of edges in the MST
     * 
     * @return std::vector<Edge> 
     */
    std::vector<Edge> find_mst();

    /**
     * @brief Clear the entire MST
     * 
     * @return void
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