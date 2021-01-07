#include "point_light_node.hpp"

PointLightNode::PointLightNode() {}

float PointLightNode::getLightIntensity() const {
  return lightIntensity;
}

void PointLightNode::setLightIntensity(float newLightIntensity) {
  lightIntensity = newLightIntensity;
}

glm::vec3 PointLightNode::getLightColor() const {
  return lightColor;
}

void PointLightNode::setLightColor(glm::vec3 newLightColor) {
  lightColor = newLightColor;
}

void PointLightNode::setLightPosition(glm::vec3 newPosition){
  lightPosition = newPosition;
}

glm::vec3 PointLightNode::getLightPosition() const {
  return lightPosition;
}