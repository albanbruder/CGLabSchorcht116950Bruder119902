#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <memory>
#include "node.hpp"
#include <glm/matrix.hpp>
#include <string>

class CameraNode : public Node {
  public:
    CameraNode();
    CameraNode(std::string name);
    bool getPerspective() const;
    bool getEnabled() const;
    void setEnabled(bool enabled);
    glm::mat4 getProjectionMatrix() const;
    void setProjectionMatrix(const glm::mat4 &newProjectionMatrix);

  
  private:
    bool isPerspective;
    bool isEnabled;
    glm::fmat4 projectionMatrix;

};

#endif