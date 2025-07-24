# breadth_first_search.py
"""
Breadth-First Search (BFS) for a simple undirected graph
This program performs BFS on a given graph from a source vertex, computes shortest distances and predecessors.
Reads input from 'input.txt' in the same directory.
"""
from collections import deque
import sys

WHITE, GRAY, BLACK = 0, 1, 2

class Vertex:
    def __init__(self, id):
        self.id = id
        self.color = WHITE
        self.distance = float('inf')
        self.parent = None

class Graph:
    def __init__(self, n):
        self.n = n
        self.adj = [[] for _ in range(n)]
        self.vertices = [Vertex(i) for i in range(n)]

    def add_edge(self, u, v):
        self.adj[u].append(v)
        self.adj[v].append(u)

    def bfs(self, s):
        for v in self.vertices:
            v.color = WHITE
            v.distance = float('inf')
            v.parent = None
        src = self.vertices[s]
        src.color = GRAY
        src.distance = 0
        src.parent = None
        q = deque([src.id])
        while q:
            u = self.vertices[q.popleft()]
            for v_id in self.adj[u.id]:
                v = self.vertices[v_id]
                if v.color == WHITE:
                    v.color = GRAY
                    v.distance = u.distance + 1
                    v.parent = u.id
                    q.append(v.id)
            u.color = BLACK

    def print_bfs_result(self):
        print("Vertex | Distance | Predecessor")
        print("-------|----------|------------")
        for v in self.vertices:
            print(f"{v.id+1}\t | ", end='')
            if v.distance == float('inf'):
                print("INF", end='')
            else:
                print(f"{int(v.distance)}", end='')
            print("\t   | ", end='')
            if v.parent is None:
                print("NIL")
            else:
                print(f"{v.parent+1}")

def read_graph_from_file(filename):
    try:
        with open(filename, 'r') as f:
            n, m, s = map(int, f.readline().split())
            g = Graph(n)
            for _ in range(m):
                u, v = map(int, f.readline().split())
                g.add_edge(u-1, v-1)
            return g, s-1
    except Exception as e:
        print(f"Error reading input: {e}")
        sys.exit(1)

def main():
    g, s = read_graph_from_file('input.txt')
    g.bfs(s)
    g.print_bfs_result()

if __name__ == "__main__":
    main() 