#ifndef MINIMUM_SPANNING_TREE
#define MINIMUM_SPANNING_TREE

#include <vector>

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
private:
    struct Edge {
        int u, v, w;

        /**
         * @brief Construct a new Edge object
         * 
         * @param u 
         * @param v 
         * @param w 
         */
        Edge(int u = 0, int v = 0, int w = 0) : u(u), v(v), w(w) {}

        bool operator < (Edge &other) const;
    };

    DSU dsu;
    std::vector<Edge> edges;

public:
    /**
     * @brief Construct a new MST object
     * 
     * @param n 
     */
    MST(int n = 0);

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
};
}

#endif