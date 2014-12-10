#include "safra.hpp"

#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

namespace _cdm {

  bool SafraNode::operator==(const SafraNode &other) const {
    if (name != other.name || mark != other.mark) {
      return false;
    }
    auto it = label.begin();
    auto oit = other.label.begin();
    for (; it != label.end() && oit != other.label.end(); ++it, ++oit) {
      if (*it != *oit) return false;
    }
    return it == label.end() && oit == other.label.end();
  }

  std::ostream& operator<< (std::ostream& stream, const SafraNode& sn) {
    stream << sn.name << ';';
    for (auto it = sn.label.cbegin(); it != sn.label.cend(); ++it) {
      stream << (*it) << ' ';
    }
    if (sn.mark) {
      stream << '!';
    }
    return stream;
  }

  SafraTree::SafraTree(const Buechi& b) : name_pool(b.num_states) {
    root.name = 0;
    root.mark = b.initial == b.final;
    root.label.insert(b.initial);
  }
  
  NamePool::NamePool(int buechi_size) {
    pool.resize(buechi_size * 2);
    pool[0] = false;
    for (int i = 1; i < buechi_size * 2; i++) {
      pool[i] = true;
    }
  }
  
  int NamePool::get_unused_name() {
    auto it = std::find(pool.begin(), pool.end(), true);
    assert(it != pool.end());
    int index = it - pool.begin();
    pool[index] = false;
    return index;
  }
  
  void NamePool::remove_names(SafraNode& sn) {
    assert(!pool[sn.name]);
    pool[sn.name] = true;
    for (auto it=sn.children.begin(); it != sn.children.end(); ++it) {
      remove_names(*it);
    }
  }
  
  std::ostream& operator<< (std::ostream& stream,
                                   const NamePool& np) {
    stream << "Name Pool: ";
    for (unsigned int i = 0; i < np.size(); i++) {
      stream << np[i];
    }
    stream << "\n";
    return stream;
  }
  
  bool is_safra_node(const SafraNode& sn) {
    if (sn.label.size() == 0) return false;
    
    std::set<int> seen;
    unsigned int elements = 0;
    for (auto it =sn.children.cbegin(); it != sn.children.cend(); ++it) {
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
    return std::all_of(sn.children.cbegin(), sn.children.cend(),
                       is_safra_node);
  }

  bool SafraTree::is_safra_tree() {
    if (root.name == -1) return true; // null tree
    return is_safra_node(root);
  }
  
  enum find_status find_name_node(int name, const SafraNode& sn) {
    if (sn.name == name && sn.mark) {
      return FOUND_MARKED;
    }
    bool found = sn.name == name;
    for (auto it = sn.children.cbegin(); it != sn.children.cend(); ++it) {
      enum find_status fs = find_name_node(name, *it);
      if (fs == FOUND_MARKED) return fs;
      else if (fs == FOUND) found = true;
    }
    return found ? FOUND : NOT_FOUND;
  }
  
  enum find_status SafraTree::find_name(int name) {
    return find_name_node(name, root);
  }
  
  void print_node(std::ostream& stream, const SafraNode& sn, int depth) {
    for (int i=0; i < depth; i++) {
      stream << '\t';
    }
    stream << sn << '\n';
    for (auto it =sn.children.cbegin(); it != sn.children.cend(); ++it) {
      print_node(stream, *it, depth+1);
    }
  }
  
  std::ostream& operator<< (std::ostream& stream, const SafraTree& st) {
    stream << st.name_pool;
    print_node(stream, st.root, 0);
    return stream;
  }
  
  SafraNode SafraGraph::copy_unmark_update(const SafraNode& other,
                                           int letter) {
    SafraNode ret;
    ret.name = other.name;
    ret.mark = false;
    for (auto state = other.label.cbegin();
         state != other.label.cend(); ++state) {
      transition key(*state, letter);
      auto range = buechi.edges.equal_range(key);
      for (auto it = range.first; it != range.second; ++it) {
        ret.label.insert(it->second);
      }
    }
    for (auto it = other.children.cbegin();
         it != other.children.cend(); ++it) {
      ret.children.push_back(copy_unmark_update(*it, letter));
    }
    return ret;
  }

  void SafraGraph::create(SafraNode& sn, NamePool& pool) {
    for (auto it = sn.children.begin(); it != sn.children.end(); ++it) {
      create(*it, pool);
    }
    
    if (sn.label.find(buechi.final) != sn.label.end()) {
      SafraNode child;
      child.name = pool.get_unused_name();
      child.mark = true;
      child.label.insert(buechi.final);
      sn.children.push_back(child);
    }
  }

  /* Remove members of seen from the label of sn.
   * Scans left to right, updating seen with label elements.
   * Returns true if the label does not become empty,
   * so the node should be kept.
   */
  bool scan_and_remove_seen(SafraNode& sn, std::set<int>& seen,
                            NamePool& pool) {
    // seen = labels to the left of sn
    for (auto it = seen.begin(); it != seen.end(); ++it) {
      sn.label.erase(*it);
    }
    if (sn.label.empty()) {
      return false;
    }
    //seen.insert(sn.label.begin(), sn.label.end());
    std::vector<SafraNode> remaining_children;
    for (auto it = sn.children.begin(); it != sn.children.end(); ++it) {
      // seen = labels to the left of sn and in children up to it
      if (scan_and_remove_seen(*it, seen, pool)) {
        remaining_children.push_back(*it);
      } else {
        pool.remove_names(*it);
      }
    }
    sn.children = remaining_children;
    seen.insert(sn.label.begin(), sn.label.end());
    return true;
  }

  void SafraGraph::horizontal_merge_and_kill(SafraNode& sn,
                                             NamePool& pool) {
    std::set<int> seen;
    if (!scan_and_remove_seen(sn, seen, pool)) {
      pool.remove_names(sn);
      sn.children.erase(sn.children.begin(), sn.children.end());
      sn.name = -1;
    }
  }

  void SafraGraph::vertical_merge(SafraNode& sn, NamePool& pool) {
    std::set<int> seen;
    for (auto it = sn.children.begin(); it != sn.children.end(); ++it) {
      seen.insert(it->label.begin(), it->label.end());
    }
    if (seen.size() == sn.label.size()) {
      sn.mark = true;
      for (auto it=sn.children.begin(); it != sn.children.end(); ++it) {
        pool.remove_names(*it);
      } 
      sn.children.erase(sn.children.begin(), sn.children.end());
    } else {
      for (auto it =sn.children.begin(); it != sn.children.end(); ++it) {
        vertical_merge(*it, pool);
      }
    }
  }

  SafraTree SafraGraph::next_tree(SafraTree& st, int letter) {
    assert(st.is_safra_tree());
    
    if (st.root.name == -1) return st;

    SafraTree ret(buechi.num_states);
    ret.name_pool = st.name_pool;
    //std::cout << st << "\n";
    ret.root = copy_unmark_update(st.root, letter);
    //std::cout << "UNMARK UPDATE\n" << ret << "\n";
    create(ret.root, ret.name_pool);
    //std::cout << "CREATE\n" << ret << "\n";
    horizontal_merge_and_kill(ret.root, ret.name_pool);
    //std::cout << "HMERGE KILL\n" << ret << "\n";
    vertical_merge(ret.root, ret.name_pool);
    //std::cout << "VMERGE\n" << ret << "\n";
    return ret;
  }
 
  Rabin SafraGraph::make_rabin() {
    std::vector<SafraTree> trees;
    trees.push_back(SafraTree(buechi));

    Rabin r;
    r.num_states = 1;
    r.num_letters = buechi.num_letters;
    r.initial = 0;
    
    // Compute the closure of the transition operation next_tree
    // trees.size() grows as we work, when we reach it we're done
    for (unsigned int i=0; i < trees.size(); i++) {
      for (int letter=0; letter < r.num_letters; letter++) {
        //std::cout << "---------("<<i<<") "<<letter<<"\n"
        //          <<trees[i]<<"\n======\n";
        SafraTree st = next_tree(trees[i], letter);

        auto find_tree = std::find(trees.begin(), trees.end(), st);
        int tree_index = find_tree - trees.begin();
        if (find_tree == trees.end()) {
          //std::cout << "("<<trees.size()<<")\n"<<st<<"\n";
          trees.push_back(st);
          tree_index = trees.size()-1;
          r.num_states++;
        }
        transition t(i, letter);
        transition_edge te(t, tree_index);
        r.edges.insert(te);
        //std::cout <<" ("<<tree_index<<")\n";
      }
    }
    r.num_edges = trees.size() * r.num_letters;
    
    // Find Rabin Pairs
    for (int i=0; i < name_pool_max; i++) {
      rabin_pair rp;
      for (unsigned int j=0; j < trees.size(); j++) {        
        switch (trees[j].find_name(i)) {
        case NOT_FOUND:
          rp.first.push_back(j);
          break;
        case FOUND_MARKED:
          rp.second.push_back(j);
          break;
        default:
          break;
        }
      }
      if (rp.second.size() > 0) {
        r.accept.push_back(rp);
      }
    }
    return r;
  }
}
