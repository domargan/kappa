#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/progress.hpp>

#include "edgelist_to_graph.h"

// TODO: Update this function so it uses tricks similar to edgelist_to_edge_array to speedup things
void edgelist_to_digraph(Digraph* digraph, const std::string &edgelist_file, char separator, graph_size_t beggining_line, graph_size_t end_line) {
    std::fstream fs;
    fs.open(edgelist_file);

    if (fs) {
        std::cout << "\n-----------------------------------------------------------------------------------------"
                     "\n[START]\t\tPopulating Digraph structure with edges from file " << edgelist_file << std::endl;

        std::string line;

        graph_size_t line_counter = 0;

        boost::progress_display show_progress(end_line - beggining_line);

        while (getline(fs, line)) {
            line_counter++;

            if(line_counter >= beggining_line && line_counter <= end_line) {
                std::stringstream sep(line);
                std::string vertex;

                std::vector<vertex_id_t> vertex_pair;

                // TODO: edges needs only 2 values per field, simplify with an 1(vector)x2(array) structure
                while (getline(sep, vertex, separator)) {
                    vertex_pair.push_back(static_cast<vertex_id_t &&>(stoi(vertex)));
                }

                digraph->add_edge_populate(vertex_pair.at(0), vertex_pair.at(1));

                vertex_pair.clear();

                ++show_progress;
            }
            else if(line_counter > end_line) {
                break;
            }
        }
    }

    fs.close();

    std::cout << "[END]\t\tFinished populating digraph with edge entries from file." << std::endl;
}
