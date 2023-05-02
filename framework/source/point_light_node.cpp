#include "point_light_node.hpp"

const glm::vec3& PointLightNode::getLightColour() const {
    return LightColour_;
}

void PointLightNode::setLightColour(const glm::vec3 &lightColour) {
    LightColour_ = lightColour;
}

float PointLightNode::getLightIntensity() const {
    return LightIntensity_;
}

void PointLightNode::setLightIntensity(float lightIntensity) {
    LightIntensity_ = lightIntensity;
}