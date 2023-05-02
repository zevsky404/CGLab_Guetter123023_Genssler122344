#ifndef OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
#define OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP

#include "model.hpp"
#include <node.hpp>
#include <utility>

class GeometryNode : public Node {
private:
    model_object geometry_;

public:
    GeometryNode() = default;
    GeometryNode(std::shared_ptr<Node> parent, std::string const& name):
        Node::Node(std::move(parent), name),
        geometry_{} {};

    GeometryNode(std::shared_ptr<Node> parent, std::string const& name, model_object geometry):
        Node::Node(std::move(parent), name),
        geometry_{geometry} {};

    const model_object &getGeometry() const;
    void setGeometry(const model_object &geometry);

    const std::string &getGeometryPath() const;

    void renderNode(std::map<std::string, shader_program> const& m_shaders, glm::mat4 const& m_view_transform) override;
};

#endif //OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
