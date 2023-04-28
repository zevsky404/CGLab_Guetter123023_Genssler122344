#include "geometry_node.hpp"

const model &GeometryNode::getGeometry() const {
    return geometry_;
}

void GeometryNode::setGeometry(const model &geometry) {
    geometry_ = geometry;
}
