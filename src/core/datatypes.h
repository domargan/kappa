#ifndef KAPPA_DATATYPES_H
#define KAPPA_DATATYPES_H

#include <cstdint>

typedef uint32_t vertex_id_t;

typedef uint32_t graph_size_t;

typedef float state_t;

typedef uint32_t task_id_t;

typedef enum {COMPUTE, UPDATE} task_type_t;

typedef uint32_t timestamp_ms_t;
typedef uint32_t timestamp_logical_t;

#endif //KAPPA_DATATYPES_H
