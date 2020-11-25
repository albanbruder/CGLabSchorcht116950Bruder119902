#ifndef CAMERA_NODE_HPP
#define CAMERA_NODE_HPP

#include <memory>
#include "node.hpp"
#include <glm/matrix.hpp>
#include <string>

class CameraNode : public Node {
  public:
    // constructors
    CameraNode();
    CameraNode(std::string name);

    bool getPerspective() const;

    /**
     * Is this camera enabled?
     */
    bool getEnabled() const;

    /**
     * Enables this camera
     */
    void setEnabled(bool enabled);

    /**
     * Get the projection matrix of this camera
     */
    glm::mat4 getProjectionMatrix() const;

    /**
     * Set the projection matrix of this camera
     */
    void setProjectionMatrix(const glm::mat4 &newProjectionMatrix);

  
  private:
    bool isPerspective;

    // enabled or not
    bool isEnabled;

    // projection matrix of the camera
    glm::fmat4 projectionMatrix;

};

#endif