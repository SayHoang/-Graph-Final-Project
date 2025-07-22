# bfs_multigraph.py
"""
Breadth-First Search (BFS) for a multigraph (đa đồ thị)
Reads input from 'input.txt' in the same directory.
Vertex labels are strings (e.g., 'A', 'B', ...).
"""
from collections import deque
import sys

WHITE, GRAY, BLACK = 0, 1, 2

class Vertex:
    def __init__(self, label):
        self.label = label
        self.color = WHITE
        self.distance = float('inf')
        self.parent = None

class MultiGraph:
    def __init__(self):
        self.adj = dict()  # label -> list of neighbor labels (with possible duplicates)
        self.vertices = dict()  # label -> Vertex

    def add_edge(self, u, v):
        if u not in self.adj:
            self.adj[u] = []
            self.vertices[u] = Vertex(u)
        if v not in self.adj:
            self.adj[v] = []
            self.vertices[v] = Vertex(v)
        self.adj[u].append(v)
        self.adj[v].append(u)  # undirected, add both ways

    def bfs(self, s_label):
        for v in self.vertices.values():
            v.color = WHITE
            v.distance = float('inf')
            v.parent = None
        src = self.vertices[s_label]
        src.color = GRAY
        src.distance = 0
        src.parent = None
        q = deque([src.label])
        while q:
            u_label = q.popleft()
            u = self.vertices[u_label]
            for v_label in self.adj[u_label]:
                v = self.vertices[v_label]
                if v.color == WHITE:
                    v.color = GRAY
                    v.distance = u.distance + 1
                    v.parent = u.label
                    q.append(v.label)
            u.color = BLACK

    def print_bfs_result(self):
        print("Vertex | Distance | Predecessor")
        print("-------|----------|------------")
        for label in sorted(self.vertices.keys()):
            v = self.vertices[label]
            print(f"{label}\t | ", end='')
            if v.distance == float('inf'):
                print("INF", end='')
            else:
                print(f"{int(v.distance)}", end='')
            print("\t   | ", end='')
            if v.parent is None:
                print("NIL")
            else:
                print(f"{v.parent}")

def read_multigraph_from_file(filename):
    try:
        with open(filename, 'r') as f:
            lines = [line.strip() for line in f if line.strip() and not line.startswith('#')]
            n, m, s = lines[0].split()
            n, m = int(n), int(m)
            g = MultiGraph()
            for i in range(1, m+1):
                u, v = lines[i].split()
                g.add_edge(u, v)
            return g, s
    except Exception as e:
        print(f"Error reading input: {e}")
        sys.exit(1)

def main():
    g, s = read_multigraph_from_file('input.txt')
    g.bfs(s)
    g.print_bfs_result()

if __name__ == "__main__":
    main() 