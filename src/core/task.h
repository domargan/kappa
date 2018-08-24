#ifndef KAPPA_TASK_H
#define KAPPA_TASK_H

#include <boost/pool/object_pool.hpp>
#include <iostream>

#include "datatypes.h"
#include "digraph.h"

class BaseTask {
public:
    virtual void execute() = 0;
    virtual void release() = 0;
};

class VertexComputeTask : public BaseTask {
public:
    static boost::object_pool<VertexComputeTask> pool;

    VertexComputeTask(void (*)(Digraph*, vertex_id_t), Digraph*, vertex_id_t);

    void execute();
    void release();

    void (*f)(Digraph*, vertex_id_t);
    Digraph *g;
    vertex_id_t v;
};

class EdgeComputeTask : public BaseTask {
public:
    static boost::object_pool<EdgeComputeTask> pool;

    EdgeComputeTask(void (*)(Digraph*, vertex_id_t, vertex_id_t), Digraph*, std::tuple<vertex_id_t, vertex_id_t>);

    void execute();
    void release();

    void (*f)(Digraph*, vertex_id_t, vertex_id_t);
    Digraph *g;
    vertex_id_t src;
    vertex_id_t dst;
};

#endif //KAPPA_TASK_H
