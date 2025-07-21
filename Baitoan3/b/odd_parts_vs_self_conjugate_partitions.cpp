#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <tuple>

namespace core {
    /**
     * @brief Check if n is a generalized pentagonal number and return k if so
     * @param n The number to check
     * @return std::pair<bool, int> (is_pentagonal, k_val)
     */
    std::pair<bool, int> isGeneralizedPentagonal(int n) {
        if (n <= 0) return {false, 0};
        int val = static_cast<int>(std::sqrt(24 * n + 1));
        if (val * val != 24 * n + 1) return {false, 0};
        if ((val + 1) % 6 == 0) return {true, (val + 1) / 6};
        if ((val - 1) % 6 == 0) return {true, (val - 1) / 6};
        return {false, 0};
    }
    /**
     * @brief Compute the number of partitions of n into distinct parts (p_d(n))
     * @param n The integer to partition
     * @return The number of partitions into distinct parts
     */
    int countDistinctPartitions(int n) {
        std::vector<int> dp(n + 1, 0);
        dp[0] = 1;
        for (int i = 1; i <= n; ++i) {
            for (int j = n; j >= i; --j) {
                dp[j] += dp[j - i];
            }
        }
        return dp[n];
    }
    /**
     * @brief Compute the number of partitions of n into an odd number of distinct parts (|O(n)|)
     * @param n The integer to partition
     * @return The number of partitions into an odd number of distinct parts
     */
    int countOddDistinctPartitions(int n) {
        // dp[i][j]: number of partitions of i into j distinct parts
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));
        dp[0][0] = 1;
        for (int part = 1; part <= n; ++part) {
            for (int sum = n; sum >= part; --sum) {
                for (int cnt = 1; cnt <= n; ++cnt) {
                    dp[sum][cnt] += dp[sum - part][cnt - 1];
                }
            }
        }
        int result = 0;
        for (int cnt = 1; cnt <= n; cnt += 2) {
            result += dp[n][cnt];
        }
        return result;
    }
    /**
     * @brief Compute the number of partitions of n into distinct odd parts (self-conjugate partitions)
     * @param n The integer to partition
     * @return The number of partitions into distinct odd parts
     */
    int countDistinctOddPartitions(int n) {
        // dp[i][j]: number of partitions of i into distinct odd parts, max part <= j
        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));
        for (int j = 1; j <= n; j += 2) {
            if (j <= n) dp[j][j] = 1;
        }
        dp[0][0] = 1;
        for (int sum = 1; sum <= n; ++sum) {
            for (int maxPart = 1; maxPart <= n; maxPart += 2) {
                if (sum - maxPart >= 0) {
                    for (int prev = maxPart - 2; prev >= 0; prev -= 2) {
                        dp[sum][maxPart] += dp[sum - maxPart][prev];
                    }
                }
            }
        }
        int result = 0;
        for (int maxPart = 1; maxPart <= n; maxPart += 2) {
            result += dp[n][maxPart];
        }
        return result;
    }
} // namespace core

int main() {
    int n = 0;
    std::ifstream fin("input.txt");
    if (!fin) {
        std::cerr << "Error: Cannot open input.txt\n";
        return 1;
    }
    fin >> n;
    fin.close();
    if (n < 0) {
        std::cout << "n must be a non-negative integer.\n";
        return 0;
    }
    int oddDistinct = core::countOddDistinctPartitions(n);
    int selfConjugate = core::countDistinctOddPartitions(n);
    std::cout << "Number of partitions of " << n << " into an odd number of distinct parts: " << oddDistinct << std::endl;
    std::cout << "Number of self-conjugate partitions of " << n << " (partitions into distinct odd parts): " << selfConjugate << std::endl;
    if (oddDistinct == selfConjugate) {
        std::cout << "They are equal." << std::endl;
    } else {
        std::cout << "They are different." << std::endl;
    }
    return 0;
} 