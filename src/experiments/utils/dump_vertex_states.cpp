#include <digraph.h>
#include <fstream>
#include <iomanip>
#include "dump_vertex_states.h"

void dump_vertex_states(Digraph *g, const std::string &file_name){
    std::ofstream output_file;
    output_file.open(file_name);

    graph_size_t max_order = g->get_max_order();

    for(graph_size_t i=0; i<max_order; i++) {
        if(g->has_vertex(i)){
            output_file << std::fixed << std::setprecision(5) << g->get_state(i) << "\n";
        }
    }

    output_file.close();
}
