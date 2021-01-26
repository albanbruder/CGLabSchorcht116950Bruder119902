#include "geometry_node.hpp"

GeometryNode::GeometryNode() {}

GeometryNode::GeometryNode(std::string name) 
  : Node{name} {}

GeometryNode::GeometryNode(std::string name, float orbit) 
  : Node{name, orbit} {}

GeometryNode::GeometryNode(std::string name, float orbit, glm::vec3 color) 
  : Node{name, orbit}, color{color} {}

std::shared_ptr<model> GeometryNode::getGeometry() const {
  return geometry;
}

void GeometryNode::setGeometry(const model &newGeometry) {
  geometry = std::make_shared<model>(newGeometry);
}

void GeometryNode::setGeometry(const std::shared_ptr<model> &newGeometry) {
  geometry = newGeometry;
}

glm::vec3 GeometryNode::getColor() const {
  return color;
}

void GeometryNode::setColor(glm::vec3 newColor) {
  color = newColor;
}

std::shared_ptr<pixel_data> GeometryNode::getTexture() const {
  return texture;
}

void GeometryNode::setTexture(std::shared_ptr<pixel_data> newTexture) {
  texture = newTexture;
}