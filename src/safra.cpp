#include "safra.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

namespace _cdm {
  
  SafraTree::SafraTree(const Buechi& b) {
    root.name = 1;
    root.mark = b.initial == b.final;
    root.label.insert(b.initial);
  }
  
  SafraNode SafraGraph::copy_unmark_update(const SafraNode& other, int letter) {
    SafraNode ret;
    ret.name = other.name;
    ret.mark = false;

    for (auto state = other.label.cbegin(); state != other.label.cend(); ++state) {
      transition key(*state, letter);
      auto range = buechi.edges.equal_range(key);
      for (auto it = range.first; it != range.second; ++it) {
        ret.label.insert(it->second);
      }
    }
    for (auto it = other.children.cbegin(); it != other.children.cend(); ++it) {
      ret.children.push_back(copy_unmark_update(*it, letter));
    }
    return ret;
  }
  
  SafraTree SafraGraph::next_tree(SafraTree& st, int letter) {
    SafraTree ret;
    
    ret.root = copy_unmark_update(st.root, letter);
    
    return ret;
  }

  void SafraTree::add_child(SafraNode& sn, SafraNode child) {
    child.name = name_pool.top();
    name_pool.pop();
    sn.children.push_back(child);
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

  bool SafraTree::is_safra_tree() {
    return is_safra_node(root);
  }
  
}
