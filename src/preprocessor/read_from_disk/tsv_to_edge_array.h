#ifndef KAPPA_TSV_TO_EDGE_ARRAY_H
#define KAPPA_TSV_TO_EDGE_ARRAY_H

#include <iostream>
#include <fstream>
#include <vector>
#include <set>

#include "datatypes.h"

typedef std::vector<std::vector<vertex_id_t>> raw_edge_array_t;

raw_edge_array_t tsv_to_edges(std::string, char);
bool sort_edges_by_src(const std::vector<vertex_id_t>&, const std::vector<vertex_id_t>&);
bool sort_edges_by_dst(const std::vector<vertex_id_t>&, const std::vector<vertex_id_t>&);
graph_size_t unique_vertex_count(raw_edge_array_t&);
std::set<vertex_id_t> extract_vertices(raw_edge_array_t&);
void print_edge_array(raw_edge_array_t&);

#endif //KAPPA_READ_TSV_DISK_H
