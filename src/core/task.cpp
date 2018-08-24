#include "task.h"

boost::object_pool<VertexComputeTask> VertexComputeTask::pool;

VertexComputeTask::VertexComputeTask(void (*f)(Digraph*, vertex_id_t), Digraph *g, vertex_id_t v)
    : f{f},
      g{g},
      v{v} {}

void VertexComputeTask::execute() {
    f(g, v);
}

void VertexComputeTask::release() {
    pool.free(this);
}

boost::object_pool<EdgeComputeTask> EdgeComputeTask::pool;

EdgeComputeTask::EdgeComputeTask(void (*f)(Digraph*, vertex_id_t, vertex_id_t), Digraph *g, std::tuple<vertex_id_t, vertex_id_t> edge)
    : f{f},
      g{g},
      src{std::get<0>(edge)},
      dst{std::get<1>(edge)} {}

void EdgeComputeTask::execute() {
    f(g, src, dst);
}

void EdgeComputeTask::release() {
    pool.free(this);
}
