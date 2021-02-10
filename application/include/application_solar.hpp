#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "scene_graph.hpp"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;

 protected:
  void initializeFramebuffer();
  void initializeShaderPrograms();
  void initializeGeometry();
  // add al objects and root
  void initializeObjects();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();
  void initalizeStars();
  void initalizeOrbits();
  void initializeSkybox();

  std::shared_ptr<GeometryNode> createOrbit(std::shared_ptr<GeometryNode> const& planet) const;

  // cpu representation of model
  std::map<std::string, std::shared_ptr<model_object>> objects{};
  
  model_object star_object;
  std::vector<GLfloat> star_container;
  
  //SceneGraph
  SceneGraph graph;

  //Shift Speed multiplier
  float multiplier=1.0f;

  texture_object skyboxTexture;

  // window size
  unsigned windowWidth = 640u;
  unsigned windowHeight = 480u;

  // frame buffer
  unsigned int FBO;
  unsigned int FBO_texture;
  // render buffer
  unsigned int RBO;
};

#endif