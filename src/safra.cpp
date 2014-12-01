#include "safra.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

namespace _cdm {
  
  std::ostream& operator<< (std::ostream& stream, const SafraNode& sn) {
    stream << sn.name << ';';
    for (auto it = sn.label.cbegin(); it != sn.label.cend(); ++it) {
      stream << (*it) << ',';
    }
    return stream;
  }
  
  SafraTree::SafraTree(const Buechi& b) {
    root.name = 1;
    root.mark = b.initial == b.final;
    root.label.insert(b.initial);
    
    for (int i = 2; i < b.num_states; i++) {
      name_pool.push(i);
    }
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

  void SafraGraph::create(SafraNode& sn, int_pool& pool) {
    for (auto it = sn.children.begin(); it != sn.children.end(); ++it) {
      create(*it, pool);
    }

    if (sn.label.find(buechi.final) != sn.label.end()) {
      SafraNode child;
      child.name = pool.top();
      pool.pop();
      child.mark = true;
      child.label.insert(buechi.final);
      sn.children.push_back(child);
    }
  }
  
  /* Remove members of seen from the label of sn.
   * Scans left to right, updating seen with label elements.
   * Returns true if the label becomes empty and the node should be removed.
   */
  bool scan_and_remove_seen(SafraNode& sn, std::set<int>& seen) {
    sn.label.erase(seen.begin(), seen.end());
    if (sn.label.empty()) {
      return true;
    }
    //seen.insert(sn.label.begin(), sn.label.end());
    std::vector<std::vector<SafraNode>::iterator> remove;
    for (auto it = sn.children.begin(); it != sn.children.end(); ++it) {
      if (scan_and_remove_seen(*it, seen)) {
        remove.push_back(it);
      }
    }
    
    return false;
  }
  
  void SafraGraph::horizontal_merge_and_kill(SafraNode& sn) {
    std::set<int> seen;
    scan_and_remove_seen(sn, seen);
  }
  
  SafraTree SafraGraph::next_tree(SafraTree& st, int letter) {
    SafraTree ret;    
    ret.root = copy_unmark_update(st.root, letter);
    return ret;
  }
 
}
