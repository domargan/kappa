#ifndef KAPPA_CONNECTED_COMPONENTS_H
#define KAPPA_CONNECTED_COMPONENTS_H

#include "digraph.h"

typedef std::map<components_number_t, std::vector<vertex_id_t>> cc_map_t;

extern cc_map_t cc_map;

void set_components_labels(Digraph *);

#endif //KAPPA_CONNECTED_COMPONENTS_H
