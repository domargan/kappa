#include <iostream>
#include "task.h"

void Task::execute() {
  switch (task_type) {
    case ON_ACTIVATE:  // On activate (basically "compute")
#if defined(PRINT_TEST)
      std::cout << "(executing task) ON_ACTIVATE\t\t" << v << std::endl;
#endif
      g->set_scheduled(v, false);
      vertex_f(g, v);

      break;
    case ON_UPDATE:  // What to do after adding/removing and edge (e.g. run
                     // "activate")
#if defined(PRINT_TEST)
      std::cout << "(executing task) ON_UPDATE\t\t" << src << " " << dst
                << std::endl;
#endif
      edge_f(g, src, dst);

      break;
    case UPDATE:  // Just removing and adding edges
#if defined(PRINT_TEST)
      std::cout << "(executing task) UPDATE\t\t" << src << " " << dst
                << std::endl;
#endif
      update_f(g, src, dst);

      break;
  }
}

void Task::release() {
  task_pool::free(this);
}
