
#include <string>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>

#include "safra.hpp"

namespace _cdm {
  
  Safra::Safra(const Buechi& b) {
    root = std::unique_ptr<safra_node>(new safra_node);
    root->name = 1;
    root->mark = b.initial == b.final;
    root->label.insert(b.initial);
  }

  Safra::Safra(const Safra& s, int l) {
    (void)s;
    (void)l;
  }
  
  bool is_safra_tree(const Safra& s) {
    (void)s;
    return false;
  }

  bool is_safra_node(const safra_node& sn) {
    std::set<int> seen;
    unsigned int elements = 0;
    for (auto it = sn.children.cbegin(); it != sn.children.cend(); ++it) {
      seen.insert(it->label.cbegin(), it->label.cend());
      elements += it->label.size();
    }
    if (elements != seen.size()) { // Incomparable duplicates
      return false;
    }
    bool superset = includes(sn.label.cbegin(), sn.label.cend(),
                             seen.cbegin(), seen.cend());
    return superset && sn.label.size() > seen.size();
  }

}
