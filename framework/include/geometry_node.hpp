#ifndef OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
#define OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP

#include "model.hpp"

class GeometryNode {
private:
    model geometry_;

public:
    const model &getGeometry() const;

    void setGeometry(const model &geometry);
};

#endif //OPENGL_FRAMEWORK_GEOMETRY_NODE_HPP
