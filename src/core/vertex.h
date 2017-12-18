#ifndef KAPPA_VERTEX_H
#define KAPPA_VERTEX_H

typedef std::vector<unsigned int> neighbors_vector;

typedef struct uvertex {
    //unsigned int id;
    neighbors_vector neighbors;
    int degree;
    float state;
    float state_temp;
};

typedef struct dvertex {
    //unsigned int id;
    neighbors_vector in_neighbors;
    neighbors_vector out_neighbors;
    int in_degree;
    int out_degree;
    float state;
    float state_temp;
};



#endif //KAPPA_VERTEX_H
