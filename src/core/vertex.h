#ifndef KAPPA_VERTEX_H
#define KAPPA_VERTEX_H

#include <vector>

#include "datatypes.h"

typedef std::vector<vertex_id_t> neighbors_vector_t;

typedef struct Dvertex {
  // TODO: remove duplicated neighbor vectors
  neighbors_vector_t in_neighbors;
  neighbors_vector_t out_neighbors;
  graph_size_t in_degree;
  graph_size_t out_degree;
  state_t* state;
  state_t* state_temp;
  components_number_t component_label;
  timestamp_logical_t update_ts;     // Timeestamp of the last UPDATE task that
                                     // added or removed an adge to the vertex
  timestamp_logical_t on_update_ts;  // Timestamp of the last ON_UPDATE task
                                     // that touched the vertex
  timestamp_logical_t on_activate_ts;  // Timestamp of the last ON_ACTIVATE task
                                       // that touched the vertex
  bool scheduled;
} Dvertex;

#endif  // KAPPA_VERTEX_H
