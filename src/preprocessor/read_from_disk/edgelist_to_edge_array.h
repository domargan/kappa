#ifndef KAPPA_EDGELIST_TO_EDGE_ARRAY_H
#define KAPPA_EDGELIST_TO_EDGE_ARRAY_H

#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "datatypes.h"

// typedef std::vector<std::vector<vertex_id_t>> raw_edge_array_t;
typedef std::vector<std::array<vertex_id_t, 2>> raw_edge_array_t;

graph_size_t edgelist_count_lines(std::string);
graph_size_t edgelist_count_lines2(std::string);
raw_edge_array_t edgelist_to_edge_array(std::string, int);
bool sort_edges_by_src(const std::vector<vertex_id_t>&,
                       const std::vector<vertex_id_t>&);
bool sort_edges_by_dst(const std::vector<vertex_id_t>&,
                       const std::vector<vertex_id_t>&);
graph_size_t unique_vertex_count(raw_edge_array_t&);
std::set<vertex_id_t> extract_vertices(raw_edge_array_t&);
void print_edge_array(raw_edge_array_t&);

#endif  // KAPPA_EDGELIST_TO_EDGE_ARRAY_H
