#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <set>
#include <boost/progress.hpp>
#include <boost/iostreams/device/mapped_file.hpp>

#include "edgelist_to_edge_array.h"

graph_size_t edgelist_count_lines(std::string edgelist_file){
    std::chrono::steady_clock::time_point begin_timer = std::chrono::steady_clock::now();

    std::ifstream fs;
    fs.open(edgelist_file);

    int number_of_lines = 0;

    if (fs) {
        std::string line;

        while (getline(fs, line)) {
            ++number_of_lines;
        }
    }

    fs.close();

    std::chrono::steady_clock::time_point end_timer = std::chrono::steady_clock::now();
    float time = std::chrono::duration<float>(end_timer - begin_timer).count();
    std::cout << "\n(CPU) LINECOUNT TIME: " << time << std::endl;

    return number_of_lines;
}

graph_size_t edgelist_count_lines2(std::string edgelist_file){
    std::chrono::steady_clock::time_point begin_timer = std::chrono::steady_clock::now();

    std::ifstream edgelist(edgelist_file);
    int number_of_lines = std::count(std::istreambuf_iterator<char>(edgelist),
                                     std::istreambuf_iterator<char>(), '\n');

    std::chrono::steady_clock::time_point end_timer = std::chrono::steady_clock::now();
    float time = std::chrono::duration<float>(end_timer - begin_timer).count();
    std::cout << "\n(CPU) LINECOUNT TIME: " << time << std::endl;

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

raw_edge_array_t edgelist_to_edge_array(std::string edgelist_file, int line_count) {
    std::chrono::steady_clock::time_point begin_read_file = std::chrono::steady_clock::now();

    boost::iostreams::mapped_file_source file(edgelist_file);
    std::string fileContent(file.data(), file.size());
    file.close();

    raw_edge_array_t edge_array(line_count);
    //edge_array.reserve(line_count);

    std::istringstream iss{fileContent};
    std::string line;
    int line_idx = 0;

    boost::progress_display show_progress(line_count);

    while (std::getline(iss, line)) {
        int pos = line.find_first_of(' ');

        edge_array[line_idx][0] = stoi(line.substr(0,pos));
        edge_array[line_idx][1] = stoi(line.substr(pos+1));

        ++line_idx;
        ++show_progress;
    }

    fileContent.clear();

    std::chrono::steady_clock::time_point end_read_file = std::chrono::steady_clock::now();
    float time_read_file = std::chrono::duration<float>(end_read_file - begin_read_file).count();
    std::cout << "\n(CPU) DATASET READING AND PARSING TIME: " << time_read_file << std::endl;

    return edge_array;
}

void print_edge_array(raw_edge_array_t& edges) {
    for (auto &edge : edges) {
        std::cout << edge[0] << " " << edge[1] << std::endl;
    }
}
