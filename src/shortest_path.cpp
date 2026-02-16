#include "../include/shortest_path.h"

#include <assert.h> //For assert
#include <cstring> //For std::memset

using namespace Data_Structure;

Shortest_Path::Shortest_Path(int n) {
    this->n = n;
    dis = new int[n + 5];
    adj = new std::vector<std::pair<int, int>>[n + 5];
}

Shortest_Path::~Shortest_Path() {
    clear();
}

void Shortest_Path::insert(int u, int v, int w) {
    assert(std::min(u, v) > 0 && std::max(u, v) <= n);
    adj[u].emplace_back(v, w);
}

void Shortest_Path::find_sp(int s) {
    assert(s > 0 && s <= n);

    std::memset(dis, 0x3f, sizeof dis);

    q.emplace(dis[s] = 0, s);
    while (!q.empty()) {
        int w, u;
        std::tie(w, u) = q.top();
        q.pop();

        if (dis[u] != w) continue;

        for (auto &[v, c] : adj[u]) {
            if (w + c < dis[v]) {
                q.emplace(dis[v] = w + c, v);
            }
        }
    }
}

void Shortest_Path::clear() {
    delete[] dis;

    for (int i = 0; i < n + 5; i++) {
        adj[i].clear();
    }
    delete[] adj;

    while (!q.empty()) {
        q.pop();
    }

    n = 0;
}