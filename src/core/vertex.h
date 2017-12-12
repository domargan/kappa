#ifndef KAPPA_VERTEX_H
#define KAPPA_VERTEX_H

typedef std::vector<unsigned int> neighbors_vector;

typedef struct uvertex {
    //unsigned int id;
    neighbors_vector* neighbors;
    int degree;
    float state;
    // get member functions out!
    void add_neighbor(unsigned int, unsigned int);
    neighbors_vector get_neighbors();
    int get_degree();
    void write_state(float);
    float get_state();
};

typedef struct dvertex {
    //unsigned int id;
    neighbors_vector* in_neighbors;
    neighbors_vector* out_neighbors;
    int in_degree;
    int out_degree;
    float state;
    // get member functions out!
    void add_in_neighbor(unsigned int);
    void add_out_neighbor(unsigned int);
    neighbors_vector get_in_neighbors();
    neighbors_vector get_out_neighbors();
    int get_in_degree();
    int get_out_degree();
    int get_degree();
    void write_state(float);
    float get_state();
};



#endif //KAPPA_VERTEX_H
