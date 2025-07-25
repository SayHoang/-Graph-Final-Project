// bfs_generalgraph.cpp
// Breadth-First Search (BFS) for a general graph
// Supports simple graphs, multigraphs, directed/undirected graphs
// Follows the C++ guidelines provided in @custom-rule.mdc
// Reads input from 'input.txt' in the same directory

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <limits>

/**
 * @brief Color states for BFS traversal.
 */
enum class Color {
    WHITE, ///< Not discovered
    GRAY,  ///< Discovered, in queue
    BLACK  ///< Fully explored
};

/**
 * @brief Represents a vertex in the general graph.
 */
struct Vertex {
    std::string label; ///< Vertex label
    Color color; ///< Color state
    int distance; ///< Distance from source
    std::string parent; ///< Predecessor label (empty if none)
    
    Vertex() : label(""), color(Color::WHITE), distance(std::numeric_limits<int>::max()), parent("") {}
    
    Vertex(const std::string& label_) 
        : label(label_), color(Color::WHITE), distance(std::numeric_limits<int>::max()), parent("") {}
};

/**
 * @brief Represents a general graph using adjacency list.
 * Can handle simple graphs, multigraphs, directed/undirected graphs.
 */
class GeneralGraph {
public:
    std::map<std::string, std::vector<std::string>> adjacencyList; ///< label -> neighbors (may have duplicates)
    std::map<std::string, Vertex> vertices; ///< label -> Vertex
    bool isDirected; ///< Whether the graph is directed
    
    GeneralGraph(bool directed = false) : isDirected(directed) {}
    
    /**
     * @brief Add a vertex to the graph.
     * @param label Vertex label
     */
    void addVertex(const std::string& label) {
        if (vertices.count(label) == 0) {
            vertices.emplace(label, Vertex(label));
            adjacencyList[label] = std::vector<std::string>();
        }
    }
    
    /**
     * @brief Add an edge to the graph.
     * @param u Source vertex label
     * @param v Target vertex label
     */
    void addEdge(const std::string& u, const std::string& v) {
        addVertex(u);
        addVertex(v);
        adjacencyList[u].push_back(v);
        if (!isDirected) {
            adjacencyList[v].push_back(u);
        }
    }
    
    /**
     * @brief Execute BFS from a given source vertex.
     * @param sourceLabel The source vertex label
     */
    void executeBreadthFirstSearch(const std::string& sourceLabel) {
        initializeVertices();
        if (vertices.count(sourceLabel) == 0) {
            std::cerr << "Error: Source vertex " << sourceLabel << " not found in graph" << std::endl;
            return;
        }
        
        Vertex& source = vertices[sourceLabel];
        source.color = Color::GRAY;
        source.distance = 0;
        source.parent = "";
        
        std::queue<std::string> queue;
        queue.push(sourceLabel);
        
        while (!queue.empty()) {
            std::string currentLabel = queue.front();
            queue.pop();
            Vertex& current = vertices[currentLabel];
            
            for (const std::string& neighborLabel : adjacencyList[currentLabel]) {
                Vertex& neighbor = vertices[neighborLabel];
                if (neighbor.color == Color::WHITE) {
                    neighbor.color = Color::GRAY;
                    neighbor.distance = current.distance + 1;
                    neighbor.parent = current.label;
                    queue.push(neighborLabel);
                }
            }
            current.color = Color::BLACK;
        }
    }
    
    /**
     * @brief Print BFS results in tabular format.
     */
    void printBFSResult() const {
        std::vector<std::string> sortedLabels;
        for (const auto& pair : vertices) {
            sortedLabels.push_back(pair.first);
        }
        std::sort(sortedLabels.begin(), sortedLabels.end());
        
        std::cout << "Vertex | Distance | Predecessor\n";
        std::cout << "-------|----------|------------\n";
        
        for (const std::string& label : sortedLabels) {
            const Vertex& vertex = vertices.at(label);
            std::cout << label << "\t | ";
            
            if (vertex.distance == std::numeric_limits<int>::max()) {
                std::cout << "INF";
            } else {
                std::cout << vertex.distance;
            }
            
            std::cout << "\t   | ";
            
            if (vertex.parent.empty()) {
                std::cout << "NIL";
            } else {
                std::cout << vertex.parent;
            }
            
            std::cout << std::endl;
        }
    }
    
    /**
     * @brief Get graph statistics.
     */
    void printGraphInfo() const {
        int vertexCount = static_cast<int>(vertices.size());
        int edgeCount = 0;
        for (const auto& pair : adjacencyList) {
            edgeCount += static_cast<int>(pair.second.size());
        }
        if (!isDirected) {
            edgeCount /= 2; // Each undirected edge is counted twice
        }
        
        std::cout << "Graph Info: " << vertexCount << " vertices, " << edgeCount << " edges, ";
        std::cout << (isDirected ? "directed" : "undirected") << std::endl;
    }

private:
    /**
     * @brief Initialize all vertices for BFS.
     */
    void initializeVertices() {
        for (auto& pair : vertices) {
            Vertex& vertex = pair.second;
            vertex.color = Color::WHITE;
            vertex.distance = std::numeric_limits<int>::max();
            vertex.parent = "";
        }
    }
};

/**
 * @brief Read general graph from input file.
 * @param filename Input file name
 * @param source Reference to store source vertex label
 * @return Constructed GeneralGraph
 */
GeneralGraph readGeneralGraphFromFile(const std::string& filename, std::string& source) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error: Cannot open " << filename << std::endl;
        exit(1);
    }
    
    int vertexCount, edgeCount;
    std::string graphType;
    inputFile >> vertexCount >> edgeCount >> source >> graphType;
    
    bool directed = (graphType == "directed" || graphType == "D");
    GeneralGraph graph(directed);
    
    for (int i = 0; i < edgeCount; ++i) {
        std::string u, v;
        inputFile >> u >> v;
        graph.addEdge(u, v);
    }
    
    return graph;
}

int main() {
    std::string source;
    GeneralGraph graph = readGeneralGraphFromFile("input.txt", source);
    
    graph.printGraphInfo();
    graph.executeBreadthFirstSearch(source);
    graph.printBFSResult();
    
    return 0;
}
