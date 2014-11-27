#ifndef _cdm_AUTOMATA_HPP_
#define _cdm_AUTOMATA_HPP_

#include <map>

namespace _cdm {

  typedef std::pair<int, int> transition;
  typedef std::pair<transition, int> transition_edge;
  typedef std::multimap<transition, int> transition_graph;

  class Automaton {
  public:
    int num_letters;
    int num_states;
    int num_edges;
    transition_graph transitions;
  };

  class Buechi : public Automaton {
  public:
    Buechi(const char *filename);
    int initial;
    int final;
    friend std::ostream& operator<< (std::ostream& stream, const Buechi& aut);
  };

  class Rabin : public Automaton {
  public:
    Rabin(Buechi b);
    friend std::ostream& operator<< (std::ostream& stream, const Rabin& aut);
  };
}

#endif
