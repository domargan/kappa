#include <boost/progress.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

#include "edgelist_to_graph.h"

void edgelist_to_digraph(Digraph* digraph,
                         std::string edgelist_file,
                         int line_count) {
  std::chrono::steady_clock::time_point timer_start =
      std::chrono::steady_clock::now();
  std::cout
      << "\n-----------------------------------------------------------------"
         "------------------------"
         "\n[START]\t\tPopulating Digraph structure with edges from file "
      << edgelist_file << std::endl;

  boost::iostreams::mapped_file_source file(edgelist_file);
  std::string fileContent(file.data(), file.size());
  file.close();

  std::istringstream iss{fileContent};
  std::string line;
  int line_idx = 0;

  boost::progress_display show_progress(line_count);

  while (std::getline(iss, line)) {
    int pos = line.find_first_of(' ');

    digraph->add_edge_populate(stoi(line.substr(0, pos)),
                               stoi(line.substr(pos + 1)));

    ++line_idx;
    ++show_progress;
  }

  fileContent.clear();

  std::cout
      << "[END]\t\tFinished populating digraph with edge entries from file."
      << std::endl;

  std::chrono::steady_clock::time_point timer_end =
      std::chrono::steady_clock::now();
  float time_read_file =
      std::chrono::duration<float>(timer_end - timer_start).count();
  std::cout << "[TIME]\t\tPopulating digraph with edges from file:\t\t\t\t\t"
            << time_read_file << std::endl;
}