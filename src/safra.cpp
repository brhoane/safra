#include "safra.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <typeinfo>

namespace _cdm {
  
  Safra::Safra(const Buechi& b) {
    root.name = 1;
    root.mark = b.initial == b.final;
    root.label.insert(b.initial);
  }
  
  void unmark(SafraNode& sn) {
    sn.mark = false;
    std::for_each(sn.children.begin(), sn.children.end(), unmark);
  }
  
  void update(transition_graph tg, int letter, SafraNode& sn) {
    std::set<int> newlabel;
    for (auto state = sn.label.cbegin(); state != sn.label.cend(); ++state) {
      transition key(state, letter);
      auto range = tg.equal_range(key);
      for (auto it = range.first; it != range.second; ++it) {
        transition_edge edge(key, *it);
        newlabel.append(edge);
      }
    }
  }
  
  Safra Safra::next_tree(int letter) {
    Safra ret(*this);
    (void)letter;
    
    unmark(ret.root);
    return ret;
  }

  bool is_safra_node(const SafraNode& sn) {
    std::set<int> seen;
    unsigned int elements = 0;
    for (auto it = sn.children.cbegin(); it != sn.children.cend(); ++it) {
      seen.insert(it->label.cbegin(), it->label.cend());
      elements += it->label.size();
    }
    if (elements != seen.size()) { // Incomparable duplicates
      return false;
    }
    bool superset = std::includes(sn.label.cbegin(), sn.label.cend(),
                             seen.cbegin(), seen.cend());
    if (!superset || sn.label.size() == seen.size()) {
      return false;
    }
    return std::all_of(sn.children.cbegin(), sn.children.cend(), is_safra_node);
  }

  bool is_safra_tree(const Safra& s) {
    return is_safra_node(s.root);
  }
  
}
