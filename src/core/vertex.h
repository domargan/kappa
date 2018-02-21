#ifndef KAPPA_VERTEX_H
#define KAPPA_VERTEX_H

typedef std::vector<uint32_t> neighbors_vector;

typedef struct uvertex {
    //uint32_t id;
    neighbors_vector *neighbors;
    float state;
    float state_temp;

};

typedef struct dvertex {
    //uint32_t id;
    neighbors_vector *in_neighbors;
    neighbors_vector *out_neighbors;
    float state;
    float state_temp;
};

#endif //KAPPA_VERTEX_H
