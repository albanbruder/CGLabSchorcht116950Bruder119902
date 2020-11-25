#include "scene_graph.hpp"


SceneGraph::SceneGraph() {
  Node root = Node("root");
  setRoot(root);
}

std::string SceneGraph::getName() const {
  return name;
}

std::shared_ptr<Node> SceneGraph::getRoot() const {
  return root;
}

std::string SceneGraph::printGraph() const {
  //ToDo in the Future
}

void SceneGraph::setName(std::string newName) {
  name = newName;
}

void SceneGraph::setRoot(const Node &newRoot) {
  root = std::make_shared<Node>(newRoot);
}