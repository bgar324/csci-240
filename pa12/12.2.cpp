#include <iostream>
#include "../DSAC-main/graph/mst.h"

using namespace dsac::graph;
using namespace std;

int main() {
    Graph<string,int> g(false);
    auto A = g.insert_vertex("A");
    auto B = g.insert_vertex("B");
    auto C = g.insert_vertex("C");
    auto D = g.insert_vertex("D");
    auto E = g.insert_vertex("E");

    g.insert_edge(A, B, 3);
    g.insert_edge(A, C, 4);
    g.insert_edge(A, D, 5);
    g.insert_edge(A, E, 5);
    g.insert_edge(B, C, 4);
    g.insert_edge(C, D, 2);
    g.insert_edge(D, E, 5);
    g.insert_edge(C, E, 3);

    auto tree = mst_kruskal(g);
    int totalCost = 0;
    for (auto e : tree) {
        auto p = g.endpoints(e);
        cout
            << *p.first << "-"
            << *p.second
            << ": " << e.weight() << "\n";
        totalCost += e.weight();
    }
    cout << "Total cost: " << totalCost << "\n";
    return 0;
}
