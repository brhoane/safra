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

  SafraTree::SafraTree(const Buechi& b) {
    root.name = 1;
    root.mark = b.initial == b.final;
    root.label.insert(b.initial);

    for (int i = 2; i <= b.num_states * 2; i++) {
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

  void print_node(std::ostream& stream, const SafraNode& sn, int depth) {
    for (int i=0; i < depth; i++) {
      stream << '\t';
    }
    stream << sn << '\n';
    for (auto it = sn.children.cbegin(); it != sn.children.cend(); ++it) {
      print_node(stream, *it, depth+1);
    }
  }

  std::ostream& operator<< (std::ostream& stream, const SafraTree& st) {
    print_node(stream, st.root, 0);
    return stream;
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
   * Returns true if the label does not become empty and the node should be kept.
   */
  bool scan_and_remove_seen(SafraNode& sn, std::set<int>& seen) {
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
      if (scan_and_remove_seen(*it, seen)) {
        remaining_children.push_back(*it);
      }
    }
    sn.children = remaining_children;
    seen.insert(sn.label.begin(), sn.label.end());
    return true;
  }

  void SafraGraph::horizontal_merge_and_kill(SafraNode& sn) {
    std::set<int> seen;
    scan_and_remove_seen(sn, seen);
  }

  void SafraGraph::vertical_merge(SafraNode& sn) {
    std::set<int> seen;
    for (auto it = sn.children.begin(); it != sn.children.end(); ++it) {
      seen.insert(it->label.begin(), it->label.end());
    }
    if (seen.size() == sn.label.size()) {
      sn.mark = true;
      sn.children.erase(sn.children.begin(), sn.children.end());
    } else {
      for (auto it = sn.children.begin(); it != sn.children.end(); ++it) {
        vertical_merge(*it);
      }
    }
  }

  SafraTree SafraGraph::next_tree(SafraTree& st, int letter) {
    assert(st.is_safra_tree());

    SafraTree ret;
    //std::cout << "LETTER - " << letter << "\n";
    ret.root = copy_unmark_update(st.root, letter);
    //std::cout << "UNMARK UPDATE\n" << ret << "\n";
    ret.name_pool = int_pool(st.name_pool);
    create(ret.root, ret.name_pool);
    //std::cout << "CREATE\n" << ret << "\n";
    horizontal_merge_and_kill(ret.root);
    //std::cout << "HMERGE KILL\n" << ret << "\n";
    vertical_merge(ret.root);
    //std::cout << "VMERGE\n" << ret << "\n";
    return ret;
  }

  Rabin SafraGraph::make_rabin() {
    std::vector<SafraTree> trees;
    trees.push_back(SafraTree(buechi));

    Rabin r;
    r.num_states = 1;
    r.num_letters = buechi.num_letters;

    for (unsigned int i=0; i < trees.size(); i++) {
      for (int letter=1; letter <= r.num_letters; letter++) {
        SafraTree st = next_tree(trees[i], letter);


        auto find_tree = std::find(trees.begin(), trees.end(), st);
        int tree_index = find_tree - trees.begin();
        if (find_tree == trees.end()) {
          trees.push_back(st);
          tree_index = trees.size()-1;
        }
        std::cout << i <<" EDGE "<<letter<<" TO "<<tree_index<<"\n";
        transition t(i, letter);
        transition_edge te(t, tree_index);
        r.edges.insert(te);
      }
    }
    r.num_edges = trees.size() * r.num_letters;
    //for (auto it = trees.begin(); it != trees.end(); ++it) {
    //  std::cout << *it << "\n";
    //}
    std::cout << r << "\n";
    return r;
  }
}
