#include "visualize.hpp"

namespace _cdm {
  
  void print_automaton(std::ostream& stream, FiniteStateMachine& fsm) {
    stream << "digraph G {\n";
    for (int i=1; i<= fsm.num_states; i++) {
      stream << i << "[label=\"" << i << "\"];\n";
    }
    for (auto it = fsm.edges.cbegin(); it != fsm.edges.cend(); ++it) {
      stream << it->first.first << "->" << it->second
             << "[label=\"" << it->first.second << "\"];\n";
    }
    stream << "}\n";
  }
}
