# dfs_multigraph.py
"""
Depth-First Search (DFS) for a multigraph
Supports parallel edges but not self-loops (following strict multigraph definition)
"""

from collections import defaultdict
import sys

class Vertex:
    def __init__(self, label):
        self.label = label
        self.color = 'WHITE'
        self.discovery_time = -1
        self.finish_time = -1
        self.parent = None

class Multigraph:
    def __init__(self):
        self.adjacency_list = defaultdict(list)  # allows parallel edges
        self.vertices = {}
        self.global_time = 0
    
    def add_vertex(self, label):
        if label not in self.vertices:
            self.vertices[label] = Vertex(label)
            self.adjacency_list[label] = []
    
    def add_edge(self, u, v):
        if u == v:
            print(f"Error: Self-loops not allowed in multigraph", file=sys.stderr)
            return
        
        self.add_vertex(u)
        self.add_vertex(v)
        self.adjacency_list[u].append(v)
        self.adjacency_list[v].append(u)  # undirected multigraph
    
    def initialize_vertices(self):
        for vertex in self.vertices.values():
            vertex.color = 'WHITE'
            vertex.discovery_time = -1
            vertex.finish_time = -1
            vertex.parent = None
    
    def execute_depth_first_search(self):
        """Execute recursive DFS traversal on the entire graph"""
        self.initialize_vertices()
        self.global_time = 0
        
        # Visit all vertices in sorted order for deterministic output
        sorted_labels = sorted(self.vertices.keys())
        
        for label in sorted_labels:
            if self.vertices[label].color == 'WHITE':
                self.dfs_visit_recursive(label)
    
    def dfs_visit_recursive(self, current_label):
        """Recursive DFS visit implementation"""
        current = self.vertices[current_label]
        self.global_time += 1
        current.discovery_time = self.global_time
        current.color = 'GRAY'
        
        # Visit neighbors in sorted order for deterministic output
        neighbors = sorted(self.adjacency_list[current_label])
        
        for neighbor_label in neighbors:
            neighbor = self.vertices[neighbor_label]
            if neighbor.color == 'WHITE':
                neighbor.parent = current_label
                self.dfs_visit_recursive(neighbor_label)
        
        current.color = 'BLACK'
        self.global_time += 1
        current.finish_time = self.global_time
    
    def execute_depth_first_search_iterative(self):
        """Execute iterative DFS traversal on the entire graph"""
        self.initialize_vertices()
        self.global_time = 0
        
        sorted_labels = sorted(self.vertices.keys())
        
        for label in sorted_labels:
            if self.vertices[label].color == 'WHITE':
                self.dfs_visit_iterative(label)
    
    def dfs_visit_iterative(self, start_label):
        """Iterative DFS visit implementation using explicit stack"""
        stack = [(start_label, False)]  # (vertex, is_finishing)
        
        while stack:
            current_label, is_finishing = stack.pop()
            current_vertex = self.vertices[current_label]
            
            if is_finishing:
                current_vertex.color = 'BLACK'
                self.global_time += 1
                current_vertex.finish_time = self.global_time
            else:
                if current_vertex.color == 'WHITE':
                    self.global_time += 1
                    current_vertex.discovery_time = self.global_time
                    current_vertex.color = 'GRAY'
                    
                    # Push finish marker
                    stack.append((current_label, True))
                    
                    # Visit neighbors in reverse sorted order (stack reverses order)
                    neighbors = sorted(self.adjacency_list[current_label], reverse=True)
                    
                    for neighbor_label in neighbors:
                        neighbor = self.vertices[neighbor_label]
                        if neighbor.color == 'WHITE':
                            neighbor.parent = current_label
                            stack.append((neighbor_label, False))
    
    def print_dfs_result(self):
        """Print DFS results in tabular format"""
        sorted_labels = sorted(self.vertices.keys())
        
        print("Vertex | d  | f  | Parent")
        print("-------|----|----|--------")
        
        for label in sorted_labels:
            vertex = self.vertices[label]
            parent_str = "NIL" if vertex.parent is None else vertex.parent
            print(f"{label}\t | {vertex.discovery_time} | {vertex.finish_time} | {parent_str}")
    
    def print_graph_info(self):
        """Print graph statistics"""
        vertex_count = len(self.vertices)
        edge_count = sum(len(neighbors) for neighbors in self.adjacency_list.values()) // 2
        
        print(f"Multigraph Info: {vertex_count} vertices, {edge_count} edges (including parallel edges)")

def read_multigraph_from_file(filename):
    """Read multigraph from input file"""
    try:
        with open(filename, 'r') as f:
            vertex_count, edge_count = map(int, f.readline().split())
            
            graph = Multigraph()
            
            for _ in range(edge_count):
                u, v = f.readline().strip().split()
                graph.add_edge(u, v)
            
            return graph
    except FileNotFoundError:
        print(f"Error: Cannot open {filename}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Error reading file: {e}", file=sys.stderr)
        sys.exit(1)

def main():
    graph = read_multigraph_from_file('input.txt')
    
    graph.print_graph_info()
    
    print("\n--- Recursive DFS ---")
    graph.execute_depth_first_search()
    graph.print_dfs_result()
    
    print("\n--- Iterative DFS ---")
    graph.execute_depth_first_search_iterative()
    graph.print_dfs_result()

if __name__ == "__main__":
    main()
