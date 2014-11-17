#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>

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

  Automaton::Automaton(const char* filename) {
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
          int num_states = std::stoi(line);
          transitions = Transitions(num_states);
          break;
        }
        case NUM_LETTERS:
          num_letters = std::stoi(line);
          break;
        case NUM_TRANSITIONS:
          tcount = std::stoi(line);
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

          boost::add_edge(state1, state2, letter, transitions);
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

  size_t Automaton::num_edges() const {
    return boost::num_edges(transitions);
  }

  size_t Automaton::num_states() const {
    return boost::num_vertices(transitions);
  }

  std::ostream& operator <<(std::ostream& stream, const Automaton& aut) {
    stream << "BUECHI\n"
           << (aut.num_states()) << "\n"
           << aut.num_letters << "\n"
           << (aut.num_edges()) << "\n";

    boost::graph_traits<Transitions>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = boost::edges(aut.transitions); ei != ei_end; ++ei) {

      int letter = boost::get(boost::edge_name_t(), aut.transitions, *ei);
      std::cout << source(*ei, aut.transitions) << ' '
                << letter << ' '
                << target(*ei, aut.transitions) << "\n";
    }

    stream << aut.initial << "\n"
           << aut.final << "\n";
    return stream;
  }

}
