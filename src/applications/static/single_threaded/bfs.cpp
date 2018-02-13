#include <iostream>
#include <list>

#include "bfs.h"

// TODO: Use templates instead of function overloading

void bfs(Graph* g, unsigned int v){
    std::cout << "Starting BFS with source vertex " << v << "..." << std::endl;

    std::list<unsigned int> bfs_queue;

    g->update_state(v, 1);
    g->finalize_state(v);

    bfs_queue.push_back(v);

    while(!bfs_queue.empty()){
        v = bfs_queue.front();
        //std::cout << v << " ";
        bfs_queue.pop_front();

        for(auto neighbor : *(g->get_neighborhood(v))) {
            if(g->get_state(neighbor) != 1) {
                g->update_state(neighbor, 1);
                g->finalize_state(neighbor);
                bfs_queue.push_back(neighbor);
            }
        }
    }
}

void bfs(Digraph* g, unsigned int v){
    std::cout << "Starting BFS with source vertex " << v << "..." << std::endl;

    std::list<unsigned int> bfs_queue;

    g->update_state(v, 1);
    g->finalize_state(v);

    bfs_queue.push_back(v);

    while(!bfs_queue.empty()){
        v = bfs_queue.front();
        //std::cout << v << " ";
        bfs_queue.pop_front();

        for(auto neighbor : *(g->get_out_neighborhood(v))) {
            if(g->get_state(neighbor) != 1) {
                g->update_state(neighbor, 1);
                g->finalize_state(neighbor);
                bfs_queue.push_back(neighbor);
            }
        }
    }
}