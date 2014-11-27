#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>

#include "automata.hpp"

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
        case NUM_STATES: {
          num_states = std::stoi(line);
          transitions = transition_graph();
          break;
        }
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
          transitions.insert(e);
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

  std::ostream& operator <<(std::ostream& stream, const Buechi& aut) {
    stream << "BUECHI\n"
           << aut.num_states << "\n"
           << aut.num_letters << "\n"
           << aut.num_edges << "\n";
    
    for (auto it = aut.transitions.cbegin(); it != aut.transitions.cend(); ++it) {
      std::cout << it->first.first << ' '
                << it->first.second << ' '
                << it->second << "\n";
    }
    
    stream << aut.initial << "\n"
           << aut.final << "\n";
    return stream;
  }

}
