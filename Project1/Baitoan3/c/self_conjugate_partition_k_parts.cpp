#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <tuple>

namespace core {
    /**
     * @brief Recursive helper for p_k^{selfcjg}(n) with memoization.
     *        Counts the number of self-conjugate partitions of n with exactly k parts.
     *        A self-conjugate partition with k parts must have largest part k, and be symmetric.
     *        The Ferrers diagram is symmetric, so the partition is determined by the number of odd-length hooks.
     *        The construction: For k odd, the middle row is odd, for k even, all rows are paired.
     *        The recurrence: p_k^{selfcjg}(n) = p_{k-2}^{selfcjg}(n-2k+1) + p_{k-1}^{selfcjg}(n-k)
     *        (see e.g. Andrews, The Theory of Partitions, Theorem 2.7.1)
     *
     * @param n The integer to partition
     * @param k The number of parts
     * @param memo Memoization table
     * @return The number of self-conjugate partitions of n with k parts
     */
    int countSelfConjugatePartitionsRecursive(int n, int k, std::vector<std::vector<int>>& memo) {
        if (k < 0 || n < 0) return 0;
        if (k == 0 && n == 0) return 1;
        if (k == 0 || n == 0) return 0;
        if (memo[n][k] != -1) return memo[n][k];
        int result = 0;
        // Case 1: Remove the outermost "hook" (length 2k-1), reduces n by 2k-1, k by 2
        if (n - (2 * k - 1) >= 0 && k >= 2) {
            result += countSelfConjugatePartitionsRecursive(n - (2 * k - 1), k - 2, memo);
        }
        // Case 2: Remove the central row (length k), reduces n by k, k by 1
        if (n - k >= 0 && k >= 1) {
            result += countSelfConjugatePartitionsRecursive(n - k, k - 1, memo);
        }
        memo[n][k] = result;
        return result;
    }
    /**
     * @brief Dynamic programming for p_k^{selfcjg}(n)
     * @param n The integer to partition
     * @param k The number of parts
     * @return The number of self-conjugate partitions of n with k parts
     */
    int countSelfConjugatePartitionsDP(int n, int k) {
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(k + 1, 0));
        dp[0][0] = 1;
        for (int total = 1; total <= n; ++total) {
            for (int parts = 1; parts <= k; ++parts) {
                // Case 1: Remove the outermost hook (length 2*parts-1)
                if (total - (2 * parts - 1) >= 0 && parts >= 2) {
                    dp[total][parts] += dp[total - (2 * parts - 1)][parts - 2];
                }
                // Case 2: Remove the central row (length parts)
                if (total - parts >= 0 && parts >= 1) {
                    dp[total][parts] += dp[total - parts][parts - 1];
                }
            }
        }
        return dp[n][k];
    }
} // namespace core

int main() {
    int n = 0, k = 0;
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
    std::vector<std::vector<int>> memo(n + 1, std::vector<int>(k + 1, -1));
    int recursiveResult = core::countSelfConjugatePartitionsRecursive(n, k, memo);
    int dpResult = core::countSelfConjugatePartitionsDP(n, k);
    std::cout << "Number of self-conjugate partitions of " << n << " with exactly " << k << " parts (recursive): " << recursiveResult << std::endl;
    std::cout << "Number of self-conjugate partitions of " << n << " with exactly " << k << " parts (DP): " << dpResult << std::endl;
    if (recursiveResult == dpResult) {
        std::cout << "Results are equal." << std::endl;
    } else {
        std::cout << "Results are different!" << std::endl;
    }
    return 0;
} 