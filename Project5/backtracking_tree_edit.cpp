#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

struct Node {
    string label;
    vector<Node*> children;
    Node* parent = nullptr;
    Node(string l) : label(l) {}
};

// Parse tree from edge list
Node* build_tree(const vector<pair<string, string>>& edges, string& root_label) {
    map<string, Node*> nodes;
    set<string> children;
    for (auto& e : edges) {
        if (!nodes.count(e.first)) nodes[e.first] = new Node(e.first);
        if (!nodes.count(e.second)) nodes[e.second] = new Node(e.second);
        nodes[e.first]->children.push_back(nodes[e.second]);
        nodes[e.second]->parent = nodes[e.first];
        children.insert(e.second);
    }
    // Find root
    for (auto& p : nodes) {
        if (!children.count(p.first)) {
            root_label = p.first;
            return p.second;
        }
    }
    return nullptr;
}

// Postorder traversal
void postorder(Node* node, vector<Node*>& order) {
    if (!node) return;
    for (auto c : node->children) postorder(c, order);
    order.push_back(node);
}

// Cost function
int cost(const string& v, const string& w) {
    if (v == "lambda" && w == "lambda") return 0;
    if (v == "lambda" || w == "lambda") return 1;
    return v == w ? 0 : 1;
}

struct EditOp {
    string type; // del, insert, subti
    string from, to;
};

// Backtracking
void backtrack(const vector<Node*>& T1, const vector<Node*>& T2, int i, int j,
               vector<EditOp>& ops, int cur_cost, int& min_cost, vector<EditOp>& best_ops) {
    if (i == T1.size() && j == T2.size()) {
        if (cur_cost < min_cost) {
            min_cost = cur_cost;
            best_ops = ops;
        }
        return;
    }
    // del
    if (i < T1.size()) {
        ops.push_back({"del", T1[i]->label, "lambda"});
        backtrack(T1, T2, i+1, j, ops, cur_cost + cost(T1[i]->label, "lambda"), min_cost, best_ops);
        ops.pop_back();
    }
    // insert
    if (j < T2.size()) {
        ops.push_back({"insert", "lambda", T2[j]->label});
        backtrack(T1, T2, i, j+1, ops, cur_cost + cost("lambda", T2[j]->label), min_cost, best_ops);
        ops.pop_back();
    }
    // subti
    if (i < T1.size() && j < T2.size()) {
        ops.push_back({"subti", T1[i]->label, T2[j]->label});
        backtrack(T1, T2, i+1, j+1, ops, cur_cost + cost(T1[i]->label, T2[j]->label), min_cost, best_ops);
        ops.pop_back();
    }
}

// Read input
void read_input(const string& filename, vector<pair<string, string>>& edges1, vector<pair<string, string>>& edges2) {
    ifstream in(filename);
    string line;
    int mode = 0;
    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') {
            if (line.find("Tree 2") != string::npos) mode = 1;
            continue;
        }
        istringstream iss(line);
        string u, v;
        iss >> u >> v;
        if (mode == 0) edges1.push_back({u, v});
        else edges2.push_back({u, v});
    }
}

int main() {
    vector<pair<string, string>> edges1, edges2;
    read_input("input.txt", edges1, edges2);
    string root1, root2;
    Node* T1 = build_tree(edges1, root1);
    Node* T2 = build_tree(edges2, root2);
    vector<Node*> post1, post2;
    postorder(T1, post1);
    postorder(T2, post2);
    vector<EditOp> ops, best_ops;
    int min_cost = 1e9;
    backtrack(post1, post2, 0, 0, ops, 0, min_cost, best_ops);
    // Output
    cout << "Danh sach bien doi (del, insert, subti):\n";
    for (auto& op : best_ops) {
        cout << op.type << "(" << op.from << ", " << op.to << ")\n";
    }
    cout << "\nChi phi nho nhat: " << min_cost << endl;
    return 0;
}
