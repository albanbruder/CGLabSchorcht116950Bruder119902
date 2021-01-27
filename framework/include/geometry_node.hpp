#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"
#include "model.hpp"
#include <memory>
#include "structs.hpp"

class GeometryNode : public Node {
  public:
    // constructors
    GeometryNode();
    GeometryNode(std::string name);
    GeometryNode(std::string name, float orbit);
    GeometryNode(std::string name, float orbit, glm::vec3 color);

    /**
     * Get the geometry model of this node
     */
    std::shared_ptr<model_object> getGeometry() const;

    /**
     * Set the geometry model of this node
     */
    void setGeometry(const std::shared_ptr<model_object> &newGeometry);

    /**
     * Get the color of this node
     */
    glm::vec3 getColor() const;

    /**
     * Set the color of this node
     */
    void setColor(glm::vec3 newColor);

    /**
     * Get the texture of this node
     */
    std::shared_ptr<texture_object> getTexture() const;

    /**
     * Set the texture of this node
     */
    void setTexture(std::shared_ptr<texture_object> newColor);

    /**
     * Get the scale of this node
     */
    float getScale() const;

    /**
     * Set the scale of this node
     */
    void setScale(float newScale);
    
  private:
    // geometry model of the node
    std::shared_ptr<model_object> geometry;

    // color of the node
    glm::vec3 color;

    std::shared_ptr<texture_object> texture;

    float scale = 1.0f;
};

#endif