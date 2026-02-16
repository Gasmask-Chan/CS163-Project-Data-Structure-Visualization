#ifndef SHORTEST_PATH
#define SHORTEST_PATH

#include <queue>

namespace Data_Structure {
class Shortest_Path {
private:
    int n;
    int *dis;
    std::vector<std::pair<int, int>> *adj;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> q;

public:
    /**
     * @brief Construct a new Shortest_Path object
     * 
     * @param n 
     */
    Shortest_Path(int n = 0);

    /**
     * @brief Destroy the Shortest_Path object
     * 
     */
    ~Shortest_Path();

    /**
     * @brief Add an edge weighted `w` from `u` toward `v` to the graph
     * 
     * The program will be asserted if the given `u`, `v` are out of the initial size of the graph
     * 
     * @param u 
     * @param v 
     * @param w 
     * 
     * @return void
     */
    void insert(int u, int v, int w);

    /**
     * @brief Find the shortest path from `s`
     * 
     * The program will be asserted if the given `s` is out of the initial size of the graph
     * 
     * @param s
     * @param t
     * 
     * @return void
     * 
     */
    void find_sp(int s);

    /**
     * @brief Clear the entire `Shortest_Path`
     * 
     */
    void clear();
};
}

#endif