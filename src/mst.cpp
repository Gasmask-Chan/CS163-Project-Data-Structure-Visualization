#include "../include/mst.h"

#include <algorithm>

using namespace Data_Structure;

DSU::DSU(int n) {
    p.assign(n + 5, -1);
}

DSU::~DSU() {
    clear();
}

int DSU::find(int u) {
    return p[u] < 0 ? u : p[u] = find(p[u]);
}

bool DSU::join(int u, int v) {
    u = find(u), v = find(v);
    if (u == v) {
        return false;
    }

    if (p[u] > p[v]) std::swap(u, v);
    p[u] += p[v];

    return true;
}

void DSU::clear() {
    p.clear();
}

bool MST::Edge::operator < (Edge &other) const {
    return w < other.w;
}

MST::MST(int n) {
    dsu = DSU(n);
}

MST::~MST() {
    clear();
}

void MST::insert(Edge edge) {
    edges.push_back(edge);
}

void MST::insert(int u, int v, int w) {
    insert(Edge(u, v, w));
}

std::vector<MST::Edge> MST::find_mst() {
    std::sort(edges.begin(), edges.end());

    std::vector<Edge> res; 
    for (auto &[u, v, w] : edges) {
        if (dsu.join(u, v)) {
            res.push_back(Edge(u, v, w));
        }
    }

    return res;
}

void MST::clear() {
    edges.clear();
    dsu.clear();
}

