#include <fstream>
#include <iostream>
#include <vector>

#include "dataset_split.h"


uint32_t number_of_lines(std::string tsv_file) {
    uint32_t number_of_lines = 0;

    std::ifstream fs;
    fs.open(tsv_file);

    std::string line;

    while (std::getline(fs, line))
        ++number_of_lines;

    std::cout << "Number of lines in tsv file: " << number_of_lines << std::endl;

    fs.close();

    return number_of_lines;
}

std::vector<uint32_t> dataset_split_by_size(uint32_t total_lines, uint32_t core_size, uint32_t chunk_size){
    // Size is measured in the number of lines in a dataset edgelist file.
    // One line in a dateset file contains data for a single edge.

    std::cout << "Splitting dataset..." << std::endl;

    uint32_t rest_size = total_lines - core_size;

    std::cout << "Total dataset size: " << total_lines << std::endl;

    uint32_t chunks_num = 1;

    if(rest_size%chunk_size == 0) {
        chunks_num += rest_size / chunk_size;
    } else {
        chunks_num += (rest_size / chunk_size) + 1;
    }

    std::cout << "Number of dataset chunks: " << chunks_num << std::endl;

    std::cout << "Core chunk size: " << core_size << std::endl;
    std::cout << "Chunk size (max): " << chunk_size << std::endl;


    std::vector<uint32_t> chunks_start_lines(chunks_num);

    chunks_start_lines.at(0) = 1;
    //std::cout << "Chunk 0 start line: 1" << std::endl;

    uint32_t chunk_line_mark = core_size+1;

    chunks_start_lines.at(1) = chunk_line_mark;
    //std::cout << "Chunk 1 start line: " << chunk_line_mark << std::endl;

    if(chunks_num >= 2){
        for(uint32_t i=2; i<chunks_num; i++){
            chunk_line_mark += chunk_size;
            chunks_start_lines.at(i) = chunk_line_mark;
            //std::cout << "Chunk " << i << " start line: " << chunk_line_mark << std::endl;
        }
    }

    return chunks_start_lines;
}