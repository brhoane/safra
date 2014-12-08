#include "visualize.hpp"

namespace _cdm {
  
  //template <class Name>
  class label_writer {
  public:
    template <class VertexOrEdge>
    void operator() (std::ostream& out, const VertexOrEdge& v) const {
      out << "[label=\"" << v << "\"]";
    }
  };
  
  void print_automaton(FiniteStateMachine& fsm) {
    std::cout << "LEN : " << fsm.num_edges << "\n";
    std::pair<int, int>* pairs= new std::pair<int,int>[fsm.num_edges+1];
    int* letters = new int[fsm.num_edges+1];
    int i = 1;

    for (auto it = fsm.edges.cbegin(); it != fsm.edges.cend(); ++it) {
      pairs[i] = std::pair<int,int>(it->first.first, it->second);
      letters[i] = it->first.second;
    }
    
    bgl_graph g(pairs, pairs + fsm.num_edges, letters, fsm.num_states+1);
    label_writer lw;
    std::ofstream fout("graph.dot");
    write_graphviz(fout, g, lw);
  }
}
