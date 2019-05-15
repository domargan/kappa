#ifndef KAPPA_UPDATE_H
#define KAPPA_UPDATE_H

#include "datatypes.h"

class Digraph;

// Updating model (functions to use for update tasks)
typedef struct Updating {
    void (*edge_insertion)(Digraph*, vertex_id_t, vertex_id_t);
    void (*edge_deletion)(Digraph*, vertex_id_t, vertex_id_t);
} Updating;

// Update tasks
typedef struct Update {
    Update(update_type_t, vertex_id_t, vertex_id_t);

    update_type_t type;

    vertex_id_t src;
    vertex_id_t dst;
} Update;

#endif //KAPPA_UPDATE_H
