//
// Created by neo on 27.04.23.
//

#include "camera_node.hpp"

bool CameraNode::isPerspective1() const {
    return isPerspective_;
}

void CameraNode::setIsPerspective(bool isPerspective) {
    CameraNode::isPerspective_ = isPerspective;
}

bool CameraNode::isEnabled1() const {
    return isEnabled_;
}

void CameraNode::setIsEnabled(bool isEnabled) {
    CameraNode::isEnabled_ = isEnabled;
}

const glm::mat4 &CameraNode::getProjectionMatrix() const {
    return projectionMatrix_;
}

void CameraNode::setProjectionMatrix(const glm::mat4 &projectionMatrix) {
    CameraNode::projectionMatrix_ = projectionMatrix;
}
