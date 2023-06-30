#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "scene_graph.hpp"
#include "geometry_node.hpp"

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
  void render();

 protected:
  void initializeShaderPrograms();
  void initializePlanetGeometry();
  void initializeEnterpriseGeometry();
  void initializeStarGeometry();
  void initializeOrbitGeometry();
  void initializeSkyboxGeometry();
  void initializeGeometry();
  void renderSkybox();
  void initializeFrameBuffer();
  void updateBuffer(int width, int height);
  void renderFrameBuffer();
  void createBufferTexture(
          GLuint texture,
          int width,
          int height,
          GLenum target,
          GLenum format,
          GLenum attachment
          );
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();

  // cpu representation of model
  model_object planet_object;
  model_object enterprise_object;
  model_object star_object;
  model_object orbit_object;
  model_object skybox_object;
  texture_object skybox_texture;
  model_object screen_quad_object;

  unsigned int post_process_fbo;
  unsigned int color_texture;
  unsigned int depth_texture;

  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;
  // scene graph for this application
  SceneGraph sceneGraph;
};

#endif