#ifndef KAPPA_RUNTIME_HARD_BARRIER_H
#define KAPPA_RUNTIME_HARD_BARRIER_H

#include "digraph.h"
#include "read_from_disk/edgelist_to_edge_array.h"
#include "scheduler.h"
#include "update.h"

void run(Computation,
        Updating,
        Digraph*,
        raw_edge_array_t&,
        const std::vector<vertex_id_t> &);

#endif //KAPPA_RUNTIME_HARD_BARRIER_H
