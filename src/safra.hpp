#ifndef _cdm_SAFRA_HPP_
#define _cdm_SAFRA_HPP_


#include <stdlib.h>
#include <vector>
#include <set>
#include <memory>

#include "automata.hpp"

namespace _cdm {
  
  struct SafraNode {
    int name;
    std::set<int> label;
    bool mark;
    std::vector<SafraNode> children;
  };
  
  class Safra {
  public:
    SafraNode root;
    
    Safra(const Buechi& b);
    
    Safra next_tree(int letter);
  };
  
  bool is_safra_tree(const Safra& s);
}

#endif
