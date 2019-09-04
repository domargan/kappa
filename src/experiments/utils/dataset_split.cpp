#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

#include "dataset_split.h"


std::vector<graph_size_t> dataset_to_batches(graph_size_t beginning, graph_size_t end, graph_size_t total_lines,
                                             graph_size_t chunk_size){
    // size is measured in the number of lines in a dataset edgelist file.

    std::cout << "\n-----------------------------------------------------------------------------------------"
                 "\n[START]\t\tSplitting dataset into batches..." << std::endl;

    end = end + 1;

    graph_size_t size = end - beginning;

    std::cout << "[INFO]\t\tTotal dataset size:\t\t\t\t\t\t" << total_lines << std::endl;
    std::cout << "[INFO]\t\tTotal selected data size:\t\t\t\t\t" << size << std::endl;

    graph_size_t chunks_num = 0;

    if(size%chunk_size == 0) {
        chunks_num += size / chunk_size;
    } else {
        chunks_num += (size / chunk_size) + 1;
    }

    std::cout << "[INFO]\t\tNumber of batches:\t\t\t\t\t\t" << chunks_num << std::endl;
    std::cout << "[INFO]\t\tBatch size (max):\t\t\t\t\t\t" << chunk_size << std::endl;

    std::vector<graph_size_t> chunks_line_marks(chunks_num+1);

    chunks_line_marks.at(0) = beginning;
    std::cout << "[INFO]\t\tChunk 1 start line:\t\t\t\t\t\t" << beginning << std::endl;

    //graph_size_t chunk_line_mark = beginning + chunk_size;
    graph_size_t chunk_line_mark = beginning;

    if(chunks_num > 1){
        for(graph_size_t i=1; i<=chunks_num; i++){
            chunk_line_mark += chunk_size;

            if (chunk_line_mark < end) {
                chunks_line_marks.at(i) = chunk_line_mark;
                std::cout << "[INFO]\t\tChunk " << i + 1 << " start line:\t\t\t\t\t\t" << chunk_line_mark << std::endl;
            } else {
                chunk_line_mark = end;
                chunks_line_marks.at(i) = chunk_line_mark;
                std::cout << "[INFO]\t\tLast chunk (" << i << ") ends before the line:\t\t\t\t" << chunk_line_mark << std::endl;
            }
        }
    } else {
        chunk_line_mark = end;
        chunks_line_marks.at(1) = chunk_line_mark;
        std::cout << "[INFO]\t\tChunk ends before the line:\t\t\t\t\t" << chunk_line_mark << std::endl;
    }

    std::cout << "[END]\t\tSplitting dataset into batches finished." << std::endl;

    return chunks_line_marks;
}

std::vector<graph_size_t> dataset_to_batches_with_core(graph_size_t beginning, graph_size_t end, graph_size_t total_lines, graph_size_t core_size,
                                                       graph_size_t chunk_size){
    // size is measured in the number of lines in a dataset edgelist file.

    std::cout << "\n-----------------------------------------------------------------------------------------"
                 "\n[START]\t\tSplitting dataset into batches..." << std::endl;

    end = end + 1;

    graph_size_t size = end - beginning;

    graph_size_t rest_size = size - core_size;

    std::cout << "[INFO]\t\tTotal dataset size:\t\t\t\t\t\t" << total_lines << std::endl;
    std::cout << "[INFO]\t\tTotal selected data size:\t\t\t\t\t" << size << std::endl;

    graph_size_t chunks_num = 1;

    if(rest_size%chunk_size == 0) {
        chunks_num += rest_size / chunk_size;
    } else {
        chunks_num += (rest_size / chunk_size) + 1;
    }

    std::cout << "[INFO]\t\tNumber of batches:\t\t\t\t\t\t" << chunks_num << std::endl;

    std::cout << "[INFO]\t\tCore batch size:\t\t\t\t\t" << core_size << std::endl;
    std::cout << "[INFO]\t\tRemaining size:\t\t\t\t\t" << rest_size << std::endl;
    std::cout << "[INFO]\t\tBatch size (max):\t\t\t\t\t\t" << chunk_size << std::endl;

    std::vector<graph_size_t> chunks_line_marks(chunks_num+1);

    chunks_line_marks.at(0) = beginning;
    std::cout << "[INFO]\t\tChunk 1 start line:\t\t\t\t\t\t" << beginning << std::endl;

    graph_size_t chunk_line_mark = beginning + core_size;

    chunks_line_marks.at(1) = chunk_line_mark;
    std::cout << "[INFO]\t\tChunk 2 start line:\t\t\t\t\t\t" << chunk_line_mark << std::endl;

    if(chunks_num > 2){
        for(graph_size_t i=2; i<=chunks_num; i++){
            chunk_line_mark += chunk_size;

            if (chunk_line_mark < end) {
                chunks_line_marks.at(i) = chunk_line_mark;
                std::cout << "[INFO]\t\tChunk " << i + 1 << " start line:\t\t\t\t\t" << chunk_line_mark << std::endl;
            } else {
                chunk_line_mark = end;
                chunks_line_marks.at(i) = chunk_line_mark;
                std::cout << "[INFO]\t\tLast chunk (" << i << ") ends before the line:\t\t\t\t\t" << chunk_line_mark << std::endl;
            }
        }
    } else {
        chunk_line_mark = end;
        chunks_line_marks.at(2) = chunk_line_mark;
        std::cout << "[INFO]\t\tLast chunk (2) ends before the line:\t\t\t\t\t" << chunk_line_mark << std::endl;
    }

    std::cout << "[END]\t\tSplitting dataset into batches finished." << std::endl;

    return chunks_line_marks;
}
