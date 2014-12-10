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
          int state1 = atoi(nptr) - 1;

          nptr = strtok(NULL, " "); assert(nptr);
          int letter = atoi(nptr) - 1;

          nptr = strtok(NULL, " "); assert(nptr);
          int state2 = atoi(nptr) - 1;

          transition_edge e(transition(state1,letter), state2);
          edges.insert(e);
          tcount--;
          if (tcount) aps--;
          break;
        }
        case INITIAL:
          initial = stoi(line) - 1;
          break;
        case FINAL:
          final = stoi(line) - 1;
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

  std::ostream& operator <<(std::ostream& stream,
                            const FiniteStateMachine& fsm) {
    stream << fsm.num_states << "\n"
           << fsm.num_letters << "\n"
           << fsm.num_edges << "\n";
    for (auto it = fsm.edges.cbegin(); it != fsm.edges.cend(); ++it) {
      stream << it->first.first << ' '  // from state
             << it->first.second << ' ' // letter
             << it->second << "\n";     // to state
    }
    return stream;
  }

  std::ostream& operator <<(std::ostream& stream, const Buechi& bfsm) {
    stream << "BUECHI\n";

    stream << (FiniteStateMachine)bfsm;

    stream << bfsm.initial << "\n"
           << bfsm.final << "\n";
    return stream;
  }

  std::ostream& operator <<(std::ostream& stream, const Rabin& rfsm) {
    stream << "Rabin\n";

    stream << (FiniteStateMachine)rfsm;

    for (auto it =rfsm.accept.cbegin(); it != rfsm.accept.cend(); ++it) {
      stream << "(";
      for (unsigned int j = 0; j < it->first.size(); j++) {
        stream << it->first[j];
        if (j < it->first.size()-1) stream << ",";
      }
      stream << "),(";
      for (unsigned int j = 0; j < it->second.size(); j++) {
        stream << it->second[j];
        if (j < it->second.size()-1) stream << ",";
      }
      stream << ")\n";
    }

    stream << rfsm.initial << "\n";
    return stream;
  }
  
  
  void FiniteStateMachine::graphviz_body(std::ostream& stream) {
    //for (int i=1; i<= fsm.num_states; i++) {
    //  stream << i << "[label=\"" << i << "\",peripheries=2];\n";
    //}
    for (auto it = this->edges.cbegin();
         it != this->edges.cend(); ++it) {
      stream << it->first.first << "->" << it->second
             << "[label=\"" << it->first.second << "\"];\n";
    }
  }
  
  void Buechi::graphviz_body(std::ostream& stream) {
    stream << "a[shape=none, label=\"\"];\n"
           << final << "[peripheries=2];\n"
           << "a->" << initial << "\n";
    FiniteStateMachine::graphviz_body(stream);
  }
  
  void Rabin::graphviz_body(std::ostream& stream) {
    stream << "a[shape=none, label=\"\"];\n"
           << "a->" << initial << "\n";
    FiniteStateMachine::graphviz_body(stream);
  }
  
  void FiniteStateMachine::graphviz_out(std::ostream& stream) {
    stream << "digraph G {\n";
    graphviz_body(stream);
    stream << "}\n";
  }
}
