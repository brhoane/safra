#ifndef _cdm_VISUALIZE_HPP_
#define _cdm_VISUALIZE_HPP_

#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

#include "automata.hpp"

using namespace boost;

namespace _cdm {
  struct edge_letter_t {
    typedef edge_property_tag kind;
  };
  typedef property<edge_letter_t, int> letter_property;
  typedef adjacency_list<vecS, vecS, directedS,
                         no_property, letter_property> bgl_graph;

  void print_automaton(FiniteStateMachine& fsm);

}

#endif
