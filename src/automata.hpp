/**
 * @file
 * @author  Brandon Hoane <bhoane@andrew.cmu.edu>
 * @version 1.0
 *
 * @brief Defines Buechi and Rabin Automata.
 *
 * Provides constructors to read a Buechi automaton from an input file,
 * and to determine a Rabin automaton from a Buechi.
 * Also defines printing functions, both human-readable and graphviz.
 */

#ifndef _cdm_AUTOMATA_HPP_
#define _cdm_AUTOMATA_HPP_

#include <iostream>
#include <map>
#include <set>
#include <vector>

namespace _cdm {

  typedef std::pair<int, int> transition;
  typedef std::pair<transition, int> transition_edge;
  typedef std::multimap<transition, int> transition_graph;
  typedef std::pair<std::vector<int>, std::vector<int> > rabin_pair;
  
  /**@brief The transition function of a finite state machine.
   *
   * Edges are stored as a multimap from (state,letter) pairs
   * to states, where states and letters are ints.
   * This allows for nondeterminism.
   */
  class FiniteStateMachine {
  public:
    int num_letters;
    int num_states;
    int num_edges;
    /** @brief Maps (state,letter) pairs to zero or more states. */
    transition_graph edges;
    
    /** @brief Constructs an empty finite state machine. */
    FiniteStateMachine() {}
    
    /**@brief Prints this FSM in graphviz format.
     * 
     * Creates a graph with labelled states and transitions.
     * Subclasses can override graphviz_body to change the
     * behaviour of this function.
     */
    void graphviz_out(std::ostream& stream);
    
    /**@brief Human readable graph representation */
    friend std::ostream& operator<< (std::ostream& stream,
                                     const FiniteStateMachine& fsm);
    
    /**@brief Prints state1->state2 for each edge in edges.*/
    virtual void graphviz_body(std::ostream& stream);
  };
  
  /**@brief The transition function and initial and final states of a
   * Buechi Automaton.
   *
   * Currently only supports one initial and one final state.
   */
  class Buechi : public FiniteStateMachine {
  public:
    int initial;
    int final;
    
    /**@brief Parses the buechi automaton stored in filename.
     *
     * @param filename the file to read
     */
    Buechi(const char* filename);
    
    /**@brief Adds an in-arrow on the initial state and a double border
     * on the final.
     */
    virtual void graphviz_body(std::ostream& stream);
    friend std::ostream& operator<< (std::ostream& stream,
                                     const Buechi& bfsm);
  };

  /**@brief The transition function, initial state,
     and acceptance condition of a Rabin Automaton.
   */
  class Rabin : public FiniteStateMachine {
  public:
    int initial;
    std::vector<rabin_pair> accept;
    /**@brief Marks left and right states */
    virtual void graphviz_body(std::ostream& stream);
    friend std::ostream& operator<< (std::ostream& stream,
                                     const Rabin& rfsm);
  };
}

#endif
