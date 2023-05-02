#ifndef OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
#define OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP

#include "model.hpp"
#include <node.hpp>

class GeometryNode : public Node {
private:
    model geometry_;
    std::string geometry_path_;

public:
    GeometryNode() = default;
    GeometryNode(std::shared_ptr<Node> parent, std::string const& name, std::string const& geometry_path):
    Node::Node(parent, name),
    geometry_{},
    geometry_path_{geometry_path}
    {}

    const model &getGeometry() const;
    void setGeometry(const model &geometry);

    const std::string &getGeometryPath() const;
};

#endif //OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
