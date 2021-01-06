#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

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
    
  private:
    // light intensity of the node
    float lightIntensity;

    // light color of the node
    glm::vec3 lightColor;
};

#endif