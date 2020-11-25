#ifndef NODE_HPP
#define NODE_HPP

#include <glm/matrix.hpp>
#include <list>
#include <string>
#include <memory>

class GeometryNode;

class Node {
  public:
    // allocate and initialize objects

    /**
     * Get the parent of this node
     */
    std::shared_ptr<Node> getParent() const;

    /**
     * Set the parent of this node
     */
    void setParent(const std::shared_ptr<Node> newParent);

    /**
     * Get the a specific children of this node
     */
    std::shared_ptr<Node> getChildren(std::string Name) const;

    /**
     * Get all children of this node
     * 
     * @param recursive boolean indicating if function should return children recursivly
     */
    std::list<std::shared_ptr<Node>> getChildrenList(bool recursive = false) const;

    /**
     * Get the name of this node
     */
    std::string getName() const;

    /**
     * Get the path of this node
     */
    std::string getPath() const;

    /**
     * Get the depth of this node
     */
    int getDepth() const;

    /**
     * Get the speed of this node
     */
    float getSpeed() const;

    /**
     * Set the speed of this node
     */
    void setSpeed(float newSpeed);

    /**
     * Get the local transformation matrix of this node
     */
    glm::mat4 getLocalTransform() const;

    /**
     * Set the local transformation matrix of this node
     */
    void setLocalTransform(const glm::mat4 &transform);

    /**
     * Get the world transformation matrix of this node
     */
    glm::fmat4 getWorldTransform() const;

    /**
     * Set the world transformation matrix of this node
     */
    void setWorldTransform(const glm::mat4 &transform);

    /**
     * Add a children to this node
     */
    void addChildren(const std::shared_ptr<Node> &child);

    /**
     * Remove a children from this node
     */
    std::shared_ptr<Node> removeChildren(std::string);
    
    // constructors and destructors
    Node();
    Node(std::string name);
	  ~Node();

  private:
    // parent of the node
    std::shared_ptr<Node> parent = nullptr;

    // children of the node
    std::list<std::shared_ptr<Node>> children;

    // name of the node
    std::string name;

    // path of the node
    std::string path;

    // speed of the node
    float speed=1.0f;

    // depth of the node
    int depth;

    // local transformation matrix of the node
    glm::fmat4 localTransform;

    // world transformation matrix of the node
    glm::fmat4 worldTransform;
};

#endif