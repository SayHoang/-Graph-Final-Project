#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Directed graph adjacency matrix to adjacency list converter
 *
 * Reads an adjacency matrix of a directed graph from standard input and outputs the adjacency list.
 *
 * Input format:
 *   n
 *   a_00 a_01 ... a_0(n-1)
 *   a_10 a_11 ... a_1(n-1)
 *   ...
 *   a_(n-1)0 ... a_(n-1)(n-1)
 *
 * Output format:
 *   For each vertex i (0-based), print: Adj[i]: v1 v2 ... vk
 *   where v1, v2, ..., vk are the vertices that i has outgoing edges to.
 */
namespace core {
    using AdjacencyMatrix = std::vector<std::vector<int>>;
    using AdjacencyList = std::vector<std::vector<int>>;

    /**
     * @brief Convert adjacency matrix to adjacency list for a directed graph
     * @param adjacencyMatrix The adjacency matrix (n x n)
     * @return The adjacency list (vector of vectors)
     */
    AdjacencyList convertAdjacencyMatrixToAdjacencyList(const AdjacencyMatrix& adjacencyMatrix) {
        int vertexCount = static_cast<int>(adjacencyMatrix.size());
        AdjacencyList adjacencyList(vertexCount);
        for (int source = 0; source < vertexCount; ++source) {
            for (int target = 0; target < vertexCount; ++target) {
                if (adjacencyMatrix[source][target] == 1) {
                    adjacencyList[source].push_back(target);
                }
            }
        }
        return adjacencyList;
    }
} // namespace core

namespace utils {
    /**
     * @brief Read adjacency matrix from standard input
     * @param vertexCount Number of vertices
     * @return The adjacency matrix
     */
    core::AdjacencyMatrix readAdjacencyMatrix(int vertexCount) {
        core::AdjacencyMatrix adjacencyMatrix(vertexCount, std::vector<int>(vertexCount, 0));
        for (int i = 0; i < vertexCount; ++i) {
            for (int j = 0; j < vertexCount; ++j) {
                std::cin >> adjacencyMatrix[i][j];
            }
        }
        return adjacencyMatrix;
    }

    /**
     * @brief Print adjacency list to standard output
     * @param adjacencyList The adjacency list
     */
    void printAdjacencyList(const core::AdjacencyList& adjacencyList) {
        for (std::size_t vertex = 0; vertex < adjacencyList.size(); ++vertex) {
            std::cout << "Adj[" << vertex << "]: ";
            for (std::size_t idx = 0; idx < adjacencyList[vertex].size(); ++idx) {
                std::cout << adjacencyList[vertex][idx];
                if (idx + 1 < adjacencyList[vertex].size()) {
                    std::cout << " ";
                }
            }
            std::cout << std::endl;
        }
    }
} // namespace utils

int main() {
    int vertexCount;
    std::cin >> vertexCount;
    core::AdjacencyMatrix adjacencyMatrix = utils::readAdjacencyMatrix(vertexCount);
    core::AdjacencyList adjacencyList = core::convertAdjacencyMatrixToAdjacencyList(adjacencyMatrix);
    utils::printAdjacencyList(adjacencyList);
    return 0;
}
