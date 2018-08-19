#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/progress.hpp>
#include <digraph.h>

#include "preload_states.h"


void preload_states(Digraph* digraph, const std::string &states_file, char separator, graph_size_t beggining_line, graph_size_t end_line) {
    std::fstream fs;
    fs.open(states_file);

    if (fs) {
        std::cout << "Opened " << states_file << "\nUpdating core vertex states with entries from file..." << std::endl;

        std::string line;

        graph_size_t line_counter = 0;

        boost::progress_display show_progress(end_line - beggining_line);

        while (getline(fs, line)) {
            line_counter++;

            if(line_counter >= beggining_line && line_counter <= end_line) {
                std::stringstream sep(line);
                std::string vertex_id;
                std::string state;

                getline(sep, vertex_id, separator);
                getline(sep, state, separator);

                digraph->set_state(static_cast<vertex_id_t &&>(stoi(vertex_id)), static_cast<state_t &&>(stod(state)));

                ++show_progress;
            }
            else if(line_counter > end_line) {
                break;
            }
        }
    }

    fs.close();

    std::cout << "\nFinished populating." << std::endl;
}
