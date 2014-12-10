#ifndef _cdm_SAFRA_HPP_
#define _cdm_SAFRA_HPP_

#include <stdlib.h>
#include <vector>
#include <set>
#include <queue>
#include <map>
#include <iostream>

#include "automata.hpp"

namespace _cdm {

  enum find_status {
    NOT_FOUND,
    FOUND,
    FOUND_MARKED
  };
  
  struct SafraNode {
    int name;
    std::set<int> label;
    bool mark;
    std::vector<SafraNode> children;
    bool operator==(const SafraNode &other) const;
  };
  std::ostream& operator<< (std::ostream& stream, const SafraNode& sn);
  
  class NamePool {
  private:
    std::vector<bool> pool;
  public:
    NamePool(int buechi_size);
    int get_unused_name();
    void remove_names(SafraNode& sn);
    unsigned int size() const { return pool.size(); }
    bool operator[] (unsigned int n) const { return pool[n]; }
  };
  
  class SafraTree {
  public:
    NamePool name_pool;
    SafraNode root;

    SafraTree() = delete;
    SafraTree(int buechi_size) : name_pool(buechi_size) {};
    SafraTree(const Buechi& b);

    bool is_safra_tree();
    enum find_status find_name(int name);
    int get_unused_name();
    bool operator==(const SafraTree &other) const {
      return root == other.root;
    }
    
    friend std::ostream& operator<< (std::ostream& stream,
                                     const SafraTree& st);
  };

  class SafraGraph {
  private:
    SafraNode copy_unmark_update(const SafraNode& other, int letter);
    void create(SafraNode& sn, NamePool& pool);
    void horizontal_merge_and_kill(SafraNode& sn, NamePool& pool);
    void vertical_merge(SafraNode& sn, NamePool& pool);
    int name_pool_max;
  public:
    Buechi buechi;

    SafraGraph() = delete;
    SafraGraph(const Buechi b) : name_pool_max(b.num_states*2),
                                 buechi(b) {}
    SafraTree next_tree(SafraTree& st, int letter);
    Rabin make_rabin();
  };
}

#endif
