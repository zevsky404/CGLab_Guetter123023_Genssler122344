#ifndef OPENGL_FRAMEWORK_CAMERA_NODE_HPP
#define OPENGL_FRAMEWORK_CAMERA_NODE_HPP

#include <glm/glm.hpp>
#include <node.hpp>

class CameraNode : public Node{
private:
    bool isPerspective_ = false;
    bool isEnabled_ = false;
    glm::mat4 projectionMatrix_;

public:
    CameraNode() = default;

    explicit CameraNode(std::shared_ptr<Node> parent, std::string name):
    Node::Node(parent, std::move(name)),
    isPerspective_{},
    isEnabled_{},
    projectionMatrix_{}
    {}

    bool isPerspective() const;
    void setIsPerspective(bool isPerspective);

    bool isEnabled1() const;
    void setIsEnabled(bool isEnabled);

    const glm::mat4 &getProjectionMatrix() const;
    void setProjectionMatrix(const glm::mat4 &projectionMatrix);

};

#endif //OPENGL_FRAMEWORK_CAMERA_NODE_HPP
