#include <iostream>
#include <list>
#include <vector>
#include <algorithm>

#include "sssp.h"

// TODO: Idea: Instead of having 1 or 0 for states (from BFS), have previous vertex ids as states

// TODO: FIX: BFS path output differs with the different orderings of neighbors in the neighbor vector (Graph Class)
// TODO: FIX: int -> uint32_t (can't have unsigned because of initialisation to -1 in "previous" vector)

std::vector<int> bfs_previously_visited(Graph* g, int source_v){
    std::cout << "Starting BFS for source vertex " << source_v << "..." << std::endl;

    std::list<int> bfs_queue;
    std::vector<int> previous(g->get_order(), -1);

    g->update_state(source_v, 1);
    g->finalize_state(source_v);

    bfs_queue.push_back(source_v);

    while(!bfs_queue.empty()){
        int v = bfs_queue.front();
        //std::cout << v << " ";
        bfs_queue.pop_front();

        for(auto neighbor : *(g->get_neighborhood(v))) {
            if(g->get_state(neighbor) != 1) {
                g->update_state(neighbor, 1);
                g->finalize_state(neighbor);

                previous[neighbor] = v; //visited neighbor right after v

                bfs_queue.push_back(neighbor);
            }
        }
    }

    return previous;
}

std::list<int> shortest_path(Graph* g, std::vector<int> previous, int src_v, int dst_v) {
    // To get the shortest path from src_v to a dst_v,
    // start with previous[v] and backtrack until reaching the src_v.

    std::list<int> shortest_path;

    int current_v = dst_v;

    while(previous[current_v] != -1){
        shortest_path.push_back(current_v);
        current_v = previous[current_v];
    }

    // Uncomment if want to have src_v in the path:
    //shortest_path.push_back(src_v);

    shortest_path.reverse();
    //TODO: Use stack-like structure to avoid reversing list.

    return shortest_path;
}

std::vector<std::list<int>> sssp_compute(Graph* g, int src_v){

    std::vector<int> previous = bfs_previously_visited(g, src_v);

    std::vector<std::list<int>> shortest_paths(g->get_order());

    for(int dst_v=0; dst_v<shortest_paths.size(); dst_v++){
        shortest_paths[dst_v] = shortest_path(g, previous, src_v, dst_v);
    }

    return shortest_paths;
}