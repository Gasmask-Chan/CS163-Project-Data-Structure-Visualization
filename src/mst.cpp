#include "../include/mst.h"

#include <algorithm>

using namespace Data_Structure;

DSU::DSU(int n) {
    p.assign(n + 5, -1);
}

DSU::~DSU() {
    clear();
}

int DSU::size() {
    return (int)p.size();
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

MST::Node::Node(int id) {
    this->id = id;
    current_x = current_y = target_x = target_y = 0;
    highlighted = false;
}

bool MST::Edge::operator < (Edge &other) const {
    return w < other.w;
}

MST::MST() {}

MST::~MST() {
    clear();
}

void MST::insert(Edge edge) {
    edges.push_back(edge);
}

int MST::get_index(int u) {
    auto &index = mp[u];
    if (index == 0) {
        nodes.push_back(Node(u));
        index = nodes.size();
    }
    return index - 1;
}

void MST::insert(int u, int v, int w) {
    u = get_index(u);
    v = get_index(v);

    if ((int)nodes.size() > (int)dsu.size()) {
        dsu = DSU(nodes.size());
    }

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
    nodes.clear();
    edges.clear();
    dsu.clear();
    mp.clear();
    history.clear();
}

//===========================UI===========================

void MST::save_snapshot(int index, UI::OPERATION op) {
    history.push_back({nodes, edges, index, op});
}