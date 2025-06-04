#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <unordered_set>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <climits>
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

    // Find vertex by element value
    Vertex find_vertex(const V& elem) const {
        for (const ActualVertex& v : vertex_list) {
            if (v.element == elem) {
                return Vertex(&v);
            }
        }
        return Vertex(); // Return default (null) vertex if not found
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

// Dijkstra's shortest path algorithm
template <typename V, typename E>
struct ShortestPathResult {
    VertexIntMap<V,E> distances;
    VertexVertexMap<V,E> predecessors;
    bool path_exists;
    int total_cost;
    std::list<typename AdjacencyListGraph<V,E>::Vertex> path;
};

template <typename V, typename E>
ShortestPathResult<V,E> dijkstra(const AdjacencyListGraph<V,E>& G, 
    typename AdjacencyListGraph<V,E>::Vertex source,
    typename AdjacencyListGraph<V,E>::Vertex target) {
    
    using Vertex = typename AdjacencyListGraph<V,E>::Vertex;
    
    VertexIntMap<V,E> distances;
    VertexVertexMap<V,E> predecessors;
    VertexSet<V,E> visited;
    
    // Priority queue: pair<distance, vertex>
    priority_queue<pair<int, Vertex>, vector<pair<int, Vertex>>, 
                   greater<pair<int, Vertex>>> pq;
    
    // Initialize distances
    for (Vertex v : G.vertices()) {
        distances[v] = INT_MAX;
    }
    distances[source] = 0;
    pq.push({0, source});
    
    while (!pq.empty()) {
        int current_dist = pq.top().first;
        Vertex current = pq.top().second;
        pq.pop();
        
        if (visited.count(current)) continue;
        visited.insert(current);
        
        if (current == target) break;
        
        for (auto edge : G.incident_edges(current, true)) {
            Vertex neighbor = G.opposite(edge, current);
            int new_dist = current_dist + edge.weight();
            
            if (new_dist < distances[neighbor]) {
                distances[neighbor] = new_dist;
                predecessors[neighbor] = current;
                pq.push({new_dist, neighbor});
            }
        }
    }
    
    ShortestPathResult<V,E> result;
    result.distances = distances;
    result.predecessors = predecessors;
    result.path_exists = (distances[target] != INT_MAX);
    result.total_cost = distances[target];
    
    // Reconstruct path
    if (result.path_exists) {
        std::list<Vertex> path;
        Vertex current = target;
        while (current != source) {
            path.push_front(current);
            current = predecessors[current];
        }
        path.push_front(source);
        result.path = path;
    }
    
    return result;
}

} // namespace dsac::graph

// Function to load flight data from file
dsac::graph::AdjacencyListGraph<string, int> loadFlightData(const string& filename) {
    dsac::graph::AdjacencyListGraph<string, int> graph(true); // directed graph
    ifstream file(filename);
    string line;
    
    // Map to store vertices to avoid duplicates
    unordered_map<string, dsac::graph::AdjacencyListGraph<string, int>::Vertex> vertices;
    
    while (getline(file, line)) {
        istringstream iss(line);
        string origin, dest;
        int cost;
        
        if (iss >> origin >> dest >> cost) {
            // Insert vertices if they don't exist
            if (vertices.find(origin) == vertices.end()) {
                vertices[origin] = graph.insert_vertex(origin);
            }
            if (vertices.find(dest) == vertices.end()) {
                vertices[dest] = graph.insert_vertex(dest);
            }
            
            // Insert edge
            graph.insert_edge(vertices[origin], vertices[dest], cost);
        }
    }
    
    return graph;
}

int main() {
    // Load flight data
    cout << "Loading flight data from PA12Flights-1.txt" << endl;
    auto graph = loadFlightData("PA12Flights-1.txt");
    
    // Display the graph
    dsac::graph::dump(graph, cout);
    
    // Test shortest path operations
    cout << "=== Shortest Path Tests ===" << endl;
    
    // Test 1: Find cheapest flight from LAX to JFK
    cout << "\n1. Finding cheapest flight from LAX to JFK:" << endl;
    auto lax = graph.find_vertex("LAX");
    auto jfk = graph.find_vertex("JFK");
    
    if (lax != dsac::graph::AdjacencyListGraph<string, int>::Vertex() && 
        jfk != dsac::graph::AdjacencyListGraph<string, int>::Vertex()) {
        
        auto result = dsac::graph::dijkstra(graph, lax, jfk);
        
        if (result.path_exists) {
            cout << "Path found! Total cost: $" << result.total_cost << endl;
            cout << "Route: ";
            bool first = true;
            for (auto vertex : result.path) {
                if (!first) cout << " -> ";
                cout << *vertex;
                first = false;
            }
            cout << endl;
        } else {
            cout << "No path found from LAX to JFK" << endl;
        }
    }
    
    // Test 2: Find cheapest flight from JFK
    cout << "\n2. Finding cheapest flights originating from JFK:" << endl;
    if (jfk != dsac::graph::AdjacencyListGraph<string, int>::Vertex()) {
        cout << "Direct flights from JFK:" << endl;
        for (auto edge : graph.incident_edges(jfk, true)) {
            auto dest = graph.opposite(edge, jfk);
            cout << "  JFK -> " << *dest << " ($" << edge.weight() << ")" << endl;
        }
        
        // Find cheapest direct flight from JFK
        int min_cost = INT_MAX;
        string cheapest_dest;
        for (auto edge : graph.incident_edges(jfk, true)) {
            if (edge.weight() < min_cost) {
                min_cost = edge.weight();
                auto dest = graph.opposite(edge, jfk);
                cheapest_dest = *dest;
            }
        }
        
        if (min_cost != INT_MAX) {
            cout << "Cheapest direct flight from JFK: JFK -> " << cheapest_dest 
                 << " ($" << min_cost << ")" << endl;
        }
    }
    
    // Test 3: Find all shortest paths from a specific airport
    cout << "\n3. Shortest paths from ORD to all other airports:" << endl;
    auto ord = graph.find_vertex("ORD");
    if (ord != dsac::graph::AdjacencyListGraph<string, int>::Vertex()) {
        for (auto vertex : graph.vertices()) {
            if (vertex != ord) {
                auto result = dsac::graph::dijkstra(graph, ord, vertex);
                if (result.path_exists) {
                    cout << "ORD to " << *vertex << ": $" << result.total_cost;
                    cout << " (";
                    bool first = true;
                    for (auto v : result.path) {
                        if (!first) cout << " -> ";
                        cout << *v;
                        first = false;
                    }
                    cout << ")" << endl;
                }
            }
        }
    }
    
    return 0;
}