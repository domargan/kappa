#include <boost/progress.hpp>
#include "edge_array_to_graph.h"

void edge_array_to_digraph(Digraph* g, raw_edge_array_t& edge_array, graph_size_t beggining_line, graph_size_t end_line) {
    std::cout << "Populating Digraph with entries from edge array..." << std::endl;

    boost::progress_display show_progress(end_line - beggining_line);

    graph_size_t current_line = beggining_line-1;

    while (current_line < end_line) {

        g->add_edge(edge_array[current_line][0], edge_array[current_line][1]);
        ++current_line;

        ++show_progress;
    }

    std::cout << "\nFinished populating.\n" << std::endl;
}
