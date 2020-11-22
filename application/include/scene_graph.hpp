#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include "node.hpp"

class SceneGraph {
  public:
    std::string getName();
    Node getRoot();
    std::string printGraph();
  
  private:
    void setName(std::string newName);
    void setRoot(Node newRoot);
    std::string name;
    Node root;
};

#endif