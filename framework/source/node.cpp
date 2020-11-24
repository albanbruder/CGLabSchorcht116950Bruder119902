#include "node.hpp"

#include <algorithm>

Node::Node() {}

Node::Node(std::string nodeName) {
  name = nodeName;
}

Node::~Node() {}

std::shared_ptr<Node> Node::getParent() const {
  return parent;
}

void Node::setParent(const Node &newParent) {
  parent = std::make_shared<Node>(newParent);
}

std::shared_ptr<Node> Node::getChildren(std::string name) const {
  std::list<std::shared_ptr<Node>>::const_iterator it = std::find_if(
    std::begin(children),
    std::end(children),
    [name](std::shared_ptr<Node> child) { return child->getName() == name; }
  );

  if (it == std::end(children)) {
    return nullptr;
  }

  return *it;
}

std::list<std::shared_ptr<Node>> Node::getChildrenList() const {
  return children;
}

std::string Node::getName() const {
  return name;
}

std::string Node::getPath() const {
  return path;
}

int Node::getDepth() const {
  return depth;
}

glm::mat4 Node::getLocalTransform() const {
  return localTransform;
}

void Node::setLocalTransform(const glm::mat4 &transform) {
  localTransform = transform;
}

glm::mat4 Node::getWorldTransform() const {
  return worldTransform;
}

void Node::setWorldTransform(const glm::mat4 &transform) {
  worldTransform = transform;
}

void Node::addChildren(const Node &child) {
  children.push_back(std::make_shared<Node>(child));
}

std::shared_ptr<Node> Node::removeChildren(std::string name) {
  std::list<std::shared_ptr<Node>>::const_iterator it = std::find_if(
    std::begin(children),
    std::end(children),
    [name](std::shared_ptr<Node> child) { return child->getName() == name; }
  );
  
  if (it == std::end(children)) {
    return nullptr;
  }

  std::shared_ptr<Node> child = *it;

  std::remove(std::begin(children), std::end(children), child);
  return child;
}
  