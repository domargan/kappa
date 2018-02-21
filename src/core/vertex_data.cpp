#include "vertex_data.h"

vertex_data::vertex_data(uint32_t v_num, float init) {
    max_vertex_number = v_num;
    init_value = init;
    // std::vector.assign() has O(n) in this case.
    final_values.assign(max_vertex_number, init_value);
    temp_values = final_values;
}

float vertex_data::get_value(uint32_t vertex) {
    return final_values[vertex];
}

values vertex_data::get_values() {
    return final_values;
}

void vertex_data::write_value(uint32_t vertex, float value) {
    temp_values[vertex] = value;
}

void vertex_data::finalize() {
    // std::vector.swap() has O(1) complexity.
    final_values.swap(temp_values);
}

