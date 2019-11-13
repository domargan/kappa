#include <algorithm>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/progress.hpp>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "edgelist_to_update_array.h"

raw_update_array_t edgelist_to_update_array(std::string edgelist_file,
                                            int line_count) {
  std::chrono::steady_clock::time_point timer_start =
      std::chrono::steady_clock::now();
  std::cout << "\n-------------------------------------------------------------"
               "----------------------------"
               "\n[START]\t\tParsing dataset into update array..."
            << std::endl;

  boost::iostreams::mapped_file_source file(edgelist_file);
  std::string fileContent(file.data(), file.size());
  file.close();

  raw_update_array_t update_array(line_count);
  // update_array.reserve(line_count);

  std::istringstream iss{fileContent};
  std::string line;
  int line_idx = 0;

  boost::progress_display show_progress(line_count);

  while (std::getline(iss, line)) {
    std::vector<std::string> splitted_line;
    boost::split(splitted_line, line, [](char c) { return c == ' '; });

    update_array[line_idx][0] = stoi(splitted_line[0]);
    update_array[line_idx][1] = stoi(splitted_line[1]);
    update_array[line_idx][2] = stoi(splitted_line[2]);

    ++line_idx;
    ++show_progress;
  }

  fileContent.clear();

  std::cout << "[END]\t\tFinished parsing dataset into edge array."
            << std::endl;

  std::chrono::steady_clock::time_point timer_end =
      std::chrono::steady_clock::now();
  float time_read_file =
      std::chrono::duration<float>(timer_end - timer_start).count();
  std::cout << "[TIME]\t\tDataset reading and parsing:\t\t\t\t\t"
            << time_read_file << std::endl;

  return update_array;
}