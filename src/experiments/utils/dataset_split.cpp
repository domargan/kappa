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

std::vector<uint32_t> dataset_to_batches(uint32_t beginning, uint32_t end, uint32_t total_lines, uint32_t core_size,
                                         uint32_t chunk_size){
    // size is measured in the number of lines in a dataset edgelist file.

    std::cout << "Splitting dataset into batches..." << std::endl;

    end = end + 1;

    uint32_t size = end - beginning;

    uint32_t rest_size = size - core_size;

    std::cout << "Total dataset size: " << total_lines << std::endl;
    std::cout << "Total selected data size: " << size << std::endl;

    uint32_t chunks_num = 1;

    if(rest_size%chunk_size == 0) {
        chunks_num += rest_size / chunk_size;
    } else {
        chunks_num += (rest_size / chunk_size) + 1;
    }

    std::cout << "Number of batches: " << chunks_num << std::endl;

    std::cout << "Core batch size: " << core_size << std::endl;
    std::cout << "Remaining size: " << rest_size << std::endl;
    std::cout << "Batch size (max): " << chunk_size << std::endl;

    std::vector<uint32_t> chunks_line_marks(chunks_num+1);

    chunks_line_marks.at(0) = beginning;
    //std::cout << "Chunk 1 start line: " << beginning << std::endl;

    uint32_t chunk_line_mark = beginning + core_size;

    chunks_line_marks.at(1) = chunk_line_mark;
    //std::cout << "Chunk 2 start line: " << chunk_line_mark << std::endl;

    if(chunks_num > 2){
        for(uint32_t i=2; i<=chunks_num; i++){
            chunk_line_mark += chunk_size;

            if (chunk_line_mark < end) {
                chunks_line_marks.at(i) = chunk_line_mark;
                //std::cout << "Chunk " << i + 1 << " start line: " << chunk_line_mark << std::endl;
            } else {
                chunk_line_mark = end;
                chunks_line_marks.at(i) = chunk_line_mark;
                //std::cout << "Last chunk (" << i << ") ends before the line: " << chunk_line_mark << std::endl;
            }
        }
    }

    std::cout << "Splitting dataset into batches finished." << std::endl;

    return chunks_line_marks;
}