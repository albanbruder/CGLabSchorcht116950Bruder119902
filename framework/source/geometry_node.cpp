#include "geometry_node.hpp"

GeometryNode::GeometryNode() {}

GeometryNode::GeometryNode(std::string name) 
  : Node{name} {}

GeometryNode::GeometryNode(std::string name, float orbit) 
  : Node{name, orbit} {}

GeometryNode::GeometryNode(std::string name, float orbit, glm::vec3 color) 
  : Node{name, orbit}, color{color} {}

std::shared_ptr<model_object> GeometryNode::getGeometry() const {
  return geometry;
}

void GeometryNode::setGeometry(const std::shared_ptr<model_object> &newGeometry) {
  geometry = newGeometry;
}

glm::vec3 GeometryNode::getColor() const {
  return color;
}

void GeometryNode::setColor(glm::vec3 newColor) {
  color = newColor;
}

std::shared_ptr<texture_object> GeometryNode::getTexture() const {
  return texture;
}

void GeometryNode::setTexture(std::shared_ptr<texture_object> newTexture) {
  texture = newTexture;
}

float GeometryNode::getScale() const {
  return scale;
}

void GeometryNode::setScale(float newScale) {
  scale = newScale;
}