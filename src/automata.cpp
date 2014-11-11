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
          transitions.resize(num_states, std::vector<Transition>());
          break;
        case NUM_LETTERS:
          num_letters = std::stoi(line);
          break;
        case NUM_TRANSITIONS:
          tcount = std::stoi(line);
          break;
        case TRANSITIONS: {
          if (!tcount) break;
          tcount--;
          aps--;
          const char* line_ptr = &line[0];
          int state1 = atoi(line_ptr) - 1;
          line_ptr = strchr(line_ptr, ' '); assert(line_ptr);
          int state2 = atoi(line_ptr) - 1;
          line_ptr = strchr(line_ptr, ' '); assert(line_ptr);
          int letter = atoi(line_ptr) - 1;
          Transition t(letter, state2);
          transitions[state1].push_back(t);
          break;
        }
        case INITIAL:
          initial.push_back(stoi(line) - 1);
          break;
        case FINAL:
          final.push_back(stoi(line) - 1);
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

  size_t Automata::size() const {
    size_t ret = 0;
    for (auto it = transitions.cbegin(); it != transitions.cend(); ++it) {
      ret += it->size();
    }
    return ret;
  }

  std::ostream& operator <<(std::ostream& stream, const Automata& aut) {
    stream << "BUECHI\n"
           << aut.num_states << "\n"
           << aut.num_letters << "\n"
           << (aut.size()) << "\n";

    for (size_t i = 0; i < aut.transitions.size(); i++) {
      for (auto it = aut.transitions[i].cbegin();
           it != aut.transitions[i].cend(); ++it) {


        stream << i
               << ' ' << it->state
               << ' ' << it->letter
               << "\n";
      }
    }
    std::cout << "hi\n";
    stream << aut.initial[0] << "\n"
           << aut.final[0] << "\n";
    return stream;
  }
}
