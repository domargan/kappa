#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <boost/progress.hpp>

#include "edgelist_to_edge_array.h"

graph_size_t edgelist_count_lines(std::string edgelist_file){
    std::fstream fs;
    fs.open(edgelist_file);

    int number_of_lines = 0;

    if (fs) {
        std::string line;

        while (getline(fs, line)) {
            ++number_of_lines;
        }
    }

    fs.close();

    return number_of_lines;
}

bool sort_edges_by_src(const std::vector<vertex_id_t>& vec1, const std::vector<vertex_id_t>& vec2) {
    // Usage:
    // std::sort(edges.begin(), edges.end(), sort_edges_by_src);
    return vec1[0] < vec2[0];
}

bool sort_edges_by_dst(const std::vector<vertex_id_t>& vec1, const std::vector<vertex_id_t>& vec2) {
    // Usage:
    // std::sort(edges.begin(), edges.end(), sort_edges_by_dst);
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

    return v_num;
}

raw_edge_array_t edgelist_to_edge_array(std::string edgelist_file, char separator) {
    raw_edge_array_t edges;

    std::fstream fs;
    fs.open(edgelist_file);

    if (fs) {

        std::cout << "Opened edgelist file: " << edgelist_file << "\nReading file and parsing entries into edge array..." << std::endl;

        std::string line;

        int number_of_lines = edgelist_count_lines(edgelist_file);
        boost::progress_display show_progress(number_of_lines);

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

    std::cout << "\nFinished reading and parsing the edgelist file.\n" << std::endl;

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
