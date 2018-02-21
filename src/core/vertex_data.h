#ifndef KAPPA_VERTEX_DATA_H
#define KAPPA_VERTEX_DATA_H

#include <vector>
#include <cstdint>

typedef std::vector<float> values;

class vertex_data {
private:
    uint32_t max_vertex_number;
    float init_value;
    values temp_values;
    values final_values;
public:
    vertex_data(uint32_t v_num, float init);
    float get_value(uint32_t);
    values get_values();
    void write_value(uint32_t, float);
    void finalize();
};


#endif //KAPPA_VERTEX_DATA_H
