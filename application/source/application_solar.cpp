#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "geometry_node.hpp"
#include "camera_node.hpp"
#include "texture_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <iostream>
#include <memory>
#include <glm/gtx/string_cast.hpp>
#include <math.h>
#include <iostream>
#include <string>
#include <vector>

/**
 * Loads the skybox texture using 6 "sky" images
 */
texture_object loadSkyboxTexture() {
    std::vector<std::string> parts{
        "sky1.png",
        "sky2.png",
        "sky4.png",
        "sky3.png",
        "sky5.png",
        "sky6.png"
    };

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < parts.size(); i++)
    {
        auto pixelData = texture_loader::file("../resources/textures/" + parts[i]);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 
          pixelData.channels, 
          pixelData.width, 
          pixelData.height, 
          0, 
          pixelData.channels, 
          pixelData.channel_type, 
          pixelData.ptr());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    auto texture = texture_object{};
    texture.handle = textureID;
    texture.target = GL_TEXTURE_CUBE_MAP;

    return texture;
}

/**
 * Loads the sphere texture using the given map
 */
texture_object loadSphereTexture(std::string map) {
  auto texture = texture_object{};
  texture.target = GL_TEXTURE_2D;

  glGenTextures(1, &(texture.handle));
  glBindTexture(texture.target, texture.handle);
  glTexParameteri(texture.target, GL_TEXTURE_BASE_LEVEL, 0);
  glTexParameteri(texture.target, GL_TEXTURE_MAX_LEVEL, 0);

  auto pixelData = texture_loader::file("../resources/textures/" + map);
  glTexImage2D(texture.target, 0, 
    pixelData.channels, 
    pixelData.width, 
    pixelData.height, 
    0, 
    pixelData.channels, 
    pixelData.channel_type, 
    pixelData.ptr());

  return texture;
}

/**
 * Generates box vertices with edge length s
 */
std::vector<float> generateBoxVertices(float s) {
  return std::vector<float> {
    -s, s, s,
    -s, -s, -s,
    -s, -s, s,

    -s, s, -s,
    s, -s, -s,
    -s, -s, -s,

    s, s, -s,
    s, -s, s,
    s, -s, -s,

    s, s, s,
    -s, -s, s,
    s, -s, s,

    s, -s, -s,
    -s, -s, s,
    -s, -s, -s,

    -s, s, -s,
    s, s, s,
    s, s, -s,

    -s, s, s,
    -s, s, -s,
    -s, -s, -s,

    -s, s, -s,
    s, s, -s,
    s, -s, -s,

    s, s, -s,
    s, s, s,
    s, -s, s,

    s, s, s,
    -s, s, s,
    -s, -s, s,

    s, -s, -s,
    s, -s, s,
    -s, -s, s,

    -s, s, -s,
    -s, s, s,
    s, s, s,
  };
}

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,star_object{}
 ,star_container{}
 ,graph{}
{
  skyboxTexture = loadSkyboxTexture();

  initializeGeometry();  
  initializeObjects();
  initalizeStars();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  for (auto const& object : objects) {
    glDeleteBuffers(1, &(object.second->vertex_BO));
    glDeleteBuffers(1, &(object.second->element_BO));
    glDeleteVertexArrays(1, &(object.second->vertex_AO));
  }
}

std::shared_ptr<GeometryNode> ApplicationSolar::createOrbit(std::shared_ptr<GeometryNode> const& planet) const {
  std::shared_ptr<Node> parent = planet->getParent();

  auto orbit = std::make_shared<GeometryNode>(GeometryNode(planet->getName() + "-orbit", 0.0f, planet->getColor()));
  orbit->setGeometry(objects.at("orbit"));
  orbit->setScale(planet->getOrbit());
  parent->addChildren(orbit);
  orbit->setParent(parent);

  return orbit;
}

void ApplicationSolar::initializeObjects(){
  graph.light = std::make_shared<PointLightNode>(PointLightNode());
  graph.light->setLightColor(glm::vec3{1.0f, 1.0f, 0.9f});
  graph.light->setLightIntensity(2.0f);
  graph.light->setLightPosition(glm::vec3{0.0f, 0.0f, 0.0f});

  std::shared_ptr<Node> root = graph.getRoot();

  auto camera = std::make_shared<CameraNode>(CameraNode("camera"));
  camera->setLocalTransform(glm::rotate(glm::translate(glm::fmat4{}, glm::fvec3{0.0f, -40.0f, 10.0f}), float(1.57), glm::fvec3{1.0f, 0.0f, 0.0f}));
  camera->setProjectionMatrix(utils::calculate_projection_matrix(initial_aspect_ratio));
  root->addChildren(camera);
  camera->setParent(root);
  graph.camera = camera;

  auto sun = std::make_shared<GeometryNode>(GeometryNode("sun", 0.0f, glm::vec3{1.0f, 1.0f, 0.0f}));
  auto mercury = std::make_shared<GeometryNode>(GeometryNode("mercury", 5.0f, glm::vec3{0.7f, 0.7f, 0.7f}));
  auto venus = std::make_shared<GeometryNode>(GeometryNode("venus", 8.0f, glm::vec3{1.0f, 228.0f/255.0f, 85.0f/255.0f}));
  auto earth = std::make_shared<GeometryNode>(GeometryNode("earth", 14.0f, glm::vec3{32.0f/255.0f, 109.0f/255.0f, 223.0f/255.0f}));
  auto mars = std::make_shared<GeometryNode>(GeometryNode("mars", 20.0f, glm::vec3{243.0f/255.0f, 85.0f/255.0f, 36.0f/255.0f}));
  auto jupiter = std::make_shared<GeometryNode>(GeometryNode("jupiter", 28.0f, glm::vec3{202.0f/255.0f, 180.0f/255.0f, 173.0f/255.0f}));
  auto saturn = std::make_shared<GeometryNode>(GeometryNode("saturn", 33.0f, glm::vec3{192.0f/255.0f, 159.0f/255.0f, 186.0f/255.0f}));
  auto uranus = std::make_shared<GeometryNode>(GeometryNode("uranus", 38.0f, glm::vec3{163.0f/255.0f, 232.0f/255.0f, 253.0f/255.0f}));
  auto neptun = std::make_shared<GeometryNode>(GeometryNode("neptun", 45.0f, glm::vec3{119.0f/255.0f, 110.0f/255.0f, 1.0f}));
  auto moon = std::make_shared<GeometryNode>(GeometryNode("moon", 2.5f, glm::vec3{141.0f/255.0f, 141.0f/255.0f, 141.0f/255.0f}));

  /**
   * Load & set planet models
   */
  sun->setGeometry(objects.at("sun"));
  mercury->setGeometry(objects.at("sphere"));
  venus->setGeometry(objects.at("sphere"));
  earth->setGeometry(objects.at("sphere"));
  mars->setGeometry(objects.at("sphere"));
  jupiter->setGeometry(objects.at("sphere"));
  saturn->setGeometry(objects.at("saturn"));
  uranus->setGeometry(objects.at("sphere"));
  neptun->setGeometry(objects.at("sphere"));
  moon->setGeometry(objects.at("sphere"));

  sun->setTexture(std::make_shared<texture_object>(loadSphereTexture("sun.png")));
  mercury->setTexture(std::make_shared<texture_object>(loadSphereTexture("mercury.png")));
  earth->setTexture(std::make_shared<texture_object>(loadSphereTexture("earth.png")));
  venus->setTexture(std::make_shared<texture_object>(loadSphereTexture("venus.png")));
  mars->setTexture(std::make_shared<texture_object>(loadSphereTexture("mars.png")));
  jupiter->setTexture(std::make_shared<texture_object>(loadSphereTexture("jupiter.png")));
  saturn->setTexture(std::make_shared<texture_object>(loadSphereTexture("saturn.png")));
  uranus->setTexture(std::make_shared<texture_object>(loadSphereTexture("uranus.png")));
  neptun->setTexture(std::make_shared<texture_object>(loadSphereTexture("neptun.png")));
  moon->setTexture(std::make_shared<texture_object>(loadSphereTexture("moon.png")));

  root->addChildren(sun);
  root->addChildren(mercury);
  root->addChildren(venus);
  root->addChildren(earth);
  root->addChildren(mars);
  root->addChildren(jupiter);
  root->addChildren(saturn);
  root->addChildren(uranus);
  root->addChildren(neptun);
  earth->addChildren(moon);
  
  sun->setParent(root);
  mercury->setParent(root);
  venus->setParent(root);
  earth->setParent(root);
  mars->setParent(root);
  jupiter->setParent(root);
  saturn->setParent(root);
  uranus->setParent(root);
  neptun->setParent(root);
  moon->setParent(earth);

  moon->setScale(0.5f);
  sun->setScale(2.0f);
  mercury->setScale(0.7f);

  // setup orbits
  createOrbit(mercury);
  createOrbit(earth);
  createOrbit(venus);
  createOrbit(mars);
  createOrbit(jupiter);
  createOrbit(saturn);
  createOrbit(uranus);
  createOrbit(neptun);
  createOrbit(moon);
}

void ApplicationSolar::render() const {
  /**
   * Render skybox
   */

  // glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_FALSE);

  // bind shader
  glUseProgram(m_shaders.at("skybox").handle);

  auto sbV = graph.camera->getLocalTransform()[3];
  auto sbT = glm::translate(glm::mat4{}, glm::vec3{sbV.x, sbV.y, sbV.z});

  glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ModelMatrix"),
                      1, GL_FALSE, glm::value_ptr(sbT));

  // bind the VAO to draw
  glBindVertexArray(objects.at("skybox")->vertex_AO);
  glBindTexture(skyboxTexture.target, skyboxTexture.handle);
  glDrawArrays(objects.at("skybox")->draw_mode, 0, objects.at("skybox")->num_elements);
  glDepthMask(GL_TRUE);

  /**
   * Render stars
   */
  
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("stars").handle);
  // bind the VAO to draw
  glBindVertexArray(star_object.vertex_AO);
  
  // draw bound vertex array using bound shader
  glDrawArrays(star_object.draw_mode, 0, star_object.num_elements);

  /**
   * Render planets
   */

  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList(true)) {
    if (planet->getName() == "camera") {
      continue;
    }

    std::string shader = "planet";
    if (planet->getName() == "sun") {
      shader = "sun";
    }

    std::shared_ptr<GeometryNode> gPlanet = std::static_pointer_cast<GeometryNode>(planet);

    if (gPlanet->getGeometry() == objects.at("orbit")) {
      shader = "orbit";
    }

    // bind shader to upload uniforms
    glUseProgram(m_shaders.at(shader).handle);

    // rotate from parent transformation matrix
    planet->setLocalTransform(glm::rotate(planet->getParent()->getLocalTransform(), float(planet->getSpeed()/100.0f*glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f}));

    // translate according to radius
    planet->setLocalTransform(glm::translate(planet->getLocalTransform(), glm::fvec3{0.0f, 0.0f, planet->getOrbit()}));

    /**
     * Upload ModelMatrix uniform
     */
    if (m_shaders.at(shader).u_locs.count("ModelMatrix") > 0) {
      glUniformMatrix4fv(m_shaders.at(shader).u_locs.at("ModelMatrix"),
                        1, GL_FALSE, glm::value_ptr(glm::scale(planet->getLocalTransform(), glm::fvec3{gPlanet->getScale(), gPlanet->getScale(), gPlanet->getScale()})));
    }

    // extra matrix for normal transformation to keep them orthogonal to surface
    planet->setWorldTransform(glm::inverseTranspose(glm::inverse(graph.camera->getLocalTransform()) * planet->getLocalTransform()));

    /**
     * Upload NormalMatrix uniform
     */
    if (m_shaders.at(shader).u_locs.count("NormalMatrix") > 0) {
      glUniformMatrix4fv(m_shaders.at(shader).u_locs.at("NormalMatrix"),
                        1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));
    }

    // bind the VAO to draw
    glBindVertexArray(gPlanet->getGeometry()->vertex_AO);

    /**
     * Bind texture
     */
    if (gPlanet->getTexture() != nullptr) {
      auto texture = gPlanet->getTexture();
      glBindTexture(texture->target, texture->handle);
    }

    /**
     * Upload ColorVertex uniform
     */
    if (m_shaders.at(shader).u_locs.count("ColorVertex") > 0) {
      glUniform3f(m_shaders.at(shader).u_locs.at("ColorVertex"), gPlanet->getColor().r, gPlanet->getColor().g, gPlanet->getColor().b);
    }

    /**
     * Upload CameraPosition uniform
     */
    if (m_shaders.at(shader).u_locs.count("CameraPosition") > 0) {
      auto cameraPos = (graph.camera->getLocalTransform() * glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});
      glUniform3f(m_shaders.at(shader).u_locs.at("CameraPosition"), cameraPos.x, cameraPos.y, cameraPos.z);
    }

    /**
     * Upload LightColor uniform
     */
    if (m_shaders.at(shader).u_locs.count("LightColor") > 0) {
      glUniform3f(m_shaders.at(shader).u_locs.at("LightColor"), graph.light->getLightColor().r, graph.light->getLightColor().g, graph.light->getLightColor().b);
    }

    /**
     * Upload LightPosition uniform
     */
    if (m_shaders.at(shader).u_locs.count("LightPosition") > 0) {
      glUniform3f(m_shaders.at(shader).u_locs.at("LightPosition"), graph.light->getLightPosition().x, graph.light->getLightPosition().y, graph.light->getLightPosition().z);
    }

    /**
     * Upload LightIntensity uniform
     */
    if (m_shaders.at(shader).u_locs.count("LightIntensity") > 0) {
      glUniform1f(m_shaders.at(shader).u_locs.at("LightIntensity"), graph.light->getLightIntensity());
    }

    if (shader == "orbit") {
      glDrawArrays(gPlanet->getGeometry()->draw_mode, 0, gPlanet->getGeometry()->num_elements);
    } else {
      // draw bound vertex array using bound shader
      glDrawElements(gPlanet->getGeometry()->draw_mode, gPlanet->getGeometry()->num_elements, model::INDEX.type, NULL);
    }
  }
  
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(graph.camera->getLocalTransform());
  // upload matrix to gpu

  // bind shader to which to upload unform
  glUseProgram(m_shaders.at("sun").handle);
  glUniformMatrix4fv(m_shaders.at("sun").u_locs.at("ViewMatrix"),
                    1, GL_FALSE, glm::value_ptr(view_matrix));

  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                    1, GL_FALSE, glm::value_ptr(view_matrix));


  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix)); 

  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("orbit").handle);

  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ViewMatrix"),
                    1, GL_FALSE, glm::value_ptr(view_matrix)); 

  glUseProgram(m_shaders.at("skybox").handle);

  glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ViewMatrix"),
                    1, GL_FALSE, glm::value_ptr(view_matrix)); 

}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("sun").handle);
  glUniformMatrix4fv(m_shaders.at("sun").u_locs.at("ProjectionMatrix"),
                    1, GL_FALSE, glm::value_ptr(graph.camera->getProjectionMatrix()));

  glUseProgram(m_shaders.at("planet").handle);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                    1, GL_FALSE, glm::value_ptr(graph.camera->getProjectionMatrix()));

  // handle star shaders
  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(graph.camera->getProjectionMatrix()));     

  // handle orbit shaders
  glUseProgram(m_shaders.at("orbit").handle);
  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ProjectionMatrix"),
                    1, GL_FALSE, glm::value_ptr(graph.camera->getProjectionMatrix()));

  glUseProgram(m_shaders.at("skybox").handle);
  glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ProjectionMatrix"),
                    1, GL_FALSE, glm::value_ptr(graph.camera->getProjectionMatrix())); 
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
    // upload uniform values to new locations
    uploadView();
    uploadProjection();
  
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container
    // sky box shader
    m_shaders.emplace("skybox", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/skybox.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/skybox.frag"}}});
    // request uniform locations for shader program
    m_shaders.at("skybox").u_locs["ViewMatrix"] = -1;
    m_shaders.at("skybox").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("skybox").u_locs["Texture"] = -1;
    m_shaders.at("skybox").u_locs["ModelMatrix"] = -1;

    // sun shader
    m_shaders.emplace("sun", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/sun.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/sun.frag"}}});
    // request uniform locations for shader program
    m_shaders.at("sun").u_locs["ModelMatrix"] = -1;
    m_shaders.at("sun").u_locs["ViewMatrix"] = -1;
    m_shaders.at("sun").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("sun").u_locs["Texture"] = -1;

    m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/planet.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/planet.frag"}}});
    // request uniform locations for shader program
    m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
    m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
    m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("planet").u_locs["CameraPosition"] = -1;
    m_shaders.at("planet").u_locs["LightColor"] = -1;
    m_shaders.at("planet").u_locs["LightPosition"] = -1;
    m_shaders.at("planet").u_locs["LightIntensity"] = -1;
    m_shaders.at("planet").u_locs["Texture"] = -1;

  // create star shader
  m_shaders.emplace("stars", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/stars.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/stars.frag"}}});
  m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;  

  // create orbit shaders
  m_shaders.emplace("orbit", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/orbit.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/orbit.frag"}}});
  m_shaders.at("orbit").u_locs["ModelMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ViewMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ColorVertex"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
  /**
   * Initialize skybox object
   */
  objects.emplace("skybox", std::make_shared<model_object>());
  std::vector<float> skyboxVertices = generateBoxVertices(50);

  // generate vertex array object
  glGenVertexArrays(1, &(objects.at("skybox")->vertex_AO));
  // bind the array for attaching buffers
  glBindVertexArray(objects.at("skybox")->vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &(objects.at("skybox")->vertex_BO));
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, objects.at("skybox")->vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * skyboxVertices.size(), skyboxVertices.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
  
  // store type of primitive to draw
  objects.at("skybox")->draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  objects.at("skybox")->num_elements = GLsizei(skyboxVertices.size() / 3);

  /**
   * Initialize planet objects
   */
  std::vector<std::string> planetTypes{"sphere", "saturn", "sun"};
  for(std::string planetType : planetTypes) {
    objects.emplace(planetType, std::make_shared<model_object>());

    auto planet_model = model_loader::obj(m_resource_path + "models/" + planetType + ".obj", model::NORMAL);

    // generate vertex array object
    glGenVertexArrays(1, &(objects.at(planetType)->vertex_AO));
    // bind the array for attaching buffers
    glBindVertexArray(objects.at(planetType)->vertex_AO);

    // generate generic buffer
    glGenBuffers(1, &(objects.at(planetType)->vertex_BO));
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, objects.at(planetType)->vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    // first attribute is 3 floats with no offset & stride
    glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
    // activate second attribute on gpu
    glEnableVertexAttribArray(1);
    // second attribute is 3 floats with no offset & stride
    glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

    // generate generic buffer
    glGenBuffers(1, &(objects.at(planetType))->element_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects.at(planetType)->element_BO);
    // configure currently bound array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

    // store type of primitive to draw
    objects.at(planetType)->draw_mode = GL_TRIANGLES;
    // transfer number of indices to model object 
    objects.at(planetType)->num_elements = GLsizei(planet_model.indices.size());
  }

  /**
   * Initialize orbit object
   */
  // 360 points in orbit, one per degree
  std::vector<float> orbitVertices{};
  for (int i = 0; i < 360; ++i)
  {
    auto v = glm::rotate(glm::vec3{0.0f,0.0f, 1.0f}, float(i*M_PI/180), glm::vec3{0.0f, 1.0f, 0.0f});
    // position for point in orbit
    orbitVertices.push_back(v.x);
    orbitVertices.push_back(v.y);
    orbitVertices.push_back(v.z); 
    // color for point in orbit
    orbitVertices.push_back(1.0f);
    orbitVertices.push_back(0.0f);
    orbitVertices.push_back(0.0f); 
  }

  //Initialise Vertex Array Object
  objects.emplace("orbit", std::make_shared<model_object>());
  glGenVertexArrays(1, &(objects.at("orbit")->vertex_AO));
  glBindVertexArray(objects.at("orbit")->vertex_AO);
  //Initialise Vertex Buffer Object and load data
  glGenBuffers(1, &(objects.at("orbit")->vertex_BO));
  glBindBuffer(GL_ARRAY_BUFFER, objects.at("orbit")->vertex_BO);
  glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(sizeof(float) * orbitVertices.size()), orbitVertices.data(), GL_STATIC_DRAW);
  //Specify (activate, connect and set format) the Attributes
  glEnableVertexAttribArray(0);
  //Specify the attributes
  // atrribute 0 on GPU
  glEnableVertexAttribArray(0);
  // (index of attribute, number of components, type, normalized, stride = components*num_attributes, pointer to first component of the first attribute)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
  // attribute 1 on GPU
  glEnableVertexAttribArray(1);
  // (....,start of the 2nd attribute is at index 3, type of this is void pointer)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
  // define draw mode
  objects.at("orbit")->draw_mode = GL_LINE_LOOP;
  // define number of elements
  objects.at("orbit")->num_elements = GLsizei(orbitVertices.size() / 6);
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  //multiplier for extra speed
  //speed up when pressing shift in cobination with other keys
  if (key == GLFW_KEY_LEFT_SHIFT  && (action == GLFW_PRESS)) {
    multiplier=4.0f;
  }
  if (key == GLFW_KEY_LEFT_SHIFT  && (action == GLFW_RELEASE)) {
    multiplier=1.0f;
  }
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    graph.camera->setLocalTransform(glm::translate(graph.camera->getLocalTransform(), glm::fvec3{0.0f, 0.0f, multiplier*-0.1f}));
    uploadView();
  }
  if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    graph.camera->setLocalTransform(glm::translate(graph.camera->getLocalTransform(), glm::fvec3{0.0f, 0.0f, multiplier*0.1f}));
    uploadView();
  }
  if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    graph.camera->setLocalTransform(glm::translate(graph.camera->getLocalTransform(), glm::fvec3{multiplier*-0.1f, 0.0f, 0.0f}));
    uploadView();
  }
  if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    graph.camera->setLocalTransform(glm::translate(graph.camera->getLocalTransform(), glm::fvec3{multiplier*0.1f, 0.0f, 0.0f}));
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
  //rotate y axis
  graph.camera->setLocalTransform(glm::rotate(graph.camera->getLocalTransform(), float((pos_x / -50.0f)*M_PI/180), glm::fvec3{0.0f, 1.0f, 0.0f}));
  //rotate x axis
  graph.camera->setLocalTransform(glm::rotate(graph.camera->getLocalTransform(), float((pos_y / -50.0f)*M_PI/180),glm::fvec3{ 1.0f, 0.0f, 0.0f }));
	uploadView();
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  graph.camera->setProjectionMatrix(utils::calculate_projection_matrix(float(width) / float(height)));
  // upload new projection matrix
  uploadProjection();
}

void ApplicationSolar::initalizeStars(){
  int starsCount=200;
  int maxDistanceFromOrign=100;
  for (int i = 0; i < starsCount; ++i)
  {
    //random position for stars
    GLfloat pos_x = rand() % (2*maxDistanceFromOrign + 1) - maxDistanceFromOrign;
    GLfloat pos_y = rand() % (2*maxDistanceFromOrign + 1) - maxDistanceFromOrign;
    GLfloat pos_z = rand() % (2*maxDistanceFromOrign + 1) - maxDistanceFromOrign;
    star_container.push_back(pos_y);
    star_container.push_back(pos_x);
    star_container.push_back(pos_z); 
    //random color for stars
    GLfloat r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    GLfloat g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    GLfloat b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    star_container.push_back(r);
    star_container.push_back(g);
    star_container.push_back(b); 
  }

  //Initialise Vertex Array Object
  glGenVertexArrays(1, &star_object.vertex_AO);
  glBindVertexArray(star_object.vertex_AO);
  //Initialise Vertex Buffer Object and load data
  glGenBuffers(1, &star_object.vertex_BO);
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(sizeof(float) * star_container.size()), star_container.data(), GL_STATIC_DRAW);
  //Specify (activate, connect and set format) the Attributes
  glEnableVertexAttribArray(0);
  //Specify the attributes
  // atrribute 0 on GPU
  glEnableVertexAttribArray(0);
  // (index of attribute, number of components, type, normalized, stride = components*num_attributes, pointer to first component of the first attribute)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
  // attribute 1 on GPU
  glEnableVertexAttribArray(1);
  // (....,start of the 2nd attribute is at index 3, type of this is void pointer)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));
  // define draw mode
  star_object.draw_mode = GL_POINTS;
  // define number of elements
  star_object.num_elements = GLsizei(star_container.size() / 6); 
}

// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}