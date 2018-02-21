#ifndef KAPPA_TSV_TO_EDGE_ARRAY_H
#define KAPPA_TSV_TO_EDGE_ARRAY_H

#include <iostream>
#include <fstream>
#include <vector>
#include <set>

typedef std::vector<std::vector<uint32_t>> raw_edge_array;

raw_edge_array tsv_to_edges(std::string, char);
bool sort_edges_by_src(const std::vector<uint32_t>&, const std::vector<uint32_t>&);
bool sort_edges_by_dst(const std::vector<uint32_t>&, const std::vector<uint32_t>&);
uint32_t unique_vertex_count(raw_edge_array&);
std::set<uint32_t> extract_vertices(raw_edge_array&);
void print_edge_array(raw_edge_array&);

#endif //KAPPA_READ_TSV_DISK_H
