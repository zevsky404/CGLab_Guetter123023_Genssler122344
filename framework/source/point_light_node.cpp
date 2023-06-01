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
