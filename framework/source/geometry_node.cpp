#include "geometry_node.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

///
/// \return
const model_object &GeometryNode::getGeometry() const {
    return geometry_;
}

///
/// \param geometry
void GeometryNode::setGeometry(const model_object &geometry) {
    geometry_ = geometry;
}

///
/// \param m_shaders
/// \param m_view_transform
void GeometryNode::renderNode(const std::map<std::string, shader_program> &m_shaders,
                              const glm::mat4 &m_view_transform) {
    // bind shader to upload uniforms
    glUseProgram(m_shaders.at("planet").handle);

    glm::fmat4 model_matrix = getWorldTransform() * getLocalTransform();
    // model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -1.0f});
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                       1, GL_FALSE, glm::value_ptr(model_matrix));

    // extra matrix for normal transformation to keep them orthogonal to surface
    glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
    gl::glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                       1, GL_FALSE, glm::value_ptr(normal_matrix));

    // bind the VAO to draw
    gl::glBindVertexArray(geometry_.vertex_AO);

    // draw bound vertex array using bound shader
    gl::glDrawElements(geometry_.draw_mode, geometry_.num_elements, model::INDEX.type, NULL);
}
