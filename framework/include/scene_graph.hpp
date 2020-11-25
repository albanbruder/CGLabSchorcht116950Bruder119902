#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <memory>
#include "node.hpp"
#include "camera_node.hpp"

class SceneGraph {
  public:
    SceneGraph();
    std::string getName() const;
    std::shared_ptr<Node> getRoot() const;
    std::string printGraph() const;
    std::shared_ptr<CameraNode> camera = nullptr;

  private:
    void setName(std::string newName);
    void setRoot(const Node &newRoot);
    std::string name;
    std::shared_ptr<Node> root;
};

#endif