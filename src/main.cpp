/**
 * @brief Reads buchi and outputs rabin automaton
 *
 * @author Brandon Hoane (bhoane)
 */

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string>
#include <iostream>

#include "automata.hpp"
#include "safra.hpp"

namespace _cdm {

  struct Options {
    bool verbose;
    const char* input_filename;
    const char* output_filename;
  };

}

using namespace _cdm;

void print_usage(const char* prog_name) {
  std::cout << "Usage: " << prog_name <<
    " input_wnfa [-v] [-o output_file]\n"
    "\n"                                                                \
    "Options:"                                                          \
    "\n"                                                                \
    "\t-v:\n"                                                           \
    "\t\tPrints generated Safra Trees and transitions\n"                \
    "\tinput_wnfa:\n"                                                   \
    "\t\tThe Buchi Automata to load and determinize.\n"                 \
    "\t-o output_file:\n"                                               \
    "\t\tThe output file in which to write the Rabin Automaton.\n";
  }

bool parse_args(Options* opt, int argc, char* argv[]) {
  if (argc < 2) {
    print_usage(argv[0]);
    return false;
  }

  opt->input_filename = argv[1];
  opt->output_filename = NULL;
  opt->verbose = false;
  int gopt;
  while ((gopt = getopt(argc, argv, "vo:")) != -1) {
    switch (gopt) {
    case 'v':
      opt->verbose = true;
      break;
    case 'o':
      opt->output_filename = optarg;
      break;
    case '?':
      if (optopt == 'o') return false;
      std::cerr << "Unknown option: '" << char(optopt) << "'\n";
      return false;
    }
  }
  return true;
}

int main(int argc, char* argv[]) {
  Options opt;

  if (!parse_args(&opt, argc, argv)) {
    return 1;
  }
  Buechi b1(opt.input_filename);
  std::cout << b1 << "\n";
  SafraGraph sg(b1);
  Rabin r1 = sg.make_rabin();

  return 0;
}
