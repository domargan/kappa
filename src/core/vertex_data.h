#ifndef KAPPA_VERTEX_DATA_H
#define KAPPA_VERTEX_DATA_H

#include <vector>

typedef std::vector<float> values;

class vertex_data {
private:
    unsigned int max_vertex_number;
    float init_value;
    values temp_values;
    values final_values;
public:
    vertex_data(unsigned int v_num, float init);
    float get_value(unsigned int);
    values get_values();
    void write_value(unsigned int, float);
    void finalize();
};


#endif //KAPPA_VERTEX_DATA_H
