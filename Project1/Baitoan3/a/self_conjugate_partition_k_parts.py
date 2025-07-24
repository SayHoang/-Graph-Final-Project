import sys

def generate_partitions_k_parts(n, k):
    partitions = []
    current_partition = []
    def _generate(remaining_sum, num_parts_left, max_val_for_this_part):
        if num_parts_left == 0:
            if remaining_sum == 0:
                partitions.append(list(current_partition))
            return
        if remaining_sum <= 0 or num_parts_left <= 0:
            return
        if remaining_sum < num_parts_left:
            return
        upper_bound = min(max_val_for_this_part, remaining_sum - (num_parts_left - 1))
        for val in range(upper_bound, 0, -1):
            current_partition.append(val)
            _generate(remaining_sum - val, num_parts_left - 1, val)
            current_partition.pop()
    _generate(n, k, n)
    return partitions

def is_self_conjugate(partition):
    if not partition:
        return True
    max_val = partition[0]
    conjugate_parts = []
    for j in range(1, max_val + 1):
        count = 0
        for part_val in partition:
            if part_val >= j:
                count += 1
            else:
                break
        if count > 0:
            conjugate_parts.append(count)
        else:
            break
    return partition == conjugate_parts

def main():
    # Read input from input.txt
    try:
        with open("input.txt", "r", encoding="utf-8") as f:
            lines = f.read().split()
            n = int(lines[0])
            k = int(lines[1])
    except Exception as e:
        print("Error reading input.txt:", e)
        sys.exit(1)
    if n < 0 or k < 0:
        print("n and k must be non-negative integers.")
        return
    if k > n and n > 0:
        print(f"No partition of {n} into {k} parts.")
        return
    if k == 0 and n > 0:
        print(f"No partition of {n} into {k} parts.")
        return
    if k == 0 and n == 0:
        print("Number of self-conjugate partitions of 0 into 0 parts: 1")
        print("Partitions: [[]]")
        return
    all_k_partitions = generate_partitions_k_parts(n, k)
    self_conjugate_k_partitions = []
    for p in all_k_partitions:
        if is_self_conjugate(p):
            self_conjugate_k_partitions.append(p)
    print(f"Number of self-conjugate partitions of {n} into {k} parts: {len(self_conjugate_k_partitions)}")
    print("Partitions:")
    if not self_conjugate_k_partitions:
        print("None.")
    else:
        for p in self_conjugate_k_partitions:
            print(p)

if __name__ == "__main__":
    main() 