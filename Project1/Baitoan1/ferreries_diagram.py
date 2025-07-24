# ferreries_diagram.py

def max_part(partition):
    return max(partition) if partition else 0

def print_ferrers(partition):
    print("F:")
    pmax = max_part(partition)
    for r in partition:
        print("*" * r + " " * (pmax - r) + f" {r}")

def print_ferrers_transpose(partition):
    print("FT:")
    pmax = max_part(partition)
    n = len(partition)
    for r in range(pmax):
        row = ''
        for i in range(n):
            row += "* " if partition[i] > r else "  "
        print(row.rstrip())
    print(' '.join(str(x) for x in partition))

def generate_partitions(n, k, current, max_val):
    if k == 0:
        if n == 0:
            print_ferrers(current)
            print_ferrers_transpose(current)
            print("====================")
        return
    for i in range(min(n, max_val), 0, -1):
        current.append(i)
        generate_partitions(n - i, k - 1, current, i)
        current.pop()

def main():
    n, k = map(int, input().split())
    generate_partitions(n, k, [], n)

if __name__ == "__main__":
    main() 