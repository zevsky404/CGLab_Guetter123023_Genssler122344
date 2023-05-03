#include "camera_node.hpp"

/// getter of isPerspective
/// \return bool isPerspective
bool CameraNode::isPerspective() const {
    return isPerspective_;
}

/// setter of isPerspective
/// \param isPerspective
void CameraNode::setIsPerspective(bool isPerspective) {
    CameraNode::isPerspective_ = isPerspective;
}

/// getter of isEnabled
/// \return bool isPerspective
bool CameraNode::isEnabled() const {
    return isEnabled_;
}

/// setter of isEnabled
/// \param isEnabled
void CameraNode::setIsEnabled(bool isEnabled) {
    CameraNode::isEnabled_ = isEnabled;
}

/// getter of ProjectionMatrix
/// \return mat4 ProjectionMatrix
const glm::mat4 &CameraNode::getProjectionMatrix() const {
    return projectionMatrix_;
}

/// setter of ProjectionMatrix
/// \param projectionMatrix
void CameraNode::setProjectionMatrix(const glm::mat4 &projectionMatrix) {
    CameraNode::projectionMatrix_ = projectionMatrix;
}
