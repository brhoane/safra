#ifndef _cdm_SAFRA_HPP_
#define _cdm_SAFRA_HPP_

#include <vector>
#include <bool>
#include <boost/graph/adjacency_list.hpp>

#inclue "automata.hpp"

namespace _cdm {
  struct vertex_label_t {
    typedef vertex_property_tag kind;
  };
  struct vertex_mark_t {
    typedef vertex_property_tag kind;
  };
  
  typedef std::vector<int> label_set;
  
  typedef boost::property<vertex_mark_t, bool> Mark;
  typedef boost::property<vertex_label_t, label_set,
                          Mark> Label;
  typedef boost::property<boost::vertex_name_t, int,
                          Label> vertex_property;
  
  typedef boost::adjacency_list<
    boost::listS, boost::listS, boost::directedS,
    vertex_property, boost::no_property>
  safra_tree;
  
  struct vertex_safra_t {
    typedef vertex_property_tag kind;
  };
  
  typedef boost::property<vertex_safra_t, safra_tree> safra_property;
  
  typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::directedS,
    safra_property, edge_property>
  safra_graph;

}
