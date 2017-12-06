#ifndef KAPPA_GRAPH_H
#define KAPPA_GRAPH_H

#include <vector>
#include <iostream>
#include <set>

// edge_vector is the main data structure for our graph: a std::vector of std::vectors of integers.
// Outer vector's indices represents source vertices' IDs.
// Inner vectorss hold destination vertices' IDs for each source vertex.
typedef std::vector<std::vector<unsigned int>> edge_vector;

class Graph {
private:
    unsigned int max_vertex_number; // Set a limit for the maximum possible number of vertices to fit in the graph.
    edge_vector edges; // Edges contained in the graph. Keeps the graph topology.
public:
    explicit Graph(unsigned int);
    edge_vector get_edge_vector(); // Return the edge_vector data structure.
    void print_edge_vector(); // Print all the values of a edge_vector data structure (use just for testing).
    void add_edge(unsigned int, unsigned int); // Add an edge between two vertices.
    void remove_edge(unsigned int, unsigned int); // Remove the edge between two vertices.
    bool has_edge(unsigned int, unsigned int); // Return true of the edge is in the graph.
    int in_degree(unsigned int); // Return the in-degree of a vertex.
    int out_degree(unsigned int); // Return the out-degree of a vertex.
    int degree(unsigned int); // Return the degree of a vertex.
    int order(); // Return the number of vertices.
    int size(); // Return the number of edges.
};


#endif //KAPPA_GRAPH_H
