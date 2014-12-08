#ifndef _cdm_VISUALIZE_HPP_
#define _cdm_VISUALIZE_HPP_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

using namespace boost;

namespace _cdm {
  struct edge_letter_t {
    typedef edge_property_tag kind;
  };
  typedef property<edge_letter_t, int> letter_property;
  typedef adjacency_list<vecS, vecS, directedS,
                         no_property, letter_property> bgl_graph;

  void print_automaton(FiniteStateMachine& fsm) {
    std::pair<int, int>* pairs= new std::pair<int,int>[fsm.num_edges+1];
    int* letters = new int[fsm.num_edges+1];
    for (auto it = fsm.edges.cbegin(), int i = 1;
         it != fsm.edges.cend(); ++it) {
      pairs[i] = std::pair<int,int>(pairs.first.first,pairs.second);
      letters[i] = pairs.first.second;
    }

    bgl_graph g(pairs, pairs + fsm.num_edges, letters, fsm.num_states+1);
  }

}

#endif
