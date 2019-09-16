#include <fstream>
#include <iomanip>

#include "digraph.h"
#include "dump_vertex_states.h"

void dump_vertex_states(Digraph* g, const std::string& file_name) {
  std::ofstream output_file;
  output_file.open(file_name);

  graph_size_t max_order = g->get_max_order();

  for (graph_size_t i = 0; i < max_order; i++) {
    if (g->has_vertex(i)) {
      output_file << i << " " << g->get_component_label(i) << " " << std::fixed
                  << std::setprecision(5) << g->get_state(i) << " "
                  << g->get_vertex_update_ts(i) << " "
                  << g->get_vertex_on_update_ts(i) << " "
                  << g->get_vertex_on_activate_ts(i) << "\n";
    }
  }

  output_file.close();
}
