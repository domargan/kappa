#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include "tsv_to_edge_array.h"

// TODO: Rename this file and functions to make the names more correct (confusing: e.g. edges, edge array, etc...)
// TODO: Add an option to simplify graph building by directly doing add_edge() without building an edge array first
// TODO: Can we avoid having the edge array structure in the first place?

bool sort_edges_by_src(const std::vector<uint32_t>& vec1, const std::vector<uint32_t>& vec2) {
    return vec1[0] < vec2[0];
}

bool sort_edges_by_dst(const std::vector<uint32_t>& vec1, const std::vector<uint32_t>& vec2) {
    return vec1[1] < vec2[1];
}

std::set<uint32_t> extract_vertices(raw_edge_array& edges) {
    std::set<uint32_t> vertex_set;

    for(int i=0; i<edges.size(); i++){
        vertex_set.insert(edges[i][0]);
        vertex_set.insert(edges[i][1]);
    }

    return vertex_set;
}

uint32_t unique_vertex_count(raw_edge_array& edges){
    std::set<uint32_t> vertex_set = extract_vertices(edges);
    uint32_t v_num = static_cast<uint32_t>(vertex_set.size());

    std::cout << "Number of vertices: " << v_num << std::endl;

    return v_num;
}

raw_edge_array tsv_to_edges(std::string tsv_file, char separator) {
    raw_edge_array edges;

    std::fstream fs;
    fs.open(tsv_file);

    if (fs) {

        std::cout << "Opened " << tsv_file << "\nParsing file into edge array..." << std::endl;

        std::string line;

        while (getline(fs, line)) {
            std::stringstream sep(line);
            std::string vertex;

            edges.emplace_back();

            // TODO: edges needs only 2 values per field, simplify with an 1(vector)x2(array) structure
            while (getline(sep, vertex, separator)) {
                edges.back().push_back(static_cast<uint32_t &&>(stoi(vertex)));
            }
        }
    }

    fs.close();

    std::cout << "Parsing finished." << std::endl;
    std::cout << "Number of edges: " << edges.size() << std::endl;

    /*
    std::cout << "Sorting edge array by source vertices..." << std::endl;
    std::sort(edges.begin(), edges.end(), sort_edges_by_dst);
    std::cout << "Sorting finished." << std::endl;
    */

    return edges;
}

void print_edge_array(raw_edge_array& edges) {
    for (auto &edge : edges) {
        for (auto &vertex : edge) {
            std::cout << vertex << ' ';
        }
        std::cout << std::endl;
    }
}