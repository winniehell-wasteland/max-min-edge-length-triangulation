#ifndef MMT_GRAPHVIZ_H
#define MMT_GRAPHVIZ_H

#include <string>

#include "graph.h"

template<T>
class GraphRepr
{
  void draw(Graph& graph);
};

class Graphviz {
  template<T>
  static void draw_graph(std::string filename, const T& object)
  {

  }
};

#endif // MMT_GRAPHVIZ_H
