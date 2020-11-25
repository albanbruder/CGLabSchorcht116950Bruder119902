#ifndef SCENE_GRAPH_HPP
#define SCENE_GRAPH_HPP

#include <memory>
#include "node.hpp"
#include "camera_node.hpp"

class SceneGraph {
  public:
    // constructors
    SceneGraph();

    /**
     * Get the name of this scene
     */
    std::string getName() const;

    /**
     * Get the root node of this scene
     */
    std::shared_ptr<Node> getRoot() const;

    /**
     * Print the scene graph
     */
    std::string printGraph() const;

    /**
     * TODO: replace with multi camera system
     */
    std::shared_ptr<CameraNode> camera = nullptr;

  private:
    /**
     * Set the name of this scene
     */
    void setName(std::string newName);

    /**
     * Set the root node of this scene
     */
    void setRoot(const Node &newRoot);

    /**
     * The name of this scene
     */
    std::string name;

    /**
     * The root node of this scene
     */
    std::shared_ptr<Node> root;
};

#endif