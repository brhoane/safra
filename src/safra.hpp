#ifndef _cdm_SAFRA_HPP_
#define _cdm_SAFRA_HPP_

#include <vector>
#include <set>
#include <memory>
#include <stdlib.h>

#include "automata.hpp"

namespace _cdm {
  
  struct safra_node {
    int name;
    std::set<int> label;
    bool mark;
    std::vector<safra_node> children;
  };
  
  class Safra {
  public:
    Safra(const Buechi& b);
    Safra(const Safra& s, int l);
    std::unique_ptr<safra_node> root;
    friend std::ostream& operator<< (std::ostream& stream, const Safra& aut);
  };
    
  bool is_safra_tree(const Safra& s);
}

#endif
