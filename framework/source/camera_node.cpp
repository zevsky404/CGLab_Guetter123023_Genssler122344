#include "camera_node.hpp"

///
/// \return
bool CameraNode::isPerspective() const {
    return isPerspective_;
}

///
/// \param isPerspective
void CameraNode::setIsPerspective(bool isPerspective) {
    CameraNode::isPerspective_ = isPerspective;
}

///
/// \return
bool CameraNode::isEnabled1() const {
    return isEnabled_;
}

///
/// \param isEnabled
void CameraNode::setIsEnabled(bool isEnabled) {
    CameraNode::isEnabled_ = isEnabled;
}

///
/// \return
const glm::mat4 &CameraNode::getProjectionMatrix() const {
    return projectionMatrix_;
}

///
/// \param projectionMatrix
void CameraNode::setProjectionMatrix(const glm::mat4 &projectionMatrix) {
    CameraNode::projectionMatrix_ = projectionMatrix;
}
