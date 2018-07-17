#ifndef KAPPA_TASK_H
#define KAPPA_TASK_H

#include "datatypes.h"
#include "digraph.h"

struct Task {

    timestamp_ms_t timestamp_ms;
    timestap_logical_t timestap_logical;

    //task_function;
    //task_data;

    Task(void(*)(vertex_id_t, Digraph*)) {

    }

    void set_task_type();

    void set_timestamp_ms();
    void set_timestamp_logical();

    void set_task_function();
    void set_task_data();
};

#endif //KAPPA_TASK_H
