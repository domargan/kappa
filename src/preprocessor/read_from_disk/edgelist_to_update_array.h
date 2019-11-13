#ifndef KAPPA_EDGELIST_TO_UPDATE_ARRAY_H
#define KAPPA_EDGELIST_TO_UPDATE_ARRAY_H

#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

#include "edgelist_to_edge_array.h"
#include "datatypes.h"

raw_update_array_t edgelist_to_update_array(std::string, int);

#endif  // KAPPA_EDGELIST_TO_UPDATE_ARRAY_H
