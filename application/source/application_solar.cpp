#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "geometry_node.hpp"
#include "camera_node.hpp"

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

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,star_object{}
 ,star_container{}
 ,orbit_containers{}
 ,graph{}
{
  initializeObjects();
  initializeGeometry();  
  initalizeStars();
  initalizeOrbits();
  initializeShaderPrograms();
  
  
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
  glDeleteBuffers(1, &star_object.vertex_BO);
  glDeleteBuffers(1, &star_object.element_BO);
  glDeleteVertexArrays(1, &star_object.vertex_AO);
}

void ApplicationSolar::initializeObjects(){
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
  auto moon = std::make_shared<GeometryNode>(GeometryNode("moon", 5.0f, glm::vec3{141.0f/255.0f, 141.0f/255.0f, 141.0f/255.0f}));

  sun->setGeometry(model_loader::obj(m_resource_path + "models/sun.obj", model::NORMAL));
  mercury->setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  venus->setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  earth->setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  mars->setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  jupiter->setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  saturn->setGeometry(model_loader::obj(m_resource_path + "models/saturn.obj", model::NORMAL));
  uranus->setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  neptun->setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  moon->setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));

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
}

void ApplicationSolar::render() const {
  
  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("stars").handle);
  // bind the VAO to draw
  glBindVertexArray(star_object.vertex_AO);
  
  // draw bound vertex array using bound shader
  glDrawArrays(star_object.draw_mode, 0, star_object.num_elements);

  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    // bind shader to upload uniforms
    glUseProgram(m_shaders.at(planet->getName() + "-orbit").handle);
    // bind the VAO to draw
    glBindVertexArray(orbit_objects.at(planet->getName()).vertex_AO);
    
    // draw bound vertex array using bound shader
    glDrawArrays(orbit_objects.at(planet->getName()).draw_mode, 0, orbit_objects.at(planet->getName()).num_elements);
  }

  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList(true)) {
    if (planet->getName() == "camera") {
      continue;
    }

    std::shared_ptr<GeometryNode> gPlanet = std::static_pointer_cast<GeometryNode>(planet);

    // bind shader to upload uniforms
    glUseProgram(m_shaders.at(planet->getName()).handle);

    // rotate from parent transformation matrix
    planet->setLocalTransform(glm::rotate(planet->getParent()->getLocalTransform(), float(planet->getSpeed()/100.0f*glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f}));

    // TODO: add size as node variable
    if (planet->getName() == "moon") { // scale down the earths moon
      planet->setLocalTransform(glm::scale(planet->getLocalTransform(), glm::fvec3{0.5f, 0.5f, 0.5f}));
    }
    else if (planet->getName() == "sun") { // scale up the sun
      planet->setLocalTransform(glm::scale(planet->getLocalTransform(), glm::fvec3{2.0f, 2.0f, 2.0f}));
    }
    /* else if (planet->getName() == "mercury") { // scale down mercury
      planet->setLocalTransform(glm::scale(planet->getLocalTransform(), glm::fvec3{0.7f, 0.7f, 0.7f}));
    } */

    // translate according to radius
    planet->setLocalTransform(glm::translate(planet->getLocalTransform(), glm::fvec3{0.0f, 0.0f, planet->getOrbit()}));
    glUniformMatrix4fv(m_shaders.at(planet->getName()).u_locs.at("ModelMatrix"),
                      1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

    // extra matrix for normal transformation to keep them orthogonal to surface
    planet->setWorldTransform(glm::inverseTranspose(glm::inverse(graph.camera->getLocalTransform()) * planet->getLocalTransform()));
    glUniformMatrix4fv(m_shaders.at(planet->getName()).u_locs.at("NormalMatrix"),
                      1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));
    
    // bind the VAO to draw
    glBindVertexArray(planet_objects.at(planet->getName()).vertex_AO);

    // color vertex
    glUniform3f(m_shaders.at(planet->getName()).u_locs.at("ColorVertex"), gPlanet->getColor().r, gPlanet->getColor().g, gPlanet->getColor().b);

    // camera position
    auto cameraPos = (graph.camera->getLocalTransform() * glm::vec4{0.0f, 0.0f, 0.0f, 1.0f});
    glUniform3f(m_shaders.at(planet->getName()).u_locs.at("CameraPosition"), cameraPos.x, cameraPos.y, cameraPos.z);

    // draw bound vertex array using bound shader
    glDrawElements(planet_objects.at(planet->getName()).draw_mode, planet_objects.at(planet->getName()).num_elements, model::INDEX.type, NULL);
  }
  
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(graph.camera->getLocalTransform());
  // upload matrix to gpu
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList(true)) {
    // bind shader to which to upload unforms
    glUseProgram(m_shaders.at(planet->getName()).handle);
    glUniformMatrix4fv(m_shaders.at(planet->getName()).u_locs.at("ViewMatrix"),
                      1, GL_FALSE, glm::value_ptr(view_matrix));
  }
  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix)); 

  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    // bind shader to upload uniforms
    glUseProgram(m_shaders.at(planet->getName() + "-orbit").handle);

    glUniformMatrix4fv(m_shaders.at(planet->getName() + "-orbit").u_locs.at("ViewMatrix"),
                      1, GL_FALSE, glm::value_ptr(view_matrix)); 
  }

}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList(true)) {
    // bind shader to which to upload unforms
    glUseProgram(m_shaders.at(planet->getName()).handle);
    glUniformMatrix4fv(m_shaders.at(planet->getName()).u_locs.at("ProjectionMatrix"),
                      1, GL_FALSE, glm::value_ptr(graph.camera->getProjectionMatrix()));
  }

  // handle star shaders
  glUseProgram(m_shaders.at("stars").handle);
  glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(graph.camera->getProjectionMatrix()));     

  // handle orbit shaders
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    glUseProgram(m_shaders.at(planet->getName() + "-orbit").handle);
    glUniformMatrix4fv(m_shaders.at(planet->getName() + "-orbit").u_locs.at("ProjectionMatrix"),
                      1, GL_FALSE, glm::value_ptr(graph.camera->getProjectionMatrix()));    
  }
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
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList(true)) {
    // store shader program objects in container
    m_shaders.emplace(planet->getName(), shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
    // request uniform locations for shader program
    m_shaders.at(planet->getName()).u_locs["NormalMatrix"] = -1;
    m_shaders.at(planet->getName()).u_locs["ModelMatrix"] = -1;
    m_shaders.at(planet->getName()).u_locs["ViewMatrix"] = -1;
    m_shaders.at(planet->getName()).u_locs["ProjectionMatrix"] = -1;
    m_shaders.at(planet->getName()).u_locs["ColorVertex"] = -1;
    m_shaders.at(planet->getName()).u_locs["CameraPosition"] = -1;
  }

  // create star shader
  m_shaders.emplace("stars", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/stars.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/stars.frag"}}});
  m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
  m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;  

  // create orbit shaders
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    m_shaders.emplace(planet->getName() + "-orbit", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/stars.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/stars.frag"}}});
    m_shaders.at(planet->getName() + "-orbit").u_locs["ViewMatrix"] = -1;
    m_shaders.at(planet->getName() + "-orbit").u_locs["ProjectionMatrix"] = -1;  
  }
}

// load models
void ApplicationSolar::initializeGeometry() {
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList(true)) {
    planet_objects.emplace(planet->getName(), model_object{});

    //std::shared_ptr<model> planet_model = std::static_pointer_cast<GeometryNode>(planet)->getGeometry(); 
    std::shared_ptr<model> planet_model;
    if (planet->getName() == "saturn") {
      planet_model= std::make_shared<model>(model_loader::obj(m_resource_path + "models/saturn.obj", model::NORMAL));
    }
    else if (planet->getName() == "sun") {
      planet_model= std::make_shared<model>(model_loader::obj(m_resource_path + "models/sun.obj", model::NORMAL));
    }
    else
    {
      planet_model= std::make_shared<model>(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
    }
    
    // generate vertex array object
    glGenVertexArrays(1, &(planet_objects.at(planet->getName())).vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(planet_objects.at(planet->getName()).vertex_AO);

    // generate generic buffer
    glGenBuffers(1, &(planet_objects.at(planet->getName())).vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, planet_objects.at(planet->getName()).vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model->data.size(), planet_model->data.data(), GL_STATIC_DRAW);

    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    // first attribute is 3 floats with no offset & stride
    glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model->vertex_bytes, planet_model->offsets[model::POSITION]);
    // activate second attribute on gpu
    glEnableVertexAttribArray(1);
    // second attribute is 3 floats with no offset & stride
    glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model->vertex_bytes, planet_model->offsets[model::NORMAL]);

    // generate generic buffer
    glGenBuffers(1, &(planet_objects.at(planet->getName())).element_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_objects.at(planet->getName()).element_BO);
    // configure currently bound array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model->indices.size(), planet_model->indices.data(), GL_STATIC_DRAW);

    // store type of primitive to draw
    planet_objects.at(planet->getName()).draw_mode = GL_TRIANGLES;
    // transfer number of indices to model object 
    planet_objects.at(planet->getName()).num_elements = GLsizei(planet_model->indices.size());
  }
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

void ApplicationSolar::initalizeOrbits(){
  // 360 points in orbit, one per degree
  int orbitCount=360;

  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    orbit_containers.emplace(planet->getName(), std::vector<GLfloat>{});
    for (int i = 0; i < orbitCount; ++i)
    {
      auto v = glm::rotate(glm::vec3{0.0f,0.0f,planet->getOrbit()}, float(i*M_PI/180), glm::vec3{0.0f, 1.0f, 0.0f});
      // position for point in orbit
      orbit_containers.at(planet->getName()).push_back(v.x);
      orbit_containers.at(planet->getName()).push_back(v.y);
      orbit_containers.at(planet->getName()).push_back(v.z); 
      // color for point in orbit
      orbit_containers.at(planet->getName()).push_back(1.0f);
      orbit_containers.at(planet->getName()).push_back(0.0f);
      orbit_containers.at(planet->getName()).push_back(0.0f); 
    }

    //Initialise Vertex Array Object
    orbit_objects.emplace(planet->getName(), model_object{});
    glGenVertexArrays(1, &orbit_objects.at(planet->getName()).vertex_AO);
    glBindVertexArray(orbit_objects.at(planet->getName()).vertex_AO);
    //Initialise Vertex Buffer Object and load data
    glGenBuffers(1, &orbit_objects.at(planet->getName()).vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, orbit_objects.at(planet->getName()).vertex_BO);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(sizeof(float) * orbit_containers.at(planet->getName()).size()), orbit_containers.at(planet->getName()).data(), GL_STATIC_DRAW);
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
    orbit_objects.at(planet->getName()).draw_mode = GL_LINE_LOOP;
    // define number of elements
    orbit_objects.at(planet->getName()).num_elements = GLsizei(orbit_containers.at(planet->getName()).size() / 6);

  }
}

// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}