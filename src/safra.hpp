#ifndef _cdm_SAFRA_HPP_
#define _cdm_SAFRA_HPP_

#include <stdlib.h>
#include <vector>
#include <set>
#include <queue>

#include "automata.hpp"

namespace _cdm {
  
  struct SafraNode {
    int name;
    std::set<int> label;
    bool mark;
    std::vector<SafraNode> children;
  };
  
  //  typedef std::priority_queue<int, Compare = less<typename Container::value_type>>
  //  int_pool;
  
  class SafraTree {
  private:
    std::priority_queue<int> name_pool;
  public:
    SafraNode root;
    
    SafraTree() = default;
    SafraTree(const Buechi& b);
    
    void add_child(SafraNode& sn, SafraNode child);    
    bool is_safra_tree();
  };
  
  class SafraGraph {
  public:
    Buechi buechi;
    
    SafraGraph() = delete;
    SafraGraph(const Buechi b) : buechi(b) {}
    
    SafraNode copy_unmark_update(const SafraNode& other, int letter);
    SafraTree next_tree(SafraTree& st, int letter);
  };
}

#endif
