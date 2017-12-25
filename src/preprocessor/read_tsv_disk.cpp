#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "read_tsv_disk.h"


raw_edge_array tsv_to_edges(std::string tsv_file, char separator) {
    raw_edge_array edges;

    std::fstream fs;
    fs.open(tsv_file);

    if (fs) {

        std::cout << "Opened " << tsv_file << "\nParsing file into edge array..." << std::endl;

        std::string line;

        while (getline(fs, line)) {
            std::stringstream sep(line);
            std::string vertex;

            edges.emplace_back();

            while (getline(sep, vertex, separator)) {
                edges.back().push_back(static_cast<unsigned int &&>(stod(vertex)));
            }
        }
    }

    fs.close();

    std::cout << "Parsing finished. Number of edge array entries: " << edges.size() << std::endl;

    return raw_edge_array();
}
