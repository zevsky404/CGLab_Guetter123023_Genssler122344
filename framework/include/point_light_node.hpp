#ifndef OPENGL_FRAMEWORK_POINT_LIGHT_NODE_HPP
#define OPENGL_FRAMEWORK_POINT_LIGHT_NODE_HPP

#include "node.hpp"
#include <memory>
#include <glm/glm.hpp>

class PointLightNode: public Node {
public:
    //default constructor
    PointLightNode() = default;

    PointLightNode(glm::vec3 const& light_colour, float light_intensity):
            Node(),
            LightColour_{light_colour},
            LightIntensity_{light_intensity} {};

    PointLightNode(std::shared_ptr<Node> parent, std::string const& name, glm::vec3 const& light_colour, float light_intensity):
            Node(std::move(parent), name),
            LightColour_{light_colour},
            LightIntensity_{light_intensity} {};

    const glm::vec3 &getLightColour() const;
    void setLightColour(const glm::vec3 &lightColour);

    float getLightIntensity() const;
    void setLightIntensity(float lightIntensity);

private:
    glm::vec3 LightColour_ = {1.0f, 1.0f, 1.0f};
    float LightIntensity_ = 1.0f;
};

#endif //OPENGL_FRAMEWORK_POINT_LIGHT_NODE_HPP
