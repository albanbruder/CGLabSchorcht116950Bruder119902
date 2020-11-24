#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <memory>
#include <glm/matrix.hpp>

class CameraNode {
  public:
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