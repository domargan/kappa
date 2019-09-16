#ifndef KAPPA_TASK_H
#define KAPPA_TASK_H

#include <boost/pool/singleton_pool.hpp>

#include "datatypes.h"
#include "digraph.h"

class Task {
 public:
  void execute();
  void release();

  task_type_t task_type;
  // timestamp_logical_t timestamp_logical;

  void (*vertex_f)(Digraph*, vertex_id_t);
  void (*edge_f)(Digraph*, vertex_id_t, vertex_id_t);
  void (*update_f)(Digraph*, vertex_id_t, vertex_id_t);

  Digraph* g;
  vertex_id_t v;
  vertex_id_t src;
  vertex_id_t dst;
};

typedef boost::singleton_pool<Task, sizeof(Task)> task_pool;

#endif  // KAPPA_TASK_H
