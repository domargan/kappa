#ifndef KAPPA_VERTEX_H
#define KAPPA_VERTEX_H

typedef std::vector<uint32_t> neighbors_vector_t;
typedef float state_t;

typedef struct Uvertex {
    //uint32_t id;
    neighbors_vector_t *neighbors;
    state_t state;
    state_t state_temp;
};

typedef struct Dvertex {
    neighbors_vector_t *in_neighbors;
    neighbors_vector_t *out_neighbors;
    state_t state;
    state_t state_temp;
};

/*
typedef struct Dvertex {
    //uint32_t id;
    neighbors_vector *in_neighbors;
    neighbors_vector *out_neighbors;
    state_t state;
    state_t state_temp;
};
 */

#endif //KAPPA_VERTEX_H
