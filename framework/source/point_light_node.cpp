#include "point_light_node.hpp"

/// getter for member variable LightColor
/// \return lightColor_
const glm::vec3& PointLightNode::getLightColour() const {
    return LightColour_;
}

/// setter for light color
/// \param lightColour
void PointLightNode::setLightColour(const glm::vec3 &lightColour) {
    LightColour_ = lightColour;
}

/// getter for light intensity
/// \return
float PointLightNode::getLightIntensity() const {
    return LightIntensity_;
}

/// setter for light intensity
/// \param lightIntensity
void PointLightNode::setLightIntensity(float lightIntensity) {
    LightIntensity_ = lightIntensity;
}

/*///
/// \param m_shaders
/// \param m_view_transform
void PointLightNode::renderNode(std::map<std::string, shader_program> const& m_shaders, glm::mat4 const& m_view_transform) {
    glm::mat4 rotation_test = glm::rotate(glm::fmat4{}, glm::radians(0.01f), glm::fvec3{0.0f, 1.0f, 0.0f});
    setLocalTransform(rotation_test * getLocalTransform());

    //for (auto const& child : children_) {
    //    child->renderNode(m_shaders, m_view_transform);
    //}
}*/