#include "task.h"

void Task::execute() {
    switch (task_type) {
        case ON_ACTIVATE:                    // On activate (basically "compute")
            g->set_scheduled(v, false);
            vertex_f(g, v);

            break;
        case ON_UPDATE:                      // What to do after adding/removing and edge (e.g. run "activate")
            edge_f(g, src, dst);

            break;
        case UPDATE:                    // Just removing and adding edges
            update_f(g, src, dst);

            break;
    }
}

void Task::release() {
    task_pool::free(this);
}
