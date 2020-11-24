#include "camera_node.hpp"


CameraNode::CameraNode() {}

CameraNode::CameraNode(std::string name) 
  : Node{name} {}

bool CameraNode::getPerspective() const {
  return isPerspective;  
}

bool CameraNode::getEnabled() const{
  return isEnabled;
}

void CameraNode::setEnabled(bool enabled) {
  isEnabled = enabled;
}

glm::mat4 CameraNode::getProjectionMatrix() const {
  return projectionMatrix;
}

void CameraNode::setProjectionMatrix(const glm::mat4 &newProjectionMatrix) {
  projectionMatrix = newProjectionMatrix;
}
