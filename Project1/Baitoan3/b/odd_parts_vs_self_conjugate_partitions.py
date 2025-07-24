import sys
import os

def count_odd_distinct_partitions(n):
    """
    Count the number of partitions of n into an odd number of distinct parts.
    """
    # dp[i][j]: number of partitions of i into j distinct parts
    dp = [[0] * (n + 1) for _ in range(n + 1)]
    dp[0][0] = 1
    for part in range(1, n + 1):
        for total in range(n, part - 1, -1):
            for cnt in range(1, n + 1):
                dp[total][cnt] += dp[total - part][cnt - 1]
    result = 0
    for cnt in range(1, n + 1, 2):
        result += dp[n][cnt]
    return result

def count_distinct_odd_partitions(n):
    """
    Count the number of partitions of n into distinct odd parts (self-conjugate partitions).
    """
    # dp[i][j]: number of partitions of i into distinct odd parts, max part <= j
    dp = [[0] * (n + 1) for _ in range(n + 1)]
    dp[0][0] = 1
    for max_part in range(1, n + 1, 2):
        if max_part <= n:
            dp[max_part][max_part] = 1
    for total in range(1, n + 1):
        for max_part in range(1, n + 1, 2):
            if total - max_part >= 0:
                for prev in range(max_part - 2, -1, -2):
                    dp[total][max_part] += dp[total - max_part][prev]
    result = 0
    for max_part in range(1, n + 1, 2):
        result += dp[n][max_part]
    return result

def main():
    # Read n from input.txt
    try:
        with open("input.txt", "r", encoding="utf-8") as f:
            n = int(f.read().strip())
    except Exception as e:
        print("Error reading input.txt:", e)
        sys.exit(1)
    if n < 0:
        print("n must be a non-negative integer.")
        return
    odd_distinct = count_odd_distinct_partitions(n)
    self_conjugate = count_distinct_odd_partitions(n)
    print(f"Number of partitions of {n} into an odd number of distinct parts: {odd_distinct}")
    print(f"Number of self-conjugate partitions of {n} (partitions into distinct odd parts): {self_conjugate}")
    if odd_distinct == self_conjugate:
        print("They are equal.")
    else:
        print("They are different.")

if __name__ == "__main__":
    main() 