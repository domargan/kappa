#include <fstream>
#include <iostream>
#include <sstream>

#include "tsv_to_graph.h"
#include "digraph.h"

Digraph tsv_to_digraph(const std::string &tsv_file, char separator, unsigned int v_num, double init_state) {
    Digraph digraph = Digraph(v_num, init_state);

    std::fstream fs;
    fs.open(tsv_file);

    if (fs) {
        std::cout << "Opened " << tsv_file << "\nParsing file entries into Digraph..." << std::endl;

        std::string line;

        while (getline(fs, line)) {
            std::stringstream sep(line);
            std::string vertex;

            std::vector<unsigned int> vertex_pair;

            // TODO: edges needs only 2 values per field, simplify with an 1(vector)x2(array) structure
            while (getline(sep, vertex, separator)) {
                vertex_pair.push_back(static_cast<unsigned int &&>(stoi(vertex)));
            }

            digraph.add_edge(vertex_pair.at(0), vertex_pair.at(1));

            vertex_pair.clear();
        }
    }

    fs.close();

    std::cout << "Parsing finished." << std::endl;

    return digraph;
}
