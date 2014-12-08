#ifndef _cdm_VISUALIZE_HPP_
#define _cdm_VISUALIZE_HPP_

#include <stdlib.h>
#include <unistd.h>
#include <ostream>

#include "automata.hpp"

namespace _cdm {

  void print_automaton(std::ostream& stream, FiniteStateMachine& fsm);

}

#endif
