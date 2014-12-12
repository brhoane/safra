#ifndef _cdm_SAFRA_HPP_
#define _cdm_SAFRA_HPP_

#include <stdlib.h>
#include <vector>
#include <set>
#include <map>
#include <iostream>

#include "automata.hpp"

namespace _cdm {
  /**@brief Indicates the status of a name in a safra tree. */
  enum find_status {
    NOT_FOUND,
    FOUND,
    FOUND_MARKED
  };
  
  /**@brief A node in a safra tree. */
  struct SafraNode {
    /**Names are unique in a given tree.
     * They are in the range [0, 2*b) where b is the number of states
     * in the Buechi automaton.
     * The minimum available new name is always picked.
     */
    int name;
    /**Labels are subsets of the set of states in the Buechi automaton.
     * The labels of the children of a node form a partition
     * of a subset of the label of that node.
     */
    std::set<int> label;
    bool mark;
    std::vector<SafraNode> children;
    /**Two nodes are equal iff their names, labels, marks,
     * and children are equal.
     */
    bool operator==(const SafraNode &other) const;
    bool operator!=(const SafraNode &other) const {
      return !(*this == other);
    }
    bool operator<(const SafraNode &other) const;
  };
  std::ostream& operator<< (std::ostream& stream, const SafraNode& sn);
  
  /**@brief Manages the currently available node names.*/
  class NamePool {
  private:
    std::vector<bool> pool;
  public:
    /**@brief Initializes the name pool to 2n names.
     *
     * The name 0 is assumed to be in use. All others are unused.
     */
    NamePool(int buechi_size);
    /**@brief Get the minimum unused name in the pool.
     *
     * @return the minimum unused name, which is guarantueed to exist.
     * The name is marked as used.
     */
    int get_unused_name();
    /**@brief Returns a node's names to the name pool.
     *
     * This nodes name, and the names of all its descendants,
     * wil be marked as unused.
     */
    void remove_names(SafraNode& sn);
    unsigned int size() const { return pool.size(); }
    bool operator[] (unsigned int n) const { return pool[n]; }
    
    friend std::ostream& operator<< (std::ostream& stream,
                                     const NamePool& np);
  };
  
  /* A Safra Tree, corresponding to one state in the Rabin automaton. */
  class SafraTree {
  public:
    NamePool name_pool;
    SafraNode root;
    
    SafraTree() = delete;
    /**@brief Initialize the name pool. */
    SafraTree(int buechi_size) : name_pool(buechi_size) {};
    /**@brief Construct the initial tree. */
    SafraTree(const Buechi& b);
    
    /**@brief Checks the node invariants for this tree. */
    bool is_safra_tree() const;
    /**@brief Finds a name in the tree.
     * 
     * @return \li FOUND_MARKED if name is in a marked node in this tree.
     * \li FOUND if name appears in only unmarked nodes.
     * \li NOT_FOUND if name does not appear at all.
     */
    enum find_status find_name(int name) const;
    bool operator==(const SafraTree &other) const {
      return root == other.root;
    } 
    bool operator<(const SafraTree &other) const {
      return root < other.root;
    }

    friend std::ostream& operator<< (std::ostream& stream,
                                     const SafraTree& st);
  };

  /**@brief Builds a graph of safra trees, then converts them into
   * a Rabin automaton.
   */
  class SafraGraph {
  private:
    SafraNode copy_unmark_update(const SafraNode& other, int letter);
    void create(SafraNode& sn, NamePool& pool);
    void horizontal_merge_and_kill(SafraNode& sn, NamePool& pool);
    void vertical_merge(SafraNode& sn, NamePool& pool);
  public:
    Buechi buechi;

    SafraGraph() = delete;
    /**@brief Initialize with a Buechi automaton. */
    SafraGraph(const Buechi b) : buechi(b) {}
    /**@brief Transition a Safra tree on a letter.
     *
     * @return the result of the transition function
     */
    SafraTree next_tree(const SafraTree& st, int letter);
    /*@brief Construct a Rabin automaton from the provided Buechi.
     *
     * Computes the closure of next_tree on all letters and the
     * initial Safra tree. Each tree is assigned a number, so
     * the transition function determines a Rabin automaton.
     * The acceptance condition is also calculated.
     */
    Rabin make_rabin();
  };
}

#endif
