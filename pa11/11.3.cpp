#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <unordered_set>
#include <queue>
#include <limits>
#include <unordered_map>
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
        bool operator<(Vertex other) const { return vert < other.vert; }

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

// Custom comparator for priority queue
struct CompareDistance {
    bool operator()(const pair<int, dsac::graph::AdjacencyListGraph<string, int>::Vertex>& a,
                   const pair<int, dsac::graph::AdjacencyListGraph<string, int>::Vertex>& b) {
        return a.first > b.first;
    }
};

void dijkstra(dsac::graph::AdjacencyListGraph<string, int>& graph,
             dsac::graph::AdjacencyListGraph<string, int>::Vertex start,
             dsac::graph::AdjacencyListGraph<string, int>::Vertex end) {
    
    // Priority queue to store vertices and their distances
    priority_queue<pair<int, dsac::graph::AdjacencyListGraph<string, int>::Vertex>,
                  vector<pair<int, dsac::graph::AdjacencyListGraph<string, int>::Vertex>>,
                  CompareDistance> pq;
    
    // Maps to store distances and previous vertices
    unordered_map<dsac::graph::AdjacencyListGraph<string, int>::Vertex,
                 int,
                 dsac::graph::AdjacencyListGraph<string, int>::VertexHash> distances;
    unordered_map<dsac::graph::AdjacencyListGraph<string, int>::Vertex,
                 dsac::graph::AdjacencyListGraph<string, int>::Vertex,
                 dsac::graph::AdjacencyListGraph<string, int>::VertexHash> previous;
    
    // Initialize distances
    for (auto v : graph.vertices()) {
        distances[v] = numeric_limits<int>::max();
    }
    distances[start] = 0;
    
    pq.push({0, start});
    
    while (!pq.empty()) {
        auto current = pq.top().second;
        int current_dist = pq.top().first;
        pq.pop();
        
        if (current == end) break;
        
        // Skip if we've found a better path already
        if (current_dist > distances[current]) continue;
        
        // Check all neighbors
        for (auto edge : graph.incident_edges(current)) {
            auto neighbor = graph.opposite(edge, current);
            int new_dist = distances[current] + edge.weight();
            
            if (new_dist < distances[neighbor]) {
                distances[neighbor] = new_dist;
                previous[neighbor] = current;
                pq.push({new_dist, neighbor});
            }
        }
    }
    
    // Print the path
    cout << "Shortest path from B to E (total cost: " << distances[end] << "):\n";
    vector<dsac::graph::AdjacencyListGraph<string, int>::Vertex> path;
    auto current = end;
    while (current != start) {
        path.push_back(current);
        current = previous[current];
    }
    path.push_back(start);
    
    // Print in reverse order (from start to end)
    cout << "Path: ";
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        cout << **it;
        if (it + 1 != path.rend()) cout << " -> ";
    }
    cout << endl;
}

int main() {
    dsac::graph::AdjacencyListGraph<string, int> graph(false);  // undirected graph
    
    // Create vertices
    auto vA = graph.insert_vertex("A");
    auto vB = graph.insert_vertex("B");
    auto vC = graph.insert_vertex("C");
    auto vD = graph.insert_vertex("D");
    auto vE = graph.insert_vertex("E");
    
    // Insert edges with weights
    graph.insert_edge(vA, vB, 3);
    graph.insert_edge(vB, vC, 4);
    graph.insert_edge(vA, vD, 5);
    graph.insert_edge(vA, vE, 10);
    graph.insert_edge(vC, vD, 2);
    graph.insert_edge(vD, vE, 3);
    graph.insert_edge(vC, vE, 6);
    
    // Find shortest path from B to E
    dijkstra(graph, vB, vE);
    
    return 0;
}