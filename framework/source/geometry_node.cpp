#include "geometry_node.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

/// getter of geometry
/// \return model_object geometry
const model_object &GeometryNode::getGeometry() const {
    return geometry_;
}

/// setter for geometry
/// \param geometry
void GeometryNode::setGeometry(const model_object &geometry) {
    geometry_ = geometry;
}

void GeometryNode::renderPlanet(const std::map<std::string, shader_program> &m_shaders,
                                const glm::mat4 &m_view_transform) const {

    // bind shader to upload uniforms
    glUseProgram(m_shaders.at("planet").handle);
    // rotate planets around own y-axis
    glm::fmat4 model_matrix = getWorldTransform() * getLocalTransform();
    model_matrix = model_matrix * glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f});
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                       1, GL_FALSE, glm::value_ptr(model_matrix));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(model_matrix);
    gl::glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                           1, GL_FALSE, glm::value_ptr(normal_matrix));

    gl::glUniform3f(m_shaders.at("planet").u_locs.at("PlanetColor"),
                    color_.x, color_.y, color_.z);

    gl::glUniform3f(m_shaders.at("planet").u_locs.at("AmbientColor"),
                    color_.x, color_.y, color_.z);

    // bind the VAO to draw
    gl::glBindVertexArray(geometry_.vertex_AO);

    // draw bound vertex array using bound shader
    gl::glDrawElements(geometry_.draw_mode, geometry_.num_elements, model::INDEX.type, NULL);
}

/// renders the stars using glDrawArray
/// \param m_shaders shader information
/// \param m_view_transform camera information
void GeometryNode::renderStars(const std::map<std::string, shader_program> &m_shaders,
                               const glm::mat4 &m_view_transform) const {

    glUseProgram(m_shaders.at("stars").handle);
    glm::fmat4 model_matrix = getWorldTransform() * getLocalTransform();

    glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ModelMatrix"),
                       1, GL_FALSE, glm::value_ptr(model_matrix));

    gl::glBindVertexArray(geometry_.vertex_AO);
    glDrawArrays(geometry_.draw_mode, 0,geometry_.num_elements);
}

/// renders orbits using glDrawArray
/// \param m_shaders shader information
/// \param m_view_transform camera information
void GeometryNode::renderOrbit(const std::map<std::string, shader_program> &m_shaders,
                               const glm::mat4 &m_view_transform) const {
    glUseProgram(m_shaders.at("orbit").handle);
    glm::fmat4 model_matrix = getWorldTransform() * getLocalTransform();

    glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ModelMatrix"),
                       1, GL_FALSE, glm::value_ptr(model_matrix));

    gl::glBindVertexArray(geometry_.vertex_AO);

    glDrawArrays(geometry_.draw_mode,0, geometry_.num_elements);
}

/// renders USS Enterprise orbiting around Jupiter
/// \param m_shaders
/// \param m_view_transform
void GeometryNode::renderEnterprise(const std::map<std::string, shader_program> &m_shaders,
                                    const glm::mat4 &m_view_transform) const {

    glUseProgram(m_shaders.at("enterprise").handle);
    glm::fmat4 model_matrix = getWorldTransform() * getLocalTransform();
    glUniformMatrix4fv(m_shaders.at("enterprise").u_locs.at("ModelMatrix"),
                       1, GL_FALSE, glm::value_ptr(model_matrix));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
    gl::glUniformMatrix4fv(m_shaders.at("enterprise").u_locs.at("NormalMatrix"),
                           1, GL_FALSE, glm::value_ptr(normal_matrix));
    // bind the VAO to draw
    gl::glBindVertexArray(geometry_.vertex_AO);

    // draw bound vertex array using bound shader
    gl::glDrawElements(geometry_.draw_mode, geometry_.num_elements, model::INDEX.type, NULL);
}

/// render geometry Node
/// \param m_shaders shader information
/// \param m_view_transform cemara information
void GeometryNode::renderNode(const std::map<std::string, shader_program> &m_shaders,
                              const glm::mat4 &m_view_transform) {
    // sorts out stars and orbit, as they need to be rendered differently from planets
    if (name_.find("Planet") != std::string::npos) {
        renderPlanet(m_shaders, m_view_transform);
    } else if (name_ == "Star-Geometry") {
        renderStars(m_shaders, m_view_transform);
    }
    else if (name_ == "Orbit") {
        renderOrbit(m_shaders, m_view_transform);
    } else if (name_ == "Enterprise-Geometry") {
        renderEnterprise(m_shaders, m_view_transform);
    }
}
