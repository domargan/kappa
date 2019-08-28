#include <boost/progress.hpp>
#include <boost/algorithm/string.hpp>

#include "edge_array_to_graph.h"

void edge_array_to_digraph(Digraph* g, raw_edge_array_t& edge_array, graph_size_t beggining_line, graph_size_t end_line) {
    std::chrono::steady_clock::time_point timer_start = std::chrono::steady_clock::now();

    std::cout << "\n-----------------------------------------------------------------------------------------"
                 "\n[START]\t\tPopulating digraph with entries from edge array..." << std::endl;

    boost::progress_display show_progress(end_line - beggining_line);

    graph_size_t current_line = beggining_line-1;

    while (current_line < end_line) {

        g->add_edge(edge_array[current_line][0], edge_array[current_line][1]);
        ++current_line;

        ++show_progress;
    }

    std::cout << "[END]\t\tFinished populating digraph." << std::endl;

    std::chrono::steady_clock::time_point timer_end = std::chrono::steady_clock::now();
    float time = std::chrono::duration<float>(timer_end - timer_start).count();
    std::cout << "[TIME]\t\tPopulating digraph:\t\t\t\t\t\t" << time << std::endl;
}