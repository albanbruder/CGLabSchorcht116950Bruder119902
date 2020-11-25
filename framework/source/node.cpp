#include "node.hpp"

#include <algorithm>
#include <cstdlib>
#include <iostream>

Node::Node() {}

Node::Node(std::string nodeName) {
  name = nodeName;
  speed=(rand()%100) + 1;
}

Node::~Node() {}

std::shared_ptr<Node> Node::getParent() const {
  return parent;
}

void Node::setParent(const std::shared_ptr<Node> newParent) {
  parent = newParent;
}

std::shared_ptr<Node> Node::getChildren(std::string name) const {
  // find the elements iterator
  std::list<std::shared_ptr<Node>>::const_iterator it = std::find_if(
    std::begin(children),
    std::end(children),
    [name](std::shared_ptr<Node> child) { return child->getName() == name; }
  );

  // check if child exists
  if (it == std::end(children)) {
    return nullptr;
  }

  return *it;
}

std::list<std::shared_ptr<Node>> Node::getChildrenList(bool recursive) const {
  // if not recursive, just return the chidlren list
  if (!recursive) {
    return children;
  }

  std::list<std::shared_ptr<Node>> allChildren;

  for(std::shared_ptr<Node> child : children) {
    // add all children of this node
    allChildren.push_back(child);

    // add all children of child nodes
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

std::shared_ptr<Node> Node::removeChildren(std::string name) {
  // find the elements iterator
  std::list<std::shared_ptr<Node>>::const_iterator it = std::find_if(
    std::begin(children),
    std::end(children),
    [name](std::shared_ptr<Node> child) { return child->getName() == name; }
  );
  
  // check if element exists
  if (it == std::end(children)) {
    return nullptr;
  }

  // dereference iterator
  std::shared_ptr<Node> child = *it;

  // remove and return
  std::remove(std::begin(children), std::end(children), child);
  return child;
}

float Node::getSpeed() const{
  return speed;
}

void Node::setSpeed(float newSpeed){
  speed=newSpeed;
}