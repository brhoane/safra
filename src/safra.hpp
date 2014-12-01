#ifndef _cdm_SAFRA_HPP_
#define _cdm_SAFRA_HPP_

#include <stdlib.h>
#include <vector>
#include <set>
#include <queue>
#include <iostream>

#include "automata.hpp"

namespace _cdm {
  
  struct SafraNode {
    int name;
    std::set<int> label;
    bool mark;
    std::vector<SafraNode> children;
  };
  
  std::ostream& operator<< (std::ostream& stream, const SafraNode& sn);
  
  typedef std::priority_queue<int, std::vector<int>, std::greater<int>>
    int_pool;
  
  class SafraTree {
  public:
    int_pool name_pool;
    SafraNode root;
    
    SafraTree() = default;
    SafraTree(const Buechi& b);
    
    bool is_safra_tree();
  };
  
  class SafraGraph {
  private:
    SafraNode copy_unmark_update(const SafraNode& other, int letter);
    void create(SafraNode& sn, int_pool& pool);
  public:
    Buechi buechi;
    
    SafraGraph() = delete;
    SafraGraph(const Buechi b) : buechi(b) {}
    
    SafraTree next_tree(SafraTree& st, int letter);
  };
}

#endif
