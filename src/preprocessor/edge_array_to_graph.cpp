#include <boost/progress.hpp>
#include <boost/algorithm/string.hpp>

#include "edge_array_to_graph.h"

void edge_array_to_digraph(Digraph* g, raw_edge_array_t& edge_array, graph_size_t beggining_line, graph_size_t end_line) {
    std::chrono::steady_clock::time_point begin_timer = std::chrono::steady_clock::now();

    std::cout << "Populating Digraph with entries from edge array..." << std::endl;

    boost::progress_display show_progress(end_line - beggining_line);

    graph_size_t current_line = beggining_line-1;

    while (current_line < end_line) {

        g->add_edge(edge_array[current_line][0], edge_array[current_line][1]);
        ++current_line;

        ++show_progress;
    }

    std::cout << "\nFinished populating.\n" << std::endl;

    std::chrono::steady_clock::time_point end_timer = std::chrono::steady_clock::now();
    float time = std::chrono::duration<float>(end_timer - begin_timer).count();
    std::cout << "\n(CPU) DIGRAPH POPULATING TIME: " << time << std::endl;
}