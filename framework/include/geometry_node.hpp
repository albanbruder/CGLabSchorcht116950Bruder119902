#ifndef GEOMETRY_NODE_HPP
#define GEOMETRY_NODE_HPP

#include "node.hpp"
#include "model.hpp"
#include <memory>
#include "texture_loader.hpp"

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
    std::shared_ptr<model> getGeometry() const;

    /**
     * Set the geometry model of this node
     */
    void setGeometry(const model &newGeometry);

    /**
     * Set the geometry model of this node
     */
    void setGeometry(const std::shared_ptr<model> &newGeometry);

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
    std::shared_ptr<pixel_data> getTexture() const;

    /**
     * Set the texture of this node
     */
    void setTexture(std::shared_ptr<pixel_data> newColor);
    
  private:
    // geometry model of the node
    std::shared_ptr<model> geometry;

    // color of the node
    glm::vec3 color;

    std::shared_ptr<pixel_data> texture;
};

#endif