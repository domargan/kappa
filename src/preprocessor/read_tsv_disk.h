#ifndef KAPPA_READ_TSV_DISK_H
#define KAPPA_READ_TSV_DISK_H

#include <iostream>
#include <fstream>
#include <vector>

typedef std::vector<std::vector<unsigned int>> raw_edge_array;

raw_edge_array tsv_to_edges(std::string, char);
bool sort_edges_by_src(const std::vector<unsigned int>&, const std::vector<unsigned int>&);
void print_edge_array(raw_edge_array&);

#endif //KAPPA_READ_TSV_DISK_H
