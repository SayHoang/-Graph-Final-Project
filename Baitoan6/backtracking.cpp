#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <climits>

using namespace std;

// Tree node structure
struct TreeNode {
    string label;
    int depth;
    int order;
    TreeNode* parent;
    vector<TreeNode*> children;
    
    TreeNode(string l = "") : label(l), depth(0), order(0), parent(nullptr) {}
};

// Tree class
class Tree {
public:
    TreeNode* root;
    vector<TreeNode*> nodes;
    TreeNode* dummy; // dummy node for deletion operations
    
    Tree() : root(nullptr), dummy(nullptr) {}
    
    void addNode(string label) {
        TreeNode* node = new TreeNode(label);
        nodes.push_back(node);
        if (!root) root = node;
    }
    
    void addEdge(string parentLabel, string childLabel) {
        TreeNode* parent = findNode(parentLabel);
        TreeNode* child = findNode(childLabel);
        if (parent && child) {
            parent->children.push_back(child);
            child->parent = parent;
        }
    }
    
    TreeNode* findNode(string label) {
        for (TreeNode* node : nodes) {
            if (node->label == label) return node;
        }
        return nullptr;
    }
    
    bool is_root(TreeNode* node) {
        return node == root;
    }
    
    string label(TreeNode* node) {
        return node ? node->label : "";
    }
    
    TreeNode* parent(TreeNode* node) {
        return node ? node->parent : nullptr;
    }
    
    void clear() {
        for (TreeNode* node : nodes) {
            delete node;
        }
        nodes.clear();
        root = nullptr;
        dummy = nullptr;
    }
    
    ~Tree() {
        clear();
    }
};

// Global variables for trees
Tree T1, T2;
// Function to perform preorder traversal and set order
void preorder_tree_traversal(Tree& tree) {
    if (!tree.root) return;
    
    int order = 0;
    vector<TreeNode*> stack;
    stack.push_back(tree.root);
    
    while (!stack.empty()) {
        TreeNode* current = stack.back();
        stack.pop_back();
        
        current->order = order++;
        
        // Push children in reverse order to maintain preorder
        for (int i = current->children.size() - 1; i >= 0; i--) {
            stack.push_back(current->children[i]);
        }
    }
}

// Function to calculate depth for all nodes
void preorder_tree_depth(Tree& tree) {
    if (!tree.root) return;
    
    vector<pair<TreeNode*, int>> stack;
    stack.push_back({tree.root, 0});
    
    while (!stack.empty()) {
        TreeNode* current = stack.back().first;
        int depth = stack.back().second;
        stack.pop_back();
        
        current->depth = depth;
        
        for (TreeNode* child : current->children) {
            stack.push_back({child, depth + 1});
        }
    }
}

// Function to set up candidate nodes
map<TreeNode*, vector<TreeNode*>> set_up_candidate_nodes(Tree& T1, Tree& T2) {
    preorder_tree_depth(T1);
    preorder_tree_depth(T2);
    // Add dummy node to T2
    T2.dummy = new TreeNode("dummy");
    T2.dummy->depth = -1; // Special depth for dummy
    T2.nodes.push_back(T2.dummy);
    map<TreeNode*, vector<TreeNode*>> C;
    // Initialize candidate nodes for each node in T1
    for (TreeNode* v : T1.nodes) {
        C[v] = vector<TreeNode*>();
        C[v].push_back(T2.dummy); // Always include dummy for deletion
        // Add nodes from T2 with same depth
        for (TreeNode* w : T2.nodes) {
            if (w != T2.dummy && v->depth == w->depth) {
                C[v].push_back(w);
            }
        }
    }
    return C;
}

// Function to refine candidate nodes
void refine_candidate_nodes(map<TreeNode*, vector<TreeNode*>>& C, TreeNode* v, TreeNode* w) {
    if (w == T2.dummy) { // If v is mapped to dummy, remove dummy from all other candidates
        for (auto& pair : C) {
            TreeNode* x = pair.first;
            if (x != v) {
                auto& candidates = pair.second;
                candidates.erase(remove(candidates.begin(), candidates.end(), w), candidates.end());
            }
        }
        // For children of v, they must also be mapped to dummy
        for (TreeNode* child : v->children) {
            auto& candidates = C[child];
            candidates.erase(remove_if(candidates.begin(), candidates.end(),
                [w](TreeNode* y) { return y != T2.dummy; }), candidates.end());
        }
        // Handle sibling order constraints
        if (!T1.is_root(v) && T2.label(w) != T2.parent(v)->label) {
            for (TreeNode* sibling : T1.parent(v)->children) {
                if (v->order < sibling->order) {
                    auto& candidates = C[sibling];
                    candidates.erase(remove_if(candidates.begin(), candidates.end(),
                        [w](TreeNode* y) { return y != T2.dummy && w->order > y->order; }), candidates.end());
                }
            }
        }
    } else {
        // Remove w from all other candidates (bijective constraint)
        for (auto& pair : C) {
            TreeNode* x = pair.first;
            if (x != v) {
                auto& candidates = pair.second;
                candidates.erase(remove(candidates.begin(), candidates.end(), w), candidates.end());
            }
        }
        // Parent-child relation constraint
        for (TreeNode* child : v->children) {
            auto& candidates = C[child];
            candidates.erase(remove_if(candidates.begin(), candidates.end(),
                [w](TreeNode* y) { return y != T2.dummy && T2.parent(y) != w; }), candidates.end());
        }
        // Sibling order constraint
        if (!T1.is_root(v) && T2.label(w) != T2.parent(v)->label) {
            for (TreeNode* sibling : T1.parent(v)->children) {
                if (v->order < sibling->order) {
                    auto& candidates = C[sibling];
                    candidates.erase(remove_if(candidates.begin(), candidates.end(),
                        [w](TreeNode* y) { return y != T2.dummy && w->order > y->order; }), candidates.end());
                }
            }
        }
    }
}

// Recursive function to extend tree edit mapping
void extend_tree_edit(Tree& T1, Tree& T2, 
                     map<TreeNode*, TreeNode*>& M, 
                     vector<map<TreeNode*, TreeNode*>>& L,
                     map<TreeNode*, vector<TreeNode*>>& C, 
                     TreeNode* v) {
    for (TreeNode* w : C[v]) {
        M[v] = w; // Update mapping before recursion and solution push
        // Check if v is the last node in preorder
        bool isLast = true;
        for (TreeNode* node : T1.nodes) {
            if (node->order > v->order) {
                isLast = false;
                break;
            }
        }
        if (isLast) {
            L.push_back(M);
        } else {
            map<TreeNode*, vector<TreeNode*>> N = C;
            refine_candidate_nodes(N, v, w);
            TreeNode* next_v = nullptr;
            int min_order = INT_MAX;
            for (TreeNode* node : T1.nodes) {
                if (node->order > v->order && node->order < min_order) {
                    min_order = node->order;
                    next_v = node;
                }
            }
            if (next_v) {
                extend_tree_edit(T1, T2, M, L, N, next_v);
            }
        }
    }
}

// Main backtracking tree edit function
vector<map<TreeNode*, TreeNode*>> backtracking_tree_edit(Tree& T1, Tree& T2) {
    preorder_tree_traversal(T1);
    preorder_tree_traversal(T2);
    map<TreeNode*, TreeNode*> M; // Current mapping
    vector<map<TreeNode*, TreeNode*>> L; // List of all valid mappings
    
    map<TreeNode*, vector<TreeNode*>> C = set_up_candidate_nodes(T1, T2);
    
    // Start with first node in preorder
    TreeNode* v = T1.root;
    
    extend_tree_edit(T1, T2, M, L, C, v);
    
    return L;
}

// Function to read tree from file
Tree readTree(ifstream& file) {
    Tree tree;
    string line;
    
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        istringstream iss(line);
        string parent, child;
        iss >> parent >> child;
        
        // Add nodes if they don't exist
        if (!tree.findNode(parent)) {
            tree.addNode(parent);
        }
        if (!tree.findNode(child)) {
            tree.addNode(child);
        }
        
        tree.addEdge(parent, child);
    }
    
    return tree;
}

// Function to print mapping
void printMapping(const map<TreeNode*, TreeNode*>& mapping) {
    cout << "Mapping:" << endl;
    for (const auto& pair : mapping) {
        string v_label = pair.first->label;
        string w_label = pair.second->label;
        cout << v_label << " -> " << w_label << endl;
    }
    cout << endl;
}

// Function to calculate edit distance for a mapping
int calculateEditDistance(const map<TreeNode*, TreeNode*>& mapping) {
    int distance = 0;
    
    // Count deletions (nodes in T1 mapped to dummy)
    for (const auto& pair : mapping) {
        if (pair.second == T2.dummy) {
            distance++; // Deletion
        }
    }
    
    // Count insertions (nodes in T2 not mapped from T1)
    unordered_set<TreeNode*> mapped_T2_nodes;
    for (const auto& pair : mapping) {
        if (pair.second != T2.dummy) {
            mapped_T2_nodes.insert(pair.second);
        }
    }
    
    for (TreeNode* node : T2.nodes) {
        if (node != T2.dummy && mapped_T2_nodes.find(node) == mapped_T2_nodes.end()) {
            distance++; // Insertion
        }
    }
    
    return distance;
}

int main() {
    ifstream file("input.txt");
    if (!file.is_open()) {
        cerr << "Error opening input.txt" << endl;
        return 1;
    }
    
    string line;
    bool readingT1 = true;
    
    // Read Tree 1
    while (getline(file, line)) {
        if (line.find("# Tree 2") != string::npos) {
            readingT1 = false;
            break;
        }
        if (line.empty() || line[0] == '#') continue;
        
        istringstream iss(line);
        string parent, child;
        iss >> parent >> child;
        
        if (!T1.findNode(parent)) {
            T1.addNode(parent);
        }
        if (!T1.findNode(child)) {
            T1.addNode(child);
        }
        
        T1.addEdge(parent, child);
    }
    
    // Read Tree 2
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        
        istringstream iss(line);
        string parent, child;
        iss >> parent >> child;
        
        if (!T2.findNode(parent)) {
            T2.addNode(parent);
        }
        if (!T2.findNode(child)) {
            T2.addNode(child);
        }
        
        T2.addEdge(parent, child);
    }
    
    file.close();
    
    cout << "Tree 1:" << endl;
    for (TreeNode* node : T1.nodes) {
        cout << "   " << node->label;
        if (node->children.size() > 0) {
            cout << " -> ";
            for (size_t i = 0; i < node->children.size(); i++) {
                cout << node->children[i]->label;
                if (i < node->children.size() - 1) {
                    cout << ", ";
                }
            }
        }
        cout << endl;
    }
    
    cout << "\nTree 2:" << endl;
    for (TreeNode* node : T2.nodes) {
        cout << "   " << node->label;
        if (node->children.size() > 0) {
            cout << " -> ";
            for (size_t i = 0; i < node->children.size(); i++) {
                cout << node->children[i]->label;
                if (i < node->children.size() - 1) {
                    cout << ", ";
                }
            }
        }
        cout << endl;
    }
    
    cout << "\nRunning backtracking tree edit algorithm..." << endl;
    
    vector<map<TreeNode*, TreeNode*>> solutions = backtracking_tree_edit(T1, T2);
    
    cout << "\nFound " << solutions.size() << " valid mappings:" << endl;
    
    if (solutions.empty()) {
        cout << "No valid mappings found." << endl;
    } else {
        for (size_t i = 0; i < solutions.size(); i++) {
            cout << "\nSolution " << (i + 1) << ":" << endl;
            printMapping(solutions[i]);
            
            int distance = calculateEditDistance(solutions[i]);
            cout << "Edit distance: " << distance << endl;
        }
        
        // Find minimum edit distance
        int min_distance = INT_MAX;
        for (const auto& solution : solutions) {
            int distance = calculateEditDistance(solution);
            if (distance < min_distance) {
                min_distance = distance;
            }
        }
        
        cout << "\nMinimum edit distance: " << min_distance << endl;
    }
    
    return 0;
}
