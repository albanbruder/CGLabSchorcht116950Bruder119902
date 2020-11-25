#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"
#include "model.hpp"
#include <memory>

class GeometryNode : public Node {
  public:
    GeometryNode();
    GeometryNode(std::string name);
    std::shared_ptr<model> getGeometry() const;
    void setGeometry(const model &newGeometry);
    
  private:
    std::shared_ptr<model> geometry;
};

#endif