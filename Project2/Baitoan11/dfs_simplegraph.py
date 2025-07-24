# depth_first_search.py
"""
Depth-First Search (DFS) for a simple graph
Reads input from 'input.txt' in the same directory.
Vertex labels are strings (e.g., 'u', 'v', ...).
"""

def read_simple_graph_from_file(filename):
    adj = {}
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith('#'):
                continue
            if ':' not in line:
                continue
            u, rest = line.split(':', 1)
            u = u.strip()
            neighbors = []
            for v in rest.replace(',', ' ').split():
                neighbors.append(v.strip())
            adj.setdefault(u, []).extend([v for v in neighbors if v])
            for v in neighbors:
                if v and v not in adj:
                    adj[v] = []
    return adj

class Vertex:
    def __init__(self, label):
        self.label = label
        self.color = 'WHITE'
        self.discovery = -1
        self.finish = -1
        self.parent = None

class SimpleGraph:
    def __init__(self, adj):
        self.adj = adj
        self.vertices = {label: Vertex(label) for label in adj}
        self.time = 0

    def execute_depth_first_search(self):
        for v in self.vertices.values():
            v.color = 'WHITE'
            v.parent = None
            v.discovery = -1
            v.finish = -1
        self.time = 0
        for label in sorted(self.vertices.keys()):
            if self.vertices[label].color == 'WHITE':
                self.dfs_visit(label)

    def dfs_visit(self, u_label):
        u = self.vertices[u_label]
        self.time += 1
        u.discovery = self.time
        u.color = 'GRAY'
        for v_label in sorted(self.adj[u_label]):
            v = self.vertices[v_label]
            if v.color == 'WHITE':
                v.parent = u.label
                self.dfs_visit(v_label)
        u.color = 'BLACK'
        self.time += 1
        u.finish = self.time

    def print_dfs_result(self):
        print("Vertex | d  | f  | Parent")
        print("-------|----|----|--------")
        for label in sorted(self.vertices.keys()):
            v = self.vertices[label]
            print(f"{label}\t | {v.discovery} | {v.finish} | ", end='')
            if v.parent is None:
                print("NIL")
            else:
                print(f"{v.parent}")

def main():
    adj = read_simple_graph_from_file('input.txt')
    g = SimpleGraph(adj)
    g.execute_depth_first_search()
    g.print_dfs_result()

if __name__ == "__main__":
    main() 