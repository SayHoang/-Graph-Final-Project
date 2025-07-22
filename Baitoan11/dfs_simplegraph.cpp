// depth_first_search.cpp
// Depth-First Search (DFS) for a simple graph
// Reads input from 'input.txt' in the same directory.
// Vertex labels are strings (e.g., 'u', 'v', ...).

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>

enum class Color {
    WHITE, GRAY, BLACK
};

struct Vertex {
    std::string label;
    Color color;
    int discovery;
    int finish;
    std::string parent;
    Vertex() : label(""), color(Color::WHITE), discovery(-1), finish(-1), parent("") {}
    Vertex(const std::string& label_) : label(label_), color(Color::WHITE), discovery(-1), finish(-1), parent("") {}
};

class SimpleGraph {
public:
    std::map<std::string, std::vector<std::string>> adj; // label -> neighbors
    std::map<std::string, Vertex> vertices; // label -> Vertex
    int time;

    SimpleGraph() : time(0) {}

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
        // For undirected graph, also add adj[v].push_back(u);
        // For directed graph, comment out the next line:
        // adj[v].push_back(u);
    }

    void executeDepthFirstSearch() {
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            it->second.color = Color::WHITE;
            it->second.parent = "";
            it->second.discovery = -1;
            it->second.finish = -1;
        }
        time = 0;
        // Visit all vertices in sorted order for deterministic output
        std::vector<std::string> sorted_labels;
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            sorted_labels.push_back(it->first);
        }
        std::sort(sorted_labels.begin(), sorted_labels.end());
        for (const std::string& label : sorted_labels) {
            if (vertices[label].color == Color::WHITE) {
                dfsVisit(label);
            }
        }
    }

    void dfsVisit(const std::string& u_label) {
        Vertex& u = vertices[u_label];
        time++;
        u.discovery = time;
        u.color = Color::GRAY;
        // Visit neighbors in sorted order for deterministic output
        std::vector<std::string> neighbors = adj[u_label];
        std::sort(neighbors.begin(), neighbors.end());
        for (const std::string& v_label : neighbors) {
            Vertex& v = vertices[v_label];
            if (v.color == Color::WHITE) {
                v.parent = u.label;
                dfsVisit(v_label);
            }
        }
        u.color = Color::BLACK;
        time++;
        u.finish = time;
    }

    void printDFSResult() const {
        std::vector<std::string> sorted_labels;
        for (auto it = vertices.begin(); it != vertices.end(); ++it) {
            sorted_labels.push_back(it->first);
        }
        std::sort(sorted_labels.begin(), sorted_labels.end());
        std::cout << "Vertex | d  | f  | Parent\n";
        std::cout << "-------|----|----|--------\n";
        for (const std::string& label : sorted_labels) {
            const Vertex& v = vertices.at(label);
            std::cout << label << "\t | " << v.discovery << " | " << v.finish << " | ";
            if (v.parent.empty()) {
                std::cout << "NIL";
            } else {
                std::cout << v.parent;
            }
            std::cout << std::endl;
        }
    }
};

SimpleGraph readSimpleGraphFromFile(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin) {
        std::cerr << "Error: Cannot open " << filename << std::endl;
        exit(1);
    }
    SimpleGraph g;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;
        std::string u = line.substr(0, colon);
        u.erase(std::remove_if(u.begin(), u.end(), ::isspace), u.end());
        std::string rest = line.substr(colon + 1);
        size_t pos = 0;
        while (pos < rest.size()) {
            while (pos < rest.size() && isspace(rest[pos])) ++pos;
            size_t start = pos;
            while (pos < rest.size() && !isspace(rest[pos]) && rest[pos] != ',') ++pos;
            if (start < pos) {
                std::string v = rest.substr(start, pos - start);
                v.erase(std::remove_if(v.begin(), v.end(), ::isspace), v.end());
                if (!v.empty()) g.addEdge(u, v);
            }
            while (pos < rest.size() && (isspace(rest[pos]) || rest[pos] == ',')) ++pos;
        }
    }
    return g;
}

int main() {
    SimpleGraph g = readSimpleGraphFromFile("input.txt");
    g.executeDepthFirstSearch();
    g.printDFSResult();
    return 0;
} 