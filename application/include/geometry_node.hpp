#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"
#include "model.hpp"

class GeometryNode: public Node {
  public:
    model getGeometry();
    void setGeometry(model newGeometry);
  
  private:
    model geometry;
};

#endif