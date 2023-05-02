#ifndef OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
#define OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP

#include "model.hpp"
#include <node.hpp>

class GeometryNode : public Node {
private:
    model geometry_;
    std::string geometry_path_;
    GeometryNode() = default;


public:
    const model &getGeometry() const;
    void setGeometry(const model &geometry);
};

#endif //OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
