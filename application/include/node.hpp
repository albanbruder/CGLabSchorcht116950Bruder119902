#ifndef NODE_HPP
#define NODE_HPP

#include <glm/matrix.hpp>
#include <list>

class Node {
  public:
    // allocate and initialize objects
    Node* getParent();
    void setParent(Node newParent);
    Node* getChildren(std::string newName);
    std::list<Node> getChildrenList();
    std::string getName();
    std::string getPath();
    int getDepth();
    glm::mat4 getLocalTransform();
    void setLocalTransform(glm::mat4 transform);
    glm::mat4 getWorldTransform();
    void setWorldTransform(glm::mat4 transform);
    void addChildren(Node child);
    Node* removeChildren(std::string);
    Node();
	~Node();
  
  private:
    Node* parent;
    std::list<Node> children;
    std::string name;
    std::string path;
    int depth;
    glm::fmat4 localTransform;
    glm::fmat4 wordTransform;
};

#endif