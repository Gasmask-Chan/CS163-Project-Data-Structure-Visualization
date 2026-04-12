#include "../include/shortest_path.h"

#include <assert.h> //For assert
#include <cstring> //For std::memset

using namespace Data_Structure;

Shortest_Path::Node::Node(int id) {
    this->id = id;
    current_x = current_y = target_x = target_y = 0;
    highlighted = false;
}

Shortest_Path::Shortest_Path() {}

Shortest_Path::~Shortest_Path() {
    clear();
}

int Shortest_Path::get_index(int u) {
    auto &index = mp[u];
    if (index == 0) {
        nodes.push_back(Node(u));
        dis.push_back(0);
        adj.push_back({});

        index = nodes.size();
        n = index;
    }
    return index - 1;
}

void Shortest_Path::insert(int u, int v, int w) {
    u = get_index(u);
    v = get_index(v);

    for (auto &edge :edges) {
        if ((edge.u == u && edge.v == v) || (edge.u == v && edge.v == u)) {
            edge.w = std::min(edge.w, w);
            return;
        }
    }

    adj[u].emplace_back(edges.size());
    adj[v].emplace_back(edges.size());

    edges.push_back({u, v, w});
}

bool Shortest_Path::find_sp(int s) {
    auto it = mp.find(s);
    if (it == mp.end()) return false;
    s = it->second - 1;

    std::fill(dis.begin(), dis.end(), 1e9 + 7);
    save_snapshot(0, UI::OPERATION::FIND);

    q.emplace(dis[s] = 0, s);
    save_snapshot(1, UI::OPERATION::FIND);
    save_snapshot(2, UI::OPERATION::FIND);

    while (!q.empty()) {
        int w, u;
        std::tie(w, u) = q.top();
        q.pop();

        nodes[u].highlighted = true;
        save_snapshot(3, UI::OPERATION::FIND);
        
        save_snapshot(4, UI::OPERATION::FIND);
        if (dis[u] != w) {
            nodes[u].highlighted = false;
            continue;
        }

        save_snapshot(5, UI::OPERATION::FIND);
        for (auto &cur_id : adj[u]) {
            int v = u == edges[cur_id].v ? edges[cur_id].u : edges[cur_id].v;
            int c = edges[cur_id].w;

            edges[cur_id].status = 1;
            nodes[v].highlighted = true;
            save_snapshot(6, UI::OPERATION::FIND);
            if (w + c < dis[v]) {
                q.emplace(dis[v] = w + c, v);
                save_snapshot(7, UI::OPERATION::FIND);
                save_snapshot(8, UI::OPERATION::FIND);
            }
            nodes[v].highlighted = false;
            edges[cur_id].status = 0;
            save_snapshot(5, UI::OPERATION::FIND);
        }

        save_snapshot(2, UI::OPERATION::FIND);
    }

    save_snapshot(-1, UI::OPERATION::FIND);
    return true;
}

void Shortest_Path::clear() {
    dis.clear();
    adj.clear();
    nodes.clear();
    edges.clear();

    while (!q.empty()) {
        q.pop();
    }

    n = 0;
}

void Shortest_Path::reset() {
    history.clear();

    for (auto &e : edges) {
        e.status = 0;
    }

    for (auto &node : nodes) {
        node.highlighted = 0;
    }
}

//===========================UI===========================

void Shortest_Path::save_snapshot(int index, UI::OPERATION op) {
    history.push_back({nodes, edges, dis, index, op});
}