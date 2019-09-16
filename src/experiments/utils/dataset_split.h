#ifndef KAPPA_DATASET_SPLIT_H
#define KAPPA_DATASET_SPLIT_H

#include <string>
#include <vector>

#include "datatypes.h"

graph_size_t number_of_lines(std::string);
std::vector<graph_size_t> dataset_to_batches(graph_size_t,
                                             graph_size_t,
                                             graph_size_t,
                                             graph_size_t);
std::vector<graph_size_t> dataset_to_batches_with_core(graph_size_t,
                                                       graph_size_t,
                                                       graph_size_t,
                                                       graph_size_t,
                                                       graph_size_t);

#endif  // KAPPA_DATASET_SPLIT_H
