#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <unordered_set>
#include <vector>
#include <algorithm>
using namespace std;

namespace dsac::graph {

template <typename V, typename E>
class AdjacencyListGraph {
private:
    class ActualVertex;
    class ActualEdge;
    typedef std::list<ActualEdge*> IncidenceList;
    
    class ActualVertex {
    public:
        V element;
        IncidenceList outgoing;
        IncidenceList incoming;
        typename std::list<ActualVertex>::iterator pos;
        ActualVertex(V elem) : element{elem} {}
    };
    
    class ActualEdge {
    public:
        ActualVertex* origin;
        ActualVertex* dest;
        int weight;
        E element;
        typename std::list<ActualEdge>::iterator pos;
        ActualEdge(ActualVertex* u, ActualVertex* v, int w, E e)
            : origin{u}, dest{v}, weight{w}, element{e} {}
    };

    std::list<ActualVertex> vertex_list;
    std::list<ActualEdge> edge_list;
    bool directed;

public:
    AdjacencyListGraph(bool is_directed) : directed{is_directed} {
        std::cout << "Constructing graph using AdjacencyListGraph" << std::endl;
    }

    bool is_directed() const { return directed; }
    
    struct VertexHash;
    
    class Vertex {
        friend AdjacencyListGraph;
        friend VertexHash;
        
    private:
        ActualVertex* vert{nullptr};
        Vertex(const ActualVertex* v) : vert{const_cast<ActualVertex*>(v)} {}
        
    public:
        Vertex() {}
        V& operator*() const { return vert->element; }
        V* operator->() const { return &(vert->element); }
        bool operator==(Vertex other) const { return vert == other.vert; }
        bool operator!=(Vertex other) const { return vert != other.vert; }
        bool operator<(Vertex other) const { return vert->element < other.vert->element; }
        friend VertexHash;
    };

    struct VertexHash { 
        size_t operator()(Vertex v) const { return size_t(v.vert); } 
    };
    
    struct EdgeHash;

    class Edge {
        friend AdjacencyListGraph;
        friend EdgeHash;
        
    private:
        ActualEdge* edge{nullptr};
        Edge(const ActualEdge* e) : edge{const_cast<ActualEdge*>(e)} {}
        
    public:
        Edge() {}
        int weight() const { return edge->weight; }
        E& operator*() const { return edge->element; }
        E* operator->() const { return &(edge->element); }
        bool operator==(Edge other) const { return edge == other.edge; }
        bool operator!=(Edge other) const { return edge != other.edge; }
        bool operator<(Edge other) const { return edge < other.edge; }
    };

    struct EdgeHash { 
        size_t operator()(Edge e) const { return size_t(e.edge); } 
    };

    int num_vertices() const { return vertex_list.size(); }
    int num_edges() const { return edge_list.size(); }

    std::list<Vertex> vertices() const {
        std::list<Vertex> result;
        for (const ActualVertex& v : vertex_list)
            result.push_back(Vertex(&v));
        return result;
    }

    std::list<Edge> edges() const {
        std::list<Edge> result;
        for (const ActualEdge& e : edge_list)
            result.push_back(Edge(&e));
        return result;
    }

    bool has_edge(Vertex u, Vertex v) const {
        for (ActualEdge* e : u.vert->outgoing) {
            if (e->dest == v.vert) return true;
        }
        return false;
    }

    Edge get_edge(Vertex u, Vertex v) const {
        for (ActualEdge* e : u.vert->outgoing) {
            if (e->dest == v.vert) return Edge(e);
        }
        throw std::runtime_error("Edge not found");
    }

    int degree(Vertex v, bool outgoing = true) const {
        IncidenceList& adj(outgoing || !directed ? v.vert->outgoing : v.vert->incoming);
        return adj.size();
    }

    std::list<Vertex> neighbors(Vertex v, bool outgoing = true) const {
        std::list<Vertex> result;
        IncidenceList& adj(outgoing || !directed ? v.vert->outgoing : v.vert->incoming);
        for (ActualEdge* e : adj)
            result.push_back(Vertex(outgoing ? e->dest : e->origin));
        return result;
    }

    std::list<Edge> incident_edges(Vertex v, bool outgoing = true) const {
        std::list<Edge> result;
        IncidenceList& adj(outgoing || !directed ? v.vert->outgoing : v.vert->incoming);
        for (ActualEdge* e : adj)
            result.push_back(Edge(e));
        return result;
    }

    std::pair<Vertex,Vertex> endpoints(Edge e) const {
        return {Vertex(e.edge->origin), Vertex(e.edge->dest)};
    }

    Vertex opposite(Edge e, Vertex v) const {
        return Vertex(v.vert == e.edge->origin ? e.edge->dest : e.edge->origin);
    }

    Vertex insert_vertex(V elem) {
        auto iter = vertex_list.insert(vertex_list.end(), ActualVertex(elem));
        iter->pos = iter;
        return Vertex(&*iter);
    }

    Edge insert_edge(Vertex u, Vertex v, int weight = 1, E elem = E()) {
        // Check if edge already exists
        for (ActualEdge* e : u.vert->outgoing) {
            if (e->dest == v.vert) {
                e->weight = weight;
                e->element = elem;
                return Edge(e);
            }
        }

        // Create new edge
        auto iter = edge_list.insert(edge_list.end(), 
            ActualEdge(u.vert, v.vert, weight, elem));
        iter->pos = iter;
        ActualEdge* stored = &*iter;

        // Add to adjacency lists
        u.vert->outgoing.push_back(stored);
        if (directed)
            v.vert->incoming.push_back(stored);
        else
            v.vert->outgoing.push_back(stored);

        return Edge(stored);
    }

    void erase(Edge e) {
        // Remove from adjacency lists
        e.edge->origin->outgoing.remove(e.edge);
        if (directed)
            e.edge->dest->incoming.remove(e.edge);
        else
            e.edge->dest->outgoing.remove(e.edge);

        // Remove from edge list
        edge_list.erase(e.edge->pos);
    }

    void erase(Vertex v) {
        // Remove all incident edges
        for (ActualEdge* e : v.vert->outgoing) {
            if (directed)
                e->dest->incoming.remove(e);
            else
                e->dest->outgoing.remove(e);
            edge_list.erase(e->pos);
        }

        for (ActualEdge* e : v.vert->incoming) {
            e->origin->outgoing.remove(e);
            edge_list.erase(e->pos);
        }

        // Remove vertex
        vertex_list.erase(v.vert->pos);
    }

private:
    void clone(const AdjacencyListGraph& other) {
        directed = other.directed;
        vertex_list.clear();
        edge_list.clear();
        std::unordered_map<const ActualVertex*,Vertex> oldToNew;
        for (const auto& v : other.vertex_list)
            oldToNew[&v] = insert_vertex(v.element);
        for (const auto& e : other.edge_list)
            insert_edge(Vertex(oldToNew[e.origin]), Vertex(oldToNew[e.dest]), 
                       e.weight, e.element);
    }

public:
    AdjacencyListGraph(const AdjacencyListGraph& other) { clone(other); }

    AdjacencyListGraph& operator=(const AdjacencyListGraph& other) {
        if (this != &other) { clone(other); }
        return *this;
    }

    AdjacencyListGraph(AdjacencyListGraph&&) = default;
    AdjacencyListGraph& operator=(AdjacencyListGraph&&) = default;
    ~AdjacencyListGraph() = default;
};

template <typename V, typename E>
using VertexList = std::list<typename AdjacencyListGraph<V,E>::Vertex>;

template <typename V, typename E>
using EdgeList = std::list<typename AdjacencyListGraph<V,E>::Edge>;

template <typename V, typename E>
using VertexSet = std::unordered_set<typename AdjacencyListGraph<V,E>::Vertex, 
    typename AdjacencyListGraph<V,E>::VertexHash>;

template <typename V, typename E, typename T>
using VertexMap = std::unordered_map<typename AdjacencyListGraph<V,E>::Vertex, T, 
    typename AdjacencyListGraph<V,E>::VertexHash>;

template <typename V, typename E>
using VertexIntMap = VertexMap<V,E,int>;

template <typename V, typename E>
using VertexVertexMap = VertexMap<V,E,typename AdjacencyListGraph<V,E>::Vertex>;

template <typename V, typename E>
using VertexEdgeMap = VertexMap<V,E,typename AdjacencyListGraph<V,E>::Edge>;

template <typename V, typename E>
void dump(const AdjacencyListGraph<V,E>& G, std::ostream& out) {
    out << G.num_vertices() << " vertices and " << G.num_edges() << " edges" << std::endl;
    for (typename AdjacencyListGraph<V,E>::Vertex v : G.vertices()) {
        out << *v;
        if (G.is_directed()) out << std::endl << " out (" << G.degree(v,true) << ")";
        out << ":";
        for (typename AdjacencyListGraph<V,E>::Edge e : G.incident_edges(v,true))
            out << " " << *(G.opposite(e, v)) << "(" << e.weight() << ")";

        if (G.is_directed()) {
            out << std::endl << "  in (" << G.degree(v,false) << "):";
            for (typename AdjacencyListGraph<V,E>::Edge e : G.incident_edges(v,false))
                out << " " << *(G.opposite(e, v)) << "(" << e.weight() << ")";
        }
        out << std::endl;
    }
    out << std::endl;
}

} // namespace dsac::graph
    
void DFS(dsac::graph::AdjacencyListGraph<string, int>& graph,
    dsac::graph::AdjacencyListGraph<string, int>::Vertex start,
    unordered_set<dsac::graph::AdjacencyListGraph<string, int>::Vertex,
                   dsac::graph::AdjacencyListGraph<string, int>::VertexHash>& visited) {
    
    // Print visit of current vertex with exact format
    cout << "Visit vertex: " << *start << endl;
    visited.insert(start);
    
    // Get all neighbors and sort them alphabetically
    auto neighbors = graph.neighbors(start);
    vector<dsac::graph::AdjacencyListGraph<string, int>::Vertex> sorted_neighbors(neighbors.begin(), neighbors.end());
    sort(sorted_neighbors.begin(), sorted_neighbors.end(), 
         [](const auto& a, const auto& b) { return *a < *b; });
    
    // For each unvisited neighbor
    for (auto neighbor : sorted_neighbors) {
        if (visited.find(neighbor) == visited.end()) {
            // Print discovery edge with exact format
            cout << "Discovery edge from " << *start << " to " << *neighbor << endl;
            DFS(graph, neighbor, visited);
        }
    }
}

int main() {
    // Create an undirected graph
    dsac::graph::AdjacencyListGraph<string, int> graph(false);

    // Insert vertices
    auto vA = graph.insert_vertex("A");
    auto vB = graph.insert_vertex("B");
    auto vC = graph.insert_vertex("C");
    auto vD = graph.insert_vertex("D");
    auto vE = graph.insert_vertex("E");
    auto vF = graph.insert_vertex("F");

    // Insert edges in order that matches desired DFS traversal
    graph.insert_edge(vC, vB);
    graph.insert_edge(vB, vA);
    graph.insert_edge(vA, vD);
    graph.insert_edge(vC, vD);
    graph.insert_edge(vD, vE);
    graph.insert_edge(vD, vF);
    graph.insert_edge(vC, vE);

    // Create visited set
    unordered_set<dsac::graph::AdjacencyListGraph<string, int>::Vertex,
                  dsac::graph::AdjacencyListGraph<string, int>::VertexHash> visited;

    // Start DFS from vertex C
    DFS(graph, vC, visited);

    return 0;
}
