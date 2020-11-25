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
    std::shared_ptr<Node> getParent() const;
    void setParent(const std::shared_ptr<Node> newParent);
    std::shared_ptr<Node> getChildren(std::string Name) const;
    std::list<std::shared_ptr<Node>> getChildrenList(bool recursive = false) const;
    std::string getName() const;
    std::string getPath() const;
    int getDepth() const;
    glm::mat4 getLocalTransform() const;
    void setLocalTransform(const glm::mat4 &transform);
    glm::fmat4 getWorldTransform() const;
    void setWorldTransform(const glm::mat4 &transform);
    void addChildren(const std::shared_ptr<Node> &child);
    // std::shared_ptr<GeometryNode> addChildren(const GeometryNode &child);
    std::shared_ptr<Node> removeChildren(std::string);
    Node();
    Node(std::string name);
	  ~Node();

  private:
    std::shared_ptr<Node> parent = nullptr;
    std::list<std::shared_ptr<Node>> children;
    std::string name;
    std::string path;
    int depth;
    glm::fmat4 localTransform;
    glm::fmat4 worldTransform;
};

#endif