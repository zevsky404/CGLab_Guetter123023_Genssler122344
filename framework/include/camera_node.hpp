#ifndef OPENGL_FRAMEWORK_CAMERA_NODE_HPP
#define OPENGL_FRAMEWORK_CAMERA_NODE_HPP

#include <glm/glm.hpp>

class CameraNode {
private:
    bool isPerspective_;
    bool isEnabled_;
    glm::mat4 projectionMatrix_;

public:
    bool isPerspective1() const;
    void setIsPerspective(bool isPerspective);

    bool isEnabled1() const;
    void setIsEnabled(bool isEnabled);

    const glm::mat4 &getProjectionMatrix() const;
    void setProjectionMatrix(const glm::mat4 &projectionMatrix);

};

#endif //OPENGL_FRAMEWORK_CAMERA_NODE_HPP
