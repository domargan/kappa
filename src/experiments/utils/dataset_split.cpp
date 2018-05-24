#include <fstream>
#include <iostream>
#include <vector>

#include "dataset_split.h"

graph_size_t number_of_lines(std::string edgelist_file) {
    graph_size_t number_of_lines = 0;

    std::ifstream fs;
    fs.open(edgelist_file);

    std::string line;

    while (std::getline(fs, line))
        ++number_of_lines;

    std::cout << "Number of lines in edgelist file: " << number_of_lines << std::endl;

    fs.close();

    return number_of_lines;
}

std::vector<graph_size_t> dataset_to_batches(graph_size_t beginning, graph_size_t end, graph_size_t total_lines, graph_size_t core_size,
                                         graph_size_t chunk_size){
    // size is measured in the number of lines in a dataset edgelist file.

    std::cout << "Splitting dataset into batches..." << std::endl;

    end = end + 1;

    graph_size_t size = end - beginning;

    graph_size_t rest_size = size - core_size;

    std::cout << "Total dataset size: " << total_lines << std::endl;
    std::cout << "Total selected data size: " << size << std::endl;

    graph_size_t chunks_num = 1;

    if(rest_size%chunk_size == 0) {
        chunks_num += rest_size / chunk_size;
    } else {
        chunks_num += (rest_size / chunk_size) + 1;
    }

    std::cout << "Number of batches: " << chunks_num << std::endl;

    std::cout << "Core batch size: " << core_size << std::endl;
    std::cout << "Remaining size: " << rest_size << std::endl;
    std::cout << "Batch size (max): " << chunk_size << std::endl;

    std::vector<graph_size_t> chunks_line_marks(chunks_num+1);

    chunks_line_marks.at(0) = beginning;
    std::cout << "Chunk 1 start line: " << beginning << std::endl;

    graph_size_t chunk_line_mark = beginning + core_size;

    chunks_line_marks.at(1) = chunk_line_mark;
    std::cout << "Chunk 2 start line: " << chunk_line_mark << std::endl;

    if(chunks_num > 2){
        for(graph_size_t i=2; i<=chunks_num; i++){
            chunk_line_mark += chunk_size;

            if (chunk_line_mark < end) {
                chunks_line_marks.at(i) = chunk_line_mark;
                std::cout << "Chunk " << i + 1 << " start line: " << chunk_line_mark << std::endl;
            } else {
                chunk_line_mark = end;
                chunks_line_marks.at(i) = chunk_line_mark;
                std::cout << "Last chunk (" << i << ") ends before the line: " << chunk_line_mark << std::endl;
            }
        }
    } else {
        chunk_line_mark = end;
        chunks_line_marks.at(2) = chunk_line_mark;
        std::cout << "Last chunk (2) ends before the line : " << chunk_line_mark << std::endl;
    }

    std::cout << "Splitting dataset into batches finished." << std::endl;

    return chunks_line_marks;
}
