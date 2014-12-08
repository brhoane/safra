#include "automata.hpp"

#include <string.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace _cdm {
  // Automata Parsing State
  enum APS {
    MAGIC,
    NUM_STATES,
    NUM_LETTERS,
    NUM_TRANSITIONS,
    TRANSITIONS,
    INITIAL,
    FINAL
  };

  Buechi::Buechi(const char* filename) {
    std::string line;
    std::ifstream myfile(filename);
    int aps = MAGIC;
    int tcount = 0;

    if (myfile.is_open()) {
      while (getline(myfile,line)) {
        if (line[0] == '#') continue;
        switch (aps) {
        case MAGIC:
          assert(line.compare("BUECHI") == 0);
          break;
        case NUM_STATES:
          num_states = std::stoi(line);
          break;
        case NUM_LETTERS:
          num_letters = std::stoi(line);
          break;
        case NUM_TRANSITIONS:
          tcount = std::stoi(line);
          num_edges = tcount;
          if (!tcount) aps++;
          break;
        case TRANSITIONS: {
          char *line_ptr = &line[0];
          char *nptr = strtok(line_ptr, " ");
          int state1 = atoi(nptr);

          nptr = strtok(NULL, " "); assert(nptr);
          int letter = atoi(nptr);

          nptr = strtok(NULL, " "); assert(nptr);
          int state2 = atoi(nptr);

          transition_edge e(transition(state1,letter), state2);
          edges.insert(e);
          tcount--;
          if (tcount) aps--;
          break;
        }
        case INITIAL:
          initial = stoi(line);
          break;
        case FINAL:
          final = stoi(line);
          break;
        default:
          break;
        }
        aps++;
      }
      myfile.close();
    }
    else {
      std::cerr << "Unable to open file " << filename << "\n";
      abort();
    }
  }

  std::ostream& operator <<(std::ostream& stream, const FiniteStateMachine& fsm) {
    for (auto it = fsm.edges.cbegin(); it != fsm.edges.cend(); ++it) {
      stream << it->first.first << ' '  // from state
             << it->first.second << ' ' // letter
             << it->second << "\n";     // to state
    }
    return stream;
  }

  std::ostream& operator <<(std::ostream& stream, const Buechi& bfsm) {
    stream << "BUECHI\n"
           << bfsm.num_states << "\n"
           << bfsm.num_letters << "\n"
           << bfsm.num_edges << "\n";

    stream << (FiniteStateMachine)bfsm;

    stream << bfsm.initial << "\n"
           << bfsm.final << "\n";
    return stream;
  }

}
