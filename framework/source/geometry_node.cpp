#include "geometry_node.hpp"

GeometryNode::GeometryNode() {}

GeometryNode::GeometryNode(std::string name) 
  : Node{name} {}

GeometryNode::GeometryNode(std::string name, float orbit) 
  : Node{name, orbit} {}

std::shared_ptr<model> GeometryNode::getGeometry() const {
  return geometry;
}

void GeometryNode::setGeometry(const model &newGeometry) {
  geometry = std::make_shared<model>(newGeometry);
}