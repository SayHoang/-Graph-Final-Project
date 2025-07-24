// breadth_first_search.cpp
// Breadth-First Search (BFS) for a simple graph

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <fstream>

/**
 * @brief Color states for BFS traversal.
 */
enum class Color {
    WHITE, ///< Not discovered
    GRAY,  ///< Discovered, in queue
    BLACK  ///< Fully explored
};

/**
 * @brief Represents a vertex in the graph.
 */
struct Vertex {
    int id; ///< Vertex index (0-based)
    Color color; ///< Color state
    int distance; ///< Distance from source
    int parent; ///< Predecessor vertex (-1 if none)
    Vertex(int id_) : id(id_), color(Color::WHITE), distance(std::numeric_limits<int>::max()), parent(-1) {}
};

/**
 * @brief Represents a simple undirected graph using adjacency list.
 */
class Graph {
public:
    int n; ///< Number of vertices
    std::vector<std::vector<int>> adj; ///< Adjacency list
    std::vector<Vertex> vertices; ///< Vertex properties

    Graph(int n_) : n(n_), adj(n_), vertices() {
        for (int i = 0; i < n; ++i) {
            vertices.emplace_back(i);
        }
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected graph
    }
};

/**
 * @brief Performs BFS from a given source vertex.
 * @param g The graph
 * @param s The source vertex (0-based index)
 */
void executeBreadthFirstSearch(Graph& g, int s) {
    for (int i = 0; i < g.n; ++i) {
        g.vertices[i].color = Color::WHITE;
        g.vertices[i].distance = std::numeric_limits<int>::max();
        g.vertices[i].parent = -1;
    }
    g.vertices[s].color = Color::GRAY;
    g.vertices[s].distance = 0;
    g.vertices[s].parent = -1;
    std::queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g.adj[u]) {
            if (g.vertices[v].color == Color::WHITE) {
                g.vertices[v].color = Color::GRAY;
                g.vertices[v].distance = g.vertices[u].distance + 1;
                g.vertices[v].parent = u;
                q.push(v);
            }
        }
        g.vertices[u].color = Color::BLACK;
    }
}

/**
 * @brief Reads a graph from input stream.
 * @param in Input stream
 * @return Constructed Graph and source vertex
 */
std::pair<Graph, int> readGraph(std::istream& in) {
    int n, m, s;
    in >> n >> m >> s;
    Graph g(n);
    for (int i = 0; i < m; ++i) {
        int u, v;
        in >> u >> v;
        // Input vertices are 1-based, convert to 0-based
        g.addEdge(u - 1, v - 1);
    }
    return {g, s - 1}; // Convert source to 0-based
}

/**
 * @brief Prints BFS results: distances and predecessors.
 * @param g The graph
 */
void printBFSResult(const Graph& g) {
    std::cout << "Vertex | Distance | Predecessor\n";
    std::cout << "-------|----------|------------\n";
    for (int i = 0; i < g.n; ++i) {
        std::cout << (i + 1) << "\t | ";
        if (g.vertices[i].distance == std::numeric_limits<int>::max()) {
            std::cout << "INF";
        } else {
            std::cout << g.vertices[i].distance;
        }
        std::cout << "\t   | ";
        if (g.vertices[i].parent == -1) {
            std::cout << "NIL";
        } else {
            std::cout << (g.vertices[i].parent + 1);
        }
        std::cout << std::endl;
    }
}

int main() {
    // Input format:
    // n m s
    // u1 v1
    // u2 v2
    // ...
    // um vm
    // n: number of vertices (1-based)
    // m: number of edges
    // s: source vertex (1-based)
    // Each edge: u v (1-based)
    std::ifstream fin("input.txt");
    if (!fin) {
        std::cerr << "Error: Cannot open input.txt" << std::endl;
        return 1;
    }
    auto result = readGraph(fin);
    Graph& g = result.first;
    int s = result.second;
    executeBreadthFirstSearch(g, s);
    printBFSResult(g);
    return 0;
} 