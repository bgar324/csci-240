#include <iostream>
#include <vector>
#include <string>
using namespace std;

class MatrixGraph {
private:
    vector<vector<int>> matrix;
    vector<string> vertices;
    int num_vertices;

public:
    MatrixGraph(int size) : num_vertices(size) {
        // Initialize the adjacency matrix with 0s
        matrix.resize(size, vector<int>(size, 0));
        vertices.resize(size);
    }

    void addVertex(int index, string label) {
        if (index >= 0 && index < num_vertices) {
            vertices[index] = label;
        }
    }

    void addEdge(int from, int to, int weight = 1) {
        if (from >= 0 && from < num_vertices && to >= 0 && to < num_vertices) {
            // For undirected graph, add edges in both directions
            matrix[from][to] = weight;
            matrix[to][from] = weight;
        }
    }

    void removeEdge(int from, int to) {
        if (from >= 0 && from < num_vertices && to >= 0 && to < num_vertices) {
            matrix[from][to] = 0;
            matrix[to][from] = 0;
        }
    }

    void printGraph() {
        // Print vertex labels
        cout << "  ";
        for (const auto& vertex : vertices) {
            cout << vertex << " ";
        }
        cout << endl;

        // Print matrix
        for (int i = 0; i < num_vertices; i++) {
            cout << vertices[i] << " ";
            for (int j = 0; j < num_vertices; j++) {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main() {
    // Test case 1: Graph from Exercise 1
    cout << "Test Case 1:" << endl;
    MatrixGraph graph1(4);  // 4 vertices: A, B, C, D

    // Add vertices
    graph1.addVertex(0, "A");
    graph1.addVertex(1, "B");
    graph1.addVertex(2, "C");
    graph1.addVertex(3, "D");

    // Add edges with weights
    graph1.addEdge(0, 2, 200);  // A-C
    graph1.addEdge(0, 1, 100);  // A-B
    graph1.addEdge(1, 3, 300);  // B-D

    cout << "Initial graph:" << endl;
    graph1.printGraph();

    // Remove edge A-B
    cout << "\nAfter removing edge A-B:" << endl;
    graph1.removeEdge(0, 1);
    graph1.printGraph();

    return 0;
}
