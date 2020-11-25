#include "node.hpp"

#include <algorithm>
#include <iostream>

Node::Node() {}

Node::Node(std::string nodeName) {
  name = nodeName;
}

Node::~Node() {}

std::shared_ptr<Node> Node::getParent() const {
  return parent;
}

void Node::setParent(const std::shared_ptr<Node> newParent) {
  parent = newParent;
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

std::list<std::shared_ptr<Node>> Node::getChildrenList(bool recursive) const {
  if (!recursive) {
    return children;
  }

  std::list<std::shared_ptr<Node>> allChildren;

  for(std::shared_ptr<Node> child : children) {
    allChildren.push_back(child);

    for(std::shared_ptr<Node> subChild : child->getChildrenList(true)) {
      allChildren.push_back(subChild);
    }
  }

  return allChildren;
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

void Node::addChildren(const std::shared_ptr<Node> &child) {
  children.push_back(child);
}

/* std::shared_ptr<GeometryNode> Node::addChildren(const GeometryNode &child) {
  auto ptr = std::make_shared<GeometryNode>(child);
  children.push_back(std::dynamic_pointer_cast<Node>(ptr));
  return ptr;
} */

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
  