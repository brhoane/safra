#ifndef _cdm_AUTOMATA_HPP_
#define _cdm_AUTOMATA_HPP_

#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/properties.hpp>

namespace _cdm {

  typedef boost::property<boost::edge_name_t, int> edge_property;

  typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS,
    boost::no_property, edge_property>
  transition_graph;

  class Automaton {
  public:
    int num_letters;
    transition_graph transitions;
    int initial;
    int final;

    Automaton(const char *filename);
    virtual ~Automaton() {}
    size_t num_edges() const;
    size_t num_states() const;
    friend std::ostream& operator<< (std::ostream& stream, const Automaton& aut);
  };


}

#endif
