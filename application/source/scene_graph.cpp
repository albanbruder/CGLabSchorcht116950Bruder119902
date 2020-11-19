#include "scene_graph.hpp"

std::string SceneGraph::getName() {
  return name;
}

Node getRoot(){
  return root;
}

std::string printGraph(){
  //ToDo in the Future
}

void setName(std::string newName){
  name = newName;
}

void setRoot(Node newRoot){
  root = newRoot;
}