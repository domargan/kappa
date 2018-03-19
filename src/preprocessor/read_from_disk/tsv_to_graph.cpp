#include <fstream>
#include <iostream>
#include <sstream>

#include "tsv_to_graph.h"
#include "digraph.h"

Digraph tsv_to_digraph(const std::string &tsv_file, char separator, uint32_t beggining_line, uint32_t end_line, uint32_t v_num, state_t init_state, uint32_t update_batch_size) {
    Digraph digraph = Digraph(v_num, init_state, update_batch_size);

    std::fstream fs;
    fs.open(tsv_file);

    if (fs) {
        std::cout << "Opened " << tsv_file << "\nParsing file entries into Digraph..." << std::endl;

        std::string line;

        uint32_t line_counter = 0;

        while (getline(fs, line)) {
            line_counter++;

            if(line_counter >= beggining_line && line_counter <= end_line) {
                std::stringstream sep(line);
                std::string vertex;

                std::vector<uint32_t> vertex_pair;

                // TODO: edges needs only 2 values per field, simplify with an 1(vector)x2(array) structure
                while (getline(sep, vertex, separator)) {
                    vertex_pair.push_back(static_cast<uint32_t &&>(stoi(vertex)));
                }

                digraph.add_edge_populate(vertex_pair.at(0), vertex_pair.at(1));

                vertex_pair.clear();
            }
        }
    }

    fs.close();

    std::cout << "Parsing finished." << std::endl;

    return digraph;
}
