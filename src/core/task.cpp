#include "task.h"

void Task::execute() {
    switch (task_type) {
        case VERTEX:
            g->set_scheduled(v, false);
            vertex_f(g, v);

            break;
        case EDGE:
            edge_f(g, src, dst);

            break;
        case UPDATE:
            update_f(g, src, dst);

            break;
    }
}

void Task::release() {
    task_pool::free(this);
}
