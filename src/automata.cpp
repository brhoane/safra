#include <iostream>
#include <fstream>
#include <string>
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
  
  Automata::Automata(const char *filename) {
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
          transitions = new int[][num_states];
          break;
        case NUM_LETTERS:
          num_letters = std::stoi(line);
          break;
        case NUM_TRANSITIONS:
          
          aps++;
          break;
        case TRANSITIONS:
          break;
        case INITIAL:
          break;
        case FINAL:
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
}
