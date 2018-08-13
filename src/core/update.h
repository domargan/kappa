#ifndef KAPPA_UPDATE_H
#define KAPPA_UPDATE_H

#include "datatypes.h"

typedef struct Update {
    Update(update_type_t, vertex_id_t, vertex_id_t);

    update_type_t type;

    vertex_id_t src;
    vertex_id_t dst;
} Update;

#endif //KAPPA_UPDATE_H
