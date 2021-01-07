#ifndef POINT_LIGHT_NODE_HPP
#define POINT_LIGHT_NODE_HPP

#include "node.hpp"
#include "model.hpp"
#include <memory>

class PointLightNode : public Node {
  public:
    // constructors
    PointLightNode();

    /**
     * Get the light intensity of this node
     */
    float getLightIntensity() const;

    /**
     * Set the light intensity of this node
     */
    void setLightIntensity(float lightIntensity);

    /**
     * Get the light color of this node
     */
    glm::vec3 getLightColor() const;

    /**
     * Set the light color of this node
     */
    void setLightColor(glm::vec3 lightColor);

    /**
     * Get the light position of this node
     */
    glm::vec3 getLightPosition() const;

    /**
     * Set the light position of this node
     */
    void setLightPosition(glm::vec3 lightPosition);
    
  private:
    // light intensity of the node
    float lightIntensity;

    // light color of the node
    glm::vec3 lightColor;

     // light position of the node
    glm::vec3 lightPosition;
};

#endif