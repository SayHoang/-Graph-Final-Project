#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstddef>

namespace core {
    /**
     * @brief Generate all integer partitions of n into exactly k parts (non-increasing order)
     * @param n The integer to partition
     * @param k The number of parts
     * @param result The list to store resulting partitions
     * @param currentPartition The current partition (for recursion)
     * @param maxVal The maximum value allowed for the current part
     */
    void generatePartitionsWithKParts(int n, int k, std::vector<std::vector<int>>& result, std::vector<int>& currentPartition, int maxVal) {
        if (k == 0) {
            if (n == 0) {
                result.push_back(currentPartition);
            }
            return;
        }
        if (n <= 0 || k <= 0) return;
        if (n < k) return;
        const int upperBound = std::min(maxVal, n - (k - 1));
        for (int val = upperBound; val >= 1; --val) {
            currentPartition.push_back(val);
            generatePartitionsWithKParts(n - val, k - 1, result, currentPartition, val);
            currentPartition.pop_back();
        }
    }
    /**
     * @brief Check if a partition is self-conjugate
     * @param partition The partition (non-increasing order)
     * @return true if self-conjugate, false otherwise
     */
    bool isSelfConjugate(const std::vector<int>& partition) {
        if (partition.empty()) return true;
        const int maxVal = partition[0];
        std::vector<int> conjugateParts;
        for (int j = 1; j <= maxVal; ++j) {
            int count = 0;
            for (int partVal : partition) {
                if (partVal >= j) ++count;
                else break;
            }
            if (count > 0) conjugateParts.push_back(count);
            else break;
        }
        return partition == conjugateParts;
    }
} // namespace core

namespace utils {
    /**
     * @brief Print a partition in the format [a, b, c]
     * @param partition The partition to print
     */
    void printPartition(const std::vector<int>& partition) {
        std::cout << "[";
        for (std::size_t i = 0; i < partition.size(); ++i) {
            std::cout << partition[i];
            if (i + 1 < partition.size()) std::cout << ", ";
        }
        std::cout << "]\n";
    }
} // namespace utils

int main() {
    int n = 0;
    int k = 0;
    std::ifstream fin("input.txt");
    if (!fin) {
        std::cerr << "Error: Cannot open input.txt\n";
        return 1;
    }
    fin >> n >> k;
    fin.close();
    if (n < 0 || k < 0) {
        std::cout << "n and k must be non-negative integers.\n";
        return 0;
    }
    if (k > n && n > 0) {
        std::cout << "No partition of " << n << " into " << k << " parts.\n";
        return 0;
    }
    if (k == 0 && n > 0) {
        std::cout << "No partition of " << n << " into " << k << " parts.\n";
        return 0;
    }
    if (k == 0 && n == 0) {
        std::cout << "Number of self-conjugate partitions of 0 into 0 parts: 1\n";
        std::cout << "Partitions: [[]]\n";
        return 0;
    }
    std::vector<std::vector<int>> allKPartitions;
    std::vector<int> currentPartition;
    core::generatePartitionsWithKParts(n, k, allKPartitions, currentPartition, n);
    std::vector<std::vector<int>> selfConjugateKPartitions;
    for (const auto& partition : allKPartitions) {
        if (core::isSelfConjugate(partition)) {
            selfConjugateKPartitions.push_back(partition);
        }
    }
    std::cout << "Number of self-conjugate partitions of " << n << " into " << k << " parts: " << selfConjugateKPartitions.size() << "\n";
    std::cout << "Partitions:\n";
    if (selfConjugateKPartitions.empty()) {
        std::cout << "None.\n";
    } else {
        for (const auto& partition : selfConjugateKPartitions) {
            utils::printPartition(partition);
        }
    }
    return 0;
} 