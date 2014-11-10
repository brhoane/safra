#ifndef _cdm_AUTOMATA_HPP_
#define _cdm_AUTOMATA_HPP_

namespace _cdm {

  typedef int State;
  typedef int Letter;
  struct Transition {
    letter l;
    state s;

    Transition(): l(0),
                  s(0) {}
  };

  class Automata {
  public:
    int num_states;
    int num_letters;
    Transition *transitions[num_states];

    Automata(const char* filename) {

    }
    virtual ~Automata() {}
  }

}

#endif
