#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include "node.hpp"

class SceneGraph {
  public:
    SceneGraph();
    std::string getName() const;
    std::shared_ptr<Node> getRoot() const;
    std::string printGraph() const;
  
  private:
    void setName(std::string newName);
    void setRoot(const Node &newRoot);
    std::string name;
    std::shared_ptr<Node> root;
};

#endif