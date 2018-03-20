#ifndef KAPPA_VERTEX_H
#define KAPPA_VERTEX_H

typedef std::vector<uint32_t> neighbors_vector_t;
typedef float state_t;

// TODO: Update or remove Uvertex...
typedef struct Uvertex {
    //uint32_t id;
    neighbors_vector_t* neighbors;
    state_t state;
    state_t state_temp;
};

typedef struct Dvertex {
    // TODO: remove duplicated neighbor vectors
    neighbors_vector_t* in_neighbors;
    neighbors_vector_t* out_neighbors;
    uint32_t in_degree;
    uint32_t out_degree;
    state_t* state;
    state_t* state_temp;
};

/*
 * old struct...
typedef struct Dvertex {
    //uint32_t id;
    neighbors_vector *in_neighbors;
    neighbors_vector *out_neighbors;
    state_t state;
    state_t state_temp;
};
 */

#endif //KAPPA_VERTEX_H
