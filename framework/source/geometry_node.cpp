#include "geometry_node.hpp"

GeometryNode::GeometryNode(Node parent, std::string const& name, std::string const& geometry_path):
    Node{parent, name},
    geometry_path_{geometry_path}{}

const model &GeometryNode::getGeometry() const {
    return geometry_;
}

void GeometryNode::setGeometry(const model &geometry) {
    geometry_ = geometry;
}

std::string const& GeometryNode::getGeometry() const {
    return geometry_path_;
}
