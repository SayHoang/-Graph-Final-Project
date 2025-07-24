// bfs_multigraph.cpp
// Breadth-First Search (BFS) for a multigraph (đa đồ thị)
// Reads input from 'input.txt' in the same directory.
// Vertex labels are strings (e.g., 'A', 'B', ...).

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>

enum class Color {
    WHITE, GRAY, BLACK
};

struct Vertex {
    std::string label;
    Color color;
    int distance;
    std::string parent;
    Vertex() : label(""), color(Color::WHITE), distance(std::numeric_limits<int>::max()), parent("") {}
    Vertex(const std::string& label_)
        : label(label_), color(Color::WHITE), distance(std::numeric_limits<int>::max()), parent("") {}
};

class MultiGraph {
public:
    std::map<std::string, std::vector<std::string>> adj; // label -> neighbors (with duplicates)
    std::map<std::string, Vertex> vertices; // label -> Vertex

    void addEdge(const std::string& u, const std::string& v) {
        if (adj.count(u) == 0) {
            adj[u] = std::vector<std::string>();
            vertices.emplace(u, Vertex(u));
        }
        if (adj.count(v) == 0) {
            adj[v] = std::vector<std::string>();
            vertices.emplace(v, Vertex(v));
        }
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected
    }

    void executeBreadthFirstSearch(const std::string& s_label) {
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            auto& label = it->first;
            auto& v = it->second;
            v.color = Color::WHITE;
            v.distance = std::numeric_limits<int>::max();
            v.parent = "";
        }
        Vertex& src = vertices[s_label];
        src.color = Color::GRAY;
        src.distance = 0;
        src.parent = "";
        std::queue<std::string> q;
        q.push(src.label);
        while (!q.empty()) {
            std::string u_label = q.front();
            q.pop();
            Vertex& u = vertices[u_label];
            for (const std::string& v_label : adj[u_label]) {
                Vertex& v = vertices[v_label];
                if (v.color == Color::WHITE) {
                    v.color = Color::GRAY;
                    v.distance = u.distance + 1;
                    v.parent = u.label;
                    q.push(v.label);
                }
            }
            u.color = Color::BLACK;
        }
    }

    void printBFSResult() const {
        std::vector<std::string> sorted_labels;
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            auto& label = it->first;
            sorted_labels.push_back(label);
        }
        std::sort(sorted_labels.begin(), sorted_labels.end());
        std::cout << "Vertex | Distance | Predecessor\n";
        std::cout << "-------|----------|------------\n";
        for (const std::string& label : sorted_labels) {
            const Vertex& v = vertices.at(label);
            std::cout << label << "\t | ";
            if (v.distance == std::numeric_limits<int>::max()) {
                std::cout << "INF";
            } else {
                std::cout << v.distance;
            }
            std::cout << "\t   | ";
            if (v.parent.empty()) {
                std::cout << "NIL";
            } else {
                std::cout << v.parent;
            }
            std::cout << std::endl;
        }
    }
};

MultiGraph readMultiGraphFromFile(const std::string& filename, std::string& source) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error: Cannot open " << filename << std::endl;
        exit(1);
    }
    int n, m;
    fin >> n >> m >> source;
    MultiGraph g;
    for (int i = 0; i < m; ++i) {
        std::string u, v;
        fin >> u >> v;
        g.addEdge(u, v);
    }
    return g;
}

int main() {
    std::string source;
    MultiGraph g = readMultiGraphFromFile("input.txt", source);
    g.executeBreadthFirstSearch(source);
    g.printBFSResult();
    return 0;
} 