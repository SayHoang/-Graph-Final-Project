// dfs_multigraph.cpp
// Depth-First Search (DFS) for a multigraph
// Supports parallel edges and self-loops
// Reads input from 'input.txt' in the same directory

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <string>
#include <fstream>
#include <algorithm>

/**
 * @brief Color states for DFS traversal.
 */
enum class Color {
    WHITE, ///< Not discovered
    GRAY,  ///< Discovered, in progress
    BLACK  ///< Fully explored
};

/**
 * @brief Represents a vertex in the multigraph.
 */
struct Vertex {
    std::string label; ///< Vertex label
    Color color; ///< Color state
    int discoveryTime; ///< Discovery time
    int finishTime; ///< Finish time
    std::string parent; ///< Parent vertex label (empty if none)
    
    Vertex() : label(""), color(Color::WHITE), discoveryTime(-1), finishTime(-1), parent("") {}
    
    Vertex(const std::string& label_) 
        : label(label_), color(Color::WHITE), discoveryTime(-1), finishTime(-1), parent("") {}
};

/**
 * @brief Represents a multigraph using adjacency list with parallel edges.
 */
class Multigraph {
public:
    std::map<std::string, std::vector<std::string>> adjacencyList; ///< label -> neighbors (with duplicates)
    std::map<std::string, Vertex> vertices; ///< label -> Vertex
    int globalTime; ///< Global timer for discovery/finish times
    
    Multigraph() : globalTime(0) {}
    
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
     * @brief Add an edge to the multigraph (allows parallel edges).
     * @param u Source vertex label
     * @param v Target vertex label
     */
    void addEdge(const std::string& u, const std::string& v) {
        if (u == v) {
            std::cerr << "Error: Self-loops not allowed in multigraph\n";
            return;
        }
        addVertex(u);
        addVertex(v);
        adjacencyList[u].push_back(v);
        adjacencyList[v].push_back(u); // Undirected multigraph
    }
    
    /**
     * @brief Execute DFS traversal on the entire graph (handles disconnected components).
     */
    void executeDepthFirstSearch() {
        initializeVertices();
        globalTime = 0;
        
        // Visit all vertices in sorted order for deterministic output
        std::vector<std::string> sortedLabels;
        for (const auto& pair : vertices) {
            sortedLabels.push_back(pair.first);
        }
        std::sort(sortedLabels.begin(), sortedLabels.end());
        
        for (const std::string& label : sortedLabels) {
            if (vertices[label].color == Color::WHITE) {
                dfsVisitRecursive(label);
            }
        }
    }
    
    /**
     * @brief Execute iterative DFS traversal on the entire graph.
     */
    void executeDepthFirstSearchIterative() {
        initializeVertices();
        globalTime = 0;
        
        std::vector<std::string> sortedLabels;
        for (const auto& pair : vertices) {
            sortedLabels.push_back(pair.first);
        }
        std::sort(sortedLabels.begin(), sortedLabels.end());
        
        for (const std::string& label : sortedLabels) {
            if (vertices[label].color == Color::WHITE) {
                dfsVisitIterative(label);
            }
        }
    }
    
    /**
     * @brief Print DFS results in tabular format.
     */
    void printDFSResult() const {
        std::vector<std::string> sortedLabels;
        for (const auto& pair : vertices) {
            sortedLabels.push_back(pair.first);
        }
        std::sort(sortedLabels.begin(), sortedLabels.end());
        
        std::cout << "Vertex | d  | f  | Parent\n";
        std::cout << "-------|----|----|--------\n";
        
        for (const std::string& label : sortedLabels) {
            const Vertex& vertex = vertices.at(label);
            std::cout << label << "\t | " << vertex.discoveryTime << " | " << vertex.finishTime << " | ";
            
            if (vertex.parent.empty()) {
                std::cout << "NIL";
            } else {
                std::cout << vertex.parent;
            }
            
            std::cout << std::endl;
        }
    }
    
    /**
     * @brief Print graph statistics.
     */
    void printGraphInfo() const {
        int vertexCount = static_cast<int>(vertices.size());
        int edgeCount = 0;
        for (const auto& pair : adjacencyList) {
            edgeCount += static_cast<int>(pair.second.size());
        }
        edgeCount /= 2; // Each undirected edge is counted twice
        
        std::cout << "Multigraph Info: " << vertexCount << " vertices, " << edgeCount << " edges (including parallel edges)\n";
    }

private:
    /**
     * @brief Initialize all vertices for DFS.
     */
    void initializeVertices() {
        for (auto& pair : vertices) {
            Vertex& vertex = pair.second;
            vertex.color = Color::WHITE;
            vertex.discoveryTime = -1;
            vertex.finishTime = -1;
            vertex.parent = "";
        }
    }
    
    /**
     * @brief Recursive DFS visit implementation.
     * @param currentLabel Current vertex label
     */
    void dfsVisitRecursive(const std::string& currentLabel) {
        Vertex& current = vertices[currentLabel];
        globalTime++;
        current.discoveryTime = globalTime;
        current.color = Color::GRAY;
        
        // Visit neighbors in sorted order for deterministic output
        std::vector<std::string> neighbors = adjacencyList[currentLabel];
        std::sort(neighbors.begin(), neighbors.end());
        
        for (const std::string& neighborLabel : neighbors) {
            Vertex& neighbor = vertices[neighborLabel];
            if (neighbor.color == Color::WHITE) {
                neighbor.parent = currentLabel;
                dfsVisitRecursive(neighborLabel);
            }
        }
        
        current.color = Color::BLACK;
        globalTime++;
        current.finishTime = globalTime;
    }
    
    /**
     * @brief Iterative DFS visit implementation using explicit stack.
     * @param startLabel Starting vertex label
     */
    void dfsVisitIterative(const std::string& startLabel) {
        std::stack<std::pair<std::string, bool>> stack; // (vertex, isFinishing)
        stack.push({startLabel, false});
        
        while (!stack.empty()) {
            std::pair<std::string, bool> current = stack.top();
            stack.pop();
            
            std::string currentLabel = current.first;
            bool isFinishing = current.second;
            
            Vertex& currentVertex = vertices[currentLabel];
            
            if (isFinishing) {
                currentVertex.color = Color::BLACK;
                globalTime++;
                currentVertex.finishTime = globalTime;
            } else {
                if (currentVertex.color == Color::WHITE) {
                    globalTime++;
                    currentVertex.discoveryTime = globalTime;
                    currentVertex.color = Color::GRAY;
                    
                    // Push finish marker
                    stack.push({currentLabel, true});
                    
                    // Visit neighbors in reverse sorted order (stack reverses order)
                    std::vector<std::string> neighbors = adjacencyList[currentLabel];
                    std::sort(neighbors.begin(), neighbors.end(), std::greater<std::string>());
                    
                    for (const std::string& neighborLabel : neighbors) {
                        Vertex& neighbor = vertices[neighborLabel];
                        if (neighbor.color == Color::WHITE) {
                            neighbor.parent = currentLabel;
                            stack.push({neighborLabel, false});
                        }
                    }
                }
            }
        }
    }
};

/**
 * @brief Read multigraph from input file.
 * @param filename Input file name
 * @return Constructed Multigraph
 */
Multigraph readMultigraphFromFile(const std::string& filename) {
    std::ifstream inputFile(filename);
    if (!inputFile) {
        std::cerr << "Error: Cannot open " << filename << std::endl;
        exit(1);
    }
    
    int vertexCount, edgeCount;
    inputFile >> vertexCount >> edgeCount;
    
    Multigraph graph;
    
    for (int i = 0; i < edgeCount; ++i) {
        std::string u, v;
        inputFile >> u >> v;
        graph.addEdge(u, v);
    }
    
    return graph;
}

int main() {
    Multigraph graph = readMultigraphFromFile("input.txt");
    
    graph.printGraphInfo();
    std::cout << "\n--- Recursive DFS ---\n";
    graph.executeDepthFirstSearch();
    graph.printDFSResult();
    
    std::cout << "\n--- Iterative DFS ---\n";
    graph.executeDepthFirstSearchIterative();
    graph.printDFSResult();
    
    return 0;
}
