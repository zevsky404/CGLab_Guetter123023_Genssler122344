#ifndef OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
#define OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP

#include "model.hpp"
#include <node.hpp>
#include <utility>

class GeometryNode : public Node {
private:
    model_object geometry_;
    texture_object texture_;

public:
    //default constructor
    GeometryNode() = default;

    GeometryNode(std::shared_ptr<Node> parent, std::string const& name):
    Node::Node(std::move(parent), name),
    geometry_{},
    texture_{}
    {};

    GeometryNode(std::shared_ptr<Node> parent,
                 std::string const& name,
                 model_object geometry):
    Node::Node(std::move(parent), name),
    geometry_{geometry},
    texture_{}
    {};

    GeometryNode(std::shared_ptr<Node> parent,
                 std::string const& name,
                 model_object const& geometry,
                 glm::vec3 const& color):
    Node::Node(std::move(parent), name, color),
    geometry_{geometry},
    texture_{}
    {};

    GeometryNode(std::shared_ptr<Node> parent,
                 std::string const& name,
                 model_object geometry,
                 texture_object texture):
    Node::Node(std::move(parent), name),
    geometry_{geometry},
    texture_{texture}
    {};

    //get geometry variable
    const model_object &getGeometry() const;
    //set geometry variable
    void setGeometry(const model_object &geometry);

    const texture_object &getTexture() const;

    void setTexture(const texture_object &texture);

    void renderPlanet(const std::map<std::string, shader_program> &m_shaders,
                      const glm::mat4 &m_view_transform) const;

    void renderPlanet(const std::map<std::string, shader_program> &m_shaders,
                      const glm::mat4 &m_view_transform, texture_object const& texture) const;

    void renderStars(const std::map<std::string, shader_program> &m_shaders,
                                   const glm::mat4 &m_view_transform) const;
    void renderOrbit(const std::map<std::string, shader_program> &m_shaders,
                                   const glm::mat4 &m_view_transform) const;
    void renderEnterprise(const std::map<std::string, shader_program> &m_shaders,
                          const glm::mat4 &m_view_transform) const;

    //render function for geometry node
    void renderNode(std::map<std::string, shader_program> const& m_shaders, glm::mat4 const& m_view_transform) override;
    void renderNode(std::map<std::string, shader_program> const& m_shaders, glm::mat4 const& m_view_transform,
                    texture_object const& texture);
};

#endif //OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
