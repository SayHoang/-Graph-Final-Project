#include "adjacency_matrix.cpp"
#include "adjacency_list.cpp"
#include "extended_adjacency_list.cpp"
#include "adjacency_map.cpp"
#include <iostream>
#include <fstream>
#include <string>

/**
 * @brief Create sample input file with general graph data
 * @param fileName Name of file to create
 */
void createSampleInputFile(const std::string& fileName) {
    std::ofstream outputFile(fileName);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Cannot create sample file " << fileName << std::endl;
        return;
    }
    
    // // Sample general graph with self-loops and multiple edges
    // outputFile << "5 15" << std::endl;
    // outputFile << "0 0" << std::endl;  // self-loop
    // outputFile << "0 1" << std::endl;
    // outputFile << "0 1" << std::endl;  // multiple edge
    // outputFile << "0 2" << std::endl;
    // outputFile << "0 2" << std::endl;  // multiple edge
    // outputFile << "0 2" << std::endl;  // multiple edge
    // outputFile << "1 1" << std::endl;  // self-loop
    // outputFile << "1 2" << std::endl;
    // outputFile << "1 3" << std::endl;
    // outputFile << "2 2" << std::endl;  // self-loop
    // outputFile << "2 4" << std::endl;
    // outputFile << "3 3" << std::endl;  // self-loop
    // outputFile << "3 4" << std::endl;
    // outputFile << "3 4" << std::endl;  // multiple edge
    // outputFile << "4 4" << std::endl;  // self-loop
    
    outputFile.close();
    std::cout << "Sample input file created: " << fileName << std::endl;
}

/**
 * @brief Demonstrate all 12 conversion functions between graph representations
 */
void demonstrateGraphRepresentationConversions() {
    const std::string INPUT_FILE_NAME = "input.txt";
    
    std::cout << "=== Complete General Graph Representation Conversion Demo ===" << std::endl;
    std::cout << "Demonstrating all 12 possible conversions between 4 representations" << std::endl;
    std::cout << std::endl;
    
    // Read initial data
    std::cout << "Reading initial graph from edge list format..." << std::endl;
    AdjacencyMatrix originalMatrix = readAdjacencyMatrixFromEdgeList(INPUT_FILE_NAME);
    displayAdjacencyMatrix(originalMatrix);
    
    // ===== FROM MATRIX =====
    std::cout << "=== CONVERSIONS FROM ADJACENCY MATRIX ===" << std::endl;
    
    std::cout << "1. Matrix -> List:" << std::endl;
    AdjacencyList listFromMatrix = convertMatrixToAdjacencyList(
        originalMatrix.matrixData, originalMatrix.numberOfVertices
    );
    displayAdjacencyList(listFromMatrix);
    
    std::cout << "2. Matrix -> Extended List:" << std::endl;
    ExtendedAdjacencyList extendedFromMatrix = convertMatrixToExtendedAdjacencyList(
        originalMatrix.matrixData, originalMatrix.numberOfVertices
    );
    displayExtendedAdjacencyList(extendedFromMatrix);
    
    std::cout << "3. Matrix -> Map:" << std::endl;
    AdjacencyMap mapFromMatrix = convertMatrixToAdjacencyMap(
        originalMatrix.matrixData, originalMatrix.numberOfVertices
    );
    displayAdjacencyMap(mapFromMatrix);
    
    // ===== FROM LIST =====
    std::cout << "=== CONVERSIONS FROM ADJACENCY LIST ===" << std::endl;
    
    std::cout << "4. List -> Matrix:" << std::endl;
    AdjacencyMatrix matrixFromList = convertAdjacencyListToMatrix(
        listFromMatrix.adjacencyData, listFromMatrix.numberOfVertices
    );
    displayAdjacencyMatrix(matrixFromList);
    
    std::cout << "5. List -> Extended List:" << std::endl;
    ExtendedAdjacencyList extendedFromList = convertAdjacencyListToExtended(
        listFromMatrix.adjacencyData, listFromMatrix.numberOfVertices
    );
    displayExtendedAdjacencyList(extendedFromList);
    
    std::cout << "6. List -> Map:" << std::endl;
    AdjacencyMap mapFromList = convertAdjacencyListToMap(
        listFromMatrix.adjacencyData, listFromMatrix.numberOfVertices
    );
    displayAdjacencyMap(mapFromList);
    
    // ===== FROM EXTENDED LIST =====
    std::cout << "=== CONVERSIONS FROM EXTENDED ADJACENCY LIST ===" << std::endl;
    
    std::cout << "7. Extended List -> Matrix:" << std::endl;
    AdjacencyMatrix matrixFromExtended = convertExtendedAdjacencyListToMatrix(
        extendedFromMatrix.edgeInstances, extendedFromMatrix.numberOfVertices
    );
    displayAdjacencyMatrix(matrixFromExtended);
    
    std::cout << "8. Extended List -> List:" << std::endl;
    AdjacencyList listFromExtended = convertExtendedAdjacencyListToList(
        extendedFromMatrix.outgoingEdgeIndices, extendedFromMatrix.edgeInstances, extendedFromMatrix.numberOfVertices
    );
    displayAdjacencyList(listFromExtended);
    
    std::cout << "9. Extended List -> Map:" << std::endl;
    AdjacencyMap mapFromExtended = convertExtendedListToMap(
        extendedFromMatrix.outgoingEdgeIndices,
        extendedFromMatrix.incomingEdgeIndices,
        extendedFromMatrix.edgeInstances,
        extendedFromMatrix.numberOfVertices,
        extendedFromMatrix.numberOfEdges
    );
    displayAdjacencyMap(mapFromExtended);
    
    // ===== FROM MAP =====
    std::cout << "=== CONVERSIONS FROM ADJACENCY MAP ===" << std::endl;
    
    std::cout << "10. Map -> Matrix:" << std::endl;
    AdjacencyMatrix matrixFromMap = convertAdjacencyMapToMatrix(
        mapFromMatrix.outgoingConnections, mapFromMatrix.numberOfVertices
    );
    displayAdjacencyMatrix(matrixFromMap);
    
    std::cout << "11. Map -> List:" << std::endl;
    AdjacencyList listFromMap = convertAdjacencyMapToList(
        mapFromMatrix.outgoingConnections, mapFromMatrix.numberOfVertices
    );
    displayAdjacencyList(listFromMap);
    
    std::cout << "12. Map -> Extended List:" << std::endl;
    ExtendedAdjacencyList extendedFromMap = convertAdjacencyMapToExtended(
        mapFromMatrix.outgoingConnections,
        mapFromMatrix.incomingConnections,
        mapFromMatrix.numberOfVertices,
        mapFromMatrix.numberOfEdges
    );
    displayExtendedAdjacencyList(extendedFromMap);
    
    // ===== WRITE OUTPUT FILES =====
    std::cout << "=== WRITING OUTPUT FILES ===" << std::endl;
    writeAdjacencyMatrixToFile(originalMatrix, "output_matrix.txt");
    writeAdjacencyListToFile(listFromMatrix, "output_list.txt");
    writeExtendedAdjacencyListToFile(extendedFromMatrix, "output_extended.txt");
    writeAdjacencyMapToFile(mapFromMatrix, "output_map.txt");
    
    std::cout << "All output files created successfully!" << std::endl;
    std::cout << "- output_matrix.txt" << std::endl;
    std::cout << "- output_list.txt" << std::endl;
    std::cout << "- output_extended.txt" << std::endl;
    std::cout << "- output_map.txt" << std::endl;
    std::cout << std::endl;
    
    // ===== STATISTICS =====
    std::cout << "=== FINAL STATISTICS ===" << std::endl;
    std::cout << "Number of vertices: " << originalMatrix.numberOfVertices << std::endl;
    std::cout << "Number of edges: " << countTotalEdgesInAdjacencyList(listFromMatrix) << std::endl;
    std::cout << "Self-loops: Supported" << std::endl;
    std::cout << "Multiple edges: Supported" << std::endl;
    std::cout << "Graph type: General Graph" << std::endl;
    std::cout << "All 12 conversions completed successfully!" << std::endl;
}

/**
 * @brief Test conversion functions with matrix input format
 */
void testMatrixInputFormat() {
    std::cout << "\n=== Testing Matrix Input Format ===" << std::endl;
    
    // Create matrix format input
    std::ofstream matrixFile("matrix_input.txt");
    matrixFile << "4" << std::endl;
    matrixFile << "0 1 1 0" << std::endl;
    matrixFile << "0 0 0 1" << std::endl;
    matrixFile << "0 0 0 0" << std::endl;
    matrixFile << "1 0 0 0" << std::endl;
    matrixFile.close();
    
    AdjacencyMatrix matrixFromFile = readAdjacencyMatrixFromFile("matrix_input.txt");
    std::cout << "Matrix read from file:" << std::endl;
    displayAdjacencyMatrix(matrixFromFile);
    
    AdjacencyList listFromMatrix = convertMatrixToAdjacencyList(
        matrixFromFile.matrixData, matrixFromFile.numberOfVertices
    );
    std::cout << "Converted to Adjacency List:" << std::endl;
    displayAdjacencyList(listFromMatrix);
}

/**
 * @brief Main program entry point
 * @return Program exit status
 */
int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    try {
        demonstrateGraphRepresentationConversions();
        testMatrixInputFormat();
        
        std::cout << "=== General Graph Representation Demo Completed Successfully ===" << std::endl;
        
    } catch (const std::exception& exception) {
        std::cerr << "Error occurred: " << exception.what() << std::endl;
        return 1;
    }
    
    return 0;
} 