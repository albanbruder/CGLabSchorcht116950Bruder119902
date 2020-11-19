#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

class CameraNode {
  public:
    std::bool getPerspective();
    std::bool getEnabled();
    void setEnabled(std::bool enabled);
    glm::mat4 getProjectionMatrix();
    void setProjectionMatrix(glm::mat4 newProjectionMatrix);

  
  private:
    std::bool isPerspective;
    std::bool isEnabled;
    glm::mat4 projectionMatrix;

};

#endif