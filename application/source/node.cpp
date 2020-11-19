#include "node.hpp"

Node Node::getParent() {
  return parent
}

void Node::setParent(Node newParent) {
  parent = newParent;
}

Node Node::getChildren(std::string newName) {
  name = newName;
}

std::list<Node> Node::getChildrenList() {
  return children;
}

std::string Node::getName() {
  return name;
}

std::string Node::getPath() {
  return path;
}

std::int Node::getDepth() {
  return depth;
}

glm::mat4 Node::getLocalTransform() {
  return localTransform;
}

void Node::setLocalTransform(glm::mat4 transform) {
  localTransform = transform;
}

glm::mat4 getWorldTransform() {
  return worldTransform;
}

void Node::setWorldTransform(glm::mat4 transform) {
  worldTransform = transform;
}

void Node::addChildren(Node child) {
  children.push_back(child);
}

Node Node::removeChildren(std::string name) {
  children.remove_if([child] -> bool { return child.name == name});
}
  