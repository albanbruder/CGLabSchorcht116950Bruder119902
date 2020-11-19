#include "camera_node.hpp"

std::bool CameraNode::getPerspective(){
  return isPerspective;  
}

std::bool CameraNode::getEnabled(){
  return isEnabled;
}

void CameraNode::setEnabled(std::bool enabled) {
  isEnabled=enabled;
}

glm::mat4 CameraNode::getProjectionMatrix() {
  return projectionMatrix;
}

void CameraNode::setProjectionMatrix(glm::mat4 newProjectionMatrix) {
  projectionMatrix=newProjectionMatrix
}
