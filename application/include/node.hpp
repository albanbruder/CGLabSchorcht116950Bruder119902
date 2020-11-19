#ifndef NODE_HPP
#define NODE_HPP

class Node {
  public:
    // allocate and initialize objects
    Node getParent();
    void setParent(Node newParent);
    Node getChildren(std::string newName);
    std::list<Node> getChildrenList();
    std::string getName();
    std::string getPath();
    std::int getDepth();
    glm::mat4 getLocalTransform();
    void setLocalTransform(glm::mat4 transform);
    glm::mat4 getWorldTransform();
    void setWorldTransform(glm::mat4 transform);
    void addChildren(Node child);
    Node removeChildren(std::string);
  
  private:
    Node parent;
    std::list<Node> children;
    std::string name;
    std::string path;
    std::int depth;
    glm::mat4 localTransform;
    glm::mat4 wordTransform;
};

#endif