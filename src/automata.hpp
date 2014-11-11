#ifndef _cdm_AUTOMATA_HPP_
#define _cdm_AUTOMATA_HPP_

#include <vector>

namespace _cdm {

  struct Transition {
    int letter;
    int state;

    Transition(int l, int s): letter(l),
                              state(s) {}
  };
  typedef std::vector<std::vector<Transition>> TransitionSet;
  typedef std::vector<int> StateList;

  class Automata {
  public:
    int num_states;
    int num_letters;
    TransitionSet transitions;
    StateList initial;
    StateList final;

    Automata(const char *filename);
    virtual ~Automata() {}
    size_t size() const;
    friend std::ostream& operator<< (std::ostream& stream, const Automata& aut);
  };

}

#endif
