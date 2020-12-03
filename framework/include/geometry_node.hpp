#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"
#include "model.hpp"
#include <memory>

class GeometryNode : public Node {
  public:
    // constructors
    GeometryNode();
    GeometryNode(std::string name);

    /**
     * Get the geometry model of this node
     */
    std::shared_ptr<model> getGeometry() const;

    /**
     * Set the geometry model of this node
     */
    void setGeometry(const model &newGeometry);
    
  private:
    // geometry model of the node
    std::shared_ptr<model> geometry;
};

#endif