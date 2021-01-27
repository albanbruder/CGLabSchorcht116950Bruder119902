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

  // cpu representation of model
  std::map<std::string, std::shared_ptr<model_object>> objects{};
  model_object planet_object;
  model_object star_object;
  GLuint skybox_AO = 0;
  std::map<std::string, model_object> orbit_objects{};
  std::vector<GLfloat> star_container;
  std::map<std::string, std::vector<GLfloat>> orbit_containers;
  //SceneGraph
  SceneGraph graph;
  //Shift Speed multiplier
  float multiplier=1.0f;
  texture_object skyboxTexture;
  unsigned int planetTexture;
};

#endif