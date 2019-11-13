#ifndef KAPPA_DATATYPES_H
#define KAPPA_DATATYPES_H

#include <chrono>
#include <cstdint>
#include <array>
#include <vector>

typedef uint32_t vertex_id_t;

typedef uint32_t graph_size_t;

typedef uint32_t components_number_t;

typedef std::vector<std::array<vertex_id_t, 2>>
    raw_edge_array_t;  // format: [[src1,dst1],[src2,dst2],...]
typedef std::vector<std::array<vertex_id_t, 3>>
    raw_update_array_t;  // format: [[src1,dst1,{1,0}],[src2,dst2,{1,0}],...]
                        // where 0 denotes deletion and 1 denotes addition

typedef double state_t;

typedef uint32_t task_id_t;

typedef enum { ON_ACTIVATE, ON_UPDATE, UPDATE } task_type_t;

typedef std::chrono::microseconds timestamp_real_t;
typedef uint32_t timestamp_logical_t;

typedef enum { ADD, REMOVE } update_type_t;

#endif  // KAPPA_DATATYPES_H
