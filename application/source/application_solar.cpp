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

#include <iostream>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::rotate(glm::translate(glm::fmat4{}, glm::fvec3{0.0f, -40.0f, 10.0f}), float(1.57), glm::fvec3{1.0f, 0.0f, 0.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
 ,graph{}
{
  initializeObjects();
  initializeGeometry();  
  initializeShaderPrograms();
  
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::initializeObjects(){
  std::shared_ptr<Node> root = graph.getRoot();
  CameraNode camera = CameraNode("Camera1");;
  //root->addChildren(camera);
  GeometryNode mercury = GeometryNode("mercury");
  GeometryNode venus = GeometryNode("venus");
  GeometryNode earth = GeometryNode("earth");
  GeometryNode mars = GeometryNode("mars");
  GeometryNode jupiter = GeometryNode("jupiter");
  GeometryNode saturn = GeometryNode("saturn");
  GeometryNode uranus = GeometryNode("uranus");
  GeometryNode neptun = GeometryNode("neptun");
  mercury.setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  venus.setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  earth.setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  mars.setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  jupiter.setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  saturn.setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  uranus.setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  neptun.setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
  root->addChildren(mercury);
  root->addChildren(venus);
  root->addChildren(earth);
  root->addChildren(mars);
  root->addChildren(jupiter);
  root->addChildren(saturn);
  root->addChildren(uranus);
  root->addChildren(neptun);
  

}

void ApplicationSolar::render() const {
  
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    float radius = -1.0f;
    
    
    if (planet->getName() == "mercury") {
      radius = -3.0f;
    }
    else if (planet->getName() == "venus") {
      radius = -8.0f;
    }
    else if (planet->getName() == "earth") {
      radius = -14.0f;
    }
    else if (planet->getName() == "mars") {
      radius = -20.0f;
    }
    else if (planet->getName() == "jupiter") {
      radius = -28.0f;
    }
    else if (planet->getName() == "saturn") {
      radius = -33.0f;
    }
    else if (planet->getName() == "uranus") {
      radius = -38.0f;
    }
    else if (planet->getName() == "neptun") {
      radius = -45.0f;
    }

    // bind shader to upload uniforms
    glUseProgram(m_shaders.at(planet->getName()).handle);
    planet->setLocalTransform(glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f}));
    planet->setLocalTransform(glm::translate(planet->getLocalTransform(), glm::fvec3{0.0f, 0.0f, radius}));
    glUniformMatrix4fv(m_shaders.at(planet->getName()).u_locs.at("ModelMatrix"),
                      1, GL_FALSE, glm::value_ptr(planet->getLocalTransform()));

    // extra matrix for normal transformation to keep them orthogonal to surface
    planet->setWorldTransform(glm::inverseTranspose(glm::inverse(m_view_transform) * planet->getLocalTransform()));
    glUniformMatrix4fv(m_shaders.at(planet->getName()).u_locs.at("NormalMatrix"),
                      1, GL_FALSE, glm::value_ptr(planet->getWorldTransform()));
    
    // bind the VAO to draw
    glBindVertexArray(planet_objects.at(planet->getName()).vertex_AO);

    // draw bound vertex array using bound shader
    glDrawElements(planet_objects.at(planet->getName()).draw_mode, planet_objects.at(planet->getName()).num_elements, model::INDEX.type, NULL);

  }
  
}

void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    glUseProgram(m_shaders.at(planet->getName()).handle);
    glUniformMatrix4fv(m_shaders.at(planet->getName()).u_locs.at("ViewMatrix"),
                      1, GL_FALSE, glm::value_ptr(view_matrix));
  }
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    glUseProgram(m_shaders.at(planet->getName()).handle);
    glUniformMatrix4fv(m_shaders.at(planet->getName()).u_locs.at("ProjectionMatrix"),
                      1, GL_FALSE, glm::value_ptr(m_view_projection));
  }
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    glUseProgram(m_shaders.at(planet->getName()).handle);
  
  }
    // upload uniform values to new locations
    uploadView();
    uploadProjection();
  
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    // store shader program objects in container
    m_shaders.emplace(planet->getName(), shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
    // request uniform locations for shader program
    m_shaders.at(planet->getName()).u_locs["NormalMatrix"] = -1;
    m_shaders.at(planet->getName()).u_locs["ModelMatrix"] = -1;
    m_shaders.at(planet->getName()).u_locs["ViewMatrix"] = -1;
    m_shaders.at(planet->getName()).u_locs["ProjectionMatrix"] = -1;
  }
}

// load models
void ApplicationSolar::initializeGeometry() {
  for(std::shared_ptr<Node> planet : graph.getRoot()->getChildrenList()) {
    planet_objects.emplace(planet->getName(), model_object{});

    model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

    // generate vertex array object
    glGenVertexArrays(1, &(planet_objects.at(planet->getName())).vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(planet_objects.at(planet->getName()).vertex_AO);

    // generate generic buffer
    glGenBuffers(1, &(planet_objects.at(planet->getName())).vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, planet_objects.at(planet->getName()).vertex_BO);
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
    glGenBuffers(1, &(planet_objects.at(planet->getName())).element_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_objects.at(planet->getName()).element_BO);
    // configure currently bound array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

    // store type of primitive to draw
    planet_objects.at(planet->getName()).draw_mode = GL_TRIANGLES;
    // transfer number of indices to model object 
    planet_objects.at(planet->getName()).num_elements = GLsizei(planet_model.indices.size());
  }
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.1f, 0.0f, 0.0f});
    uploadView();
  }
  else if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.1f, 0.0f, 0.0f});
    uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
  //m_view_transform = glm::translate(m_view_transform, glm::fvec3{ pos_x*0.01f, pos_y*-0.01f, 0.0f });
	//uploadView();
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}