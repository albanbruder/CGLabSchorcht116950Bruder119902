#include "geometry_node.hpp"

GeometryNode::GeometryNode() {}

GeometryNode::GeometryNode(std::string name) 
  : Node{name} {}

std::shared_ptr<model> GeometryNode::getGeometry() const {
  return geometry;
}

void GeometryNode::setGeometry(const model &newGeometry) {
  geometry = std::make_shared<model>(newGeometry);
}