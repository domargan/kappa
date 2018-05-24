#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <boost/progress.hpp>

#include "edgelist_to_edge_array.h"

// TODO: Rename this file and functions to make the names more correct (confusing: e.g. edges, edge array, etc...)
// TODO: Add an option to simplify graph building by directly doing add_edge() without building an edge array first
// TODO: Can we avoid having the edge array structure in the first place?

bool sort_edges_by_src(const std::vector<vertex_id_t>& vec1, const std::vector<vertex_id_t>& vec2) {
    return vec1[0] < vec2[0];
}

bool sort_edges_by_dst(const std::vector<vertex_id_t>& vec1, const std::vector<vertex_id_t>& vec2) {
    return vec1[1] < vec2[1];
}

std::set<vertex_id_t> extract_vertices(raw_edge_array_t& edges) {
    std::set<vertex_id_t> vertex_set;

    for(int i=0; i<edges.size(); i++){
        vertex_set.insert(edges[i][0]);
        vertex_set.insert(edges[i][1]);
    }

    return vertex_set;
}

graph_size_t unique_vertex_count(raw_edge_array_t& edges){
    std::set<vertex_id_t> vertex_set = extract_vertices(edges);
    graph_size_t v_num = static_cast<graph_size_t>(vertex_set.size());

    std::cout << "Number of vertices: " << v_num << std::endl;

    return v_num;
}

raw_edge_array_t edgelist_to_edges(std::string edgelist_file, char separator) {
    raw_edge_array_t edges;

    std::fstream fs;
    fs.open(edgelist_file);

    if (fs) {

        std::cout << "Opened " << edgelist_file << "\nParsing file into edge array..." << std::endl;

        std::string line;

        boost::progress_display show_progress(14855842);

        while (getline(fs, line)) {
            std::stringstream sep(line);
            std::string vertex;

            edges.emplace_back();

            // TODO: edges needs only 2 values per field, simplify with an 1(vector)x2(array) structure
            while (getline(sep, vertex, separator)) {
                edges.back().push_back(static_cast<vertex_id_t &&>(stoi(vertex)));
            }

            ++show_progress;
        }
    }

    fs.close();

    std::cout << "\nParsing finished." << std::endl;
    std::cout << "Number of edges: " << edges.size() << std::endl;

    /*
    std::cout << "Sorting edge array by source vertices..." << std::endl;
    std::sort(edges.begin(), edges.end(), sort_edges_by_dst);
    std::cout << "Sorting finished." << std::endl;
    */

    return edges;
}

void print_edge_array(raw_edge_array_t& edges) {
    for (auto &edge : edges) {
        for (auto &vertex : edge) {
            std::cout << vertex << ' ';
        }
        std::cout << std::endl;
    }
}
