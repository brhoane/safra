#ifndef _cdm_AUTOMATA_HPP_
#define _cdm_AUTOMATA_HPP_

#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace _cdm {

  typedef std::pair<int, int> transition;
  typedef std::pair<transition, int> transition_edge;
  typedef std::multimap<transition, int> transition_graph;
  typedef std::pair<std::vector<int>, std::vector<int> > rabin_pair;

  class FiniteStateMachine {
  public:
    int num_letters;
    int num_states;
    int num_edges;
    transition_graph edges;

    FiniteStateMachine() {}
    
    void graphviz_out(std::ostream& stream);
    friend std::ostream& operator<< (std::ostream& stream,
                                     const FiniteStateMachine& fsm);
    virtual void graphviz_body(std::ostream& stream);
  };

  class Buechi : public FiniteStateMachine {
  public:
    int initial;
    int final;

    Buechi(const char* filename);

    virtual void graphviz_body(std::ostream& stream);
    friend std::ostream& operator<< (std::ostream& stream,
                                     const Buechi& bfsm);
  };

  class Rabin : public FiniteStateMachine {
  public:
    int initial;
    std::vector<rabin_pair> accept;
    virtual void graphviz_body(std::ostream& stream);
    friend std::ostream& operator<< (std::ostream& stream,
                                     const Rabin& rfsm);
  };
}

#endif
