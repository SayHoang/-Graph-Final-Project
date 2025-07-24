import sys
import os
from functools import lru_cache

def count_self_conjugate_partitions_recursive(n, k, memo=None):
    """
    Recursively compute the number of self-conjugate partitions of n with exactly k parts (with memoization).
    Recurrence: p_k^{selfcjg}(n) = p_{k-2}^{selfcjg}(n-2k+1) + p_{k-1}^{selfcjg}(n-k)
    Base cases:
        - If k < 0 or n < 0: return 0
        - If k == 0 and n == 0: return 1
        - If k == 0 or n == 0: return 0
    """
    if memo is None:
        memo = {}
    if (n, k) in memo:
        return memo[(n, k)]
    if k < 0 or n < 0:
        return 0
    if k == 0 and n == 0:
        return 1
    if k == 0 or n == 0:
        return 0
    result = 0
    if n - (2 * k - 1) >= 0 and k >= 2:
        result += count_self_conjugate_partitions_recursive(n - (2 * k - 1), k - 2, memo)
    if n - k >= 0 and k >= 1:
        result += count_self_conjugate_partitions_recursive(n - k, k - 1, memo)
    memo[(n, k)] = result
    return result

def count_self_conjugate_partitions_dp(n, k):
    """
    Dynamic programming for the number of self-conjugate partitions of n with exactly k parts.
    """
    dp = [[0] * (k + 1) for _ in range(n + 1)]
    dp[0][0] = 1
    for total in range(1, n + 1):
        for parts in range(1, k + 1):
            # Case 1: Remove the outermost hook (length 2*parts-1)
            if total - (2 * parts - 1) >= 0 and parts >= 2:
                dp[total][parts] += dp[total - (2 * parts - 1)][parts - 2]
            # Case 2: Remove the central row (length parts)
            if total - parts >= 0 and parts >= 1:
                dp[total][parts] += dp[total - parts][parts - 1]
    return dp[n][k]

def main():
    # Read n, k from input.txt
    try:
        with open("input.txt", "r", encoding="utf-8") as f:
            tokens = f.read().split()
            n = int(tokens[0])
            k = int(tokens[1])
    except Exception as e:
        print("Error reading input.txt:", e)
        sys.exit(1)
    if n < 0 or k < 0:
        print("n and k must be non-negative integers.")
        return
    recursive_result = count_self_conjugate_partitions_recursive(n, k)
    dp_result = count_self_conjugate_partitions_dp(n, k)
    print(f"Number of self-conjugate partitions of {n} with exactly {k} parts (recursive): {recursive_result}")
    print(f"Number of self-conjugate partitions of {n} with exactly {k} parts (DP): {dp_result}")
    if recursive_result == dp_result:
        print("Results are equal.")
    else:
        print("Results are different!")

if __name__ == "__main__":
    main() 