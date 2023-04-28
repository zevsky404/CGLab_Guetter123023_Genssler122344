#include "node.hpp"
#include <algorithm>
#include <iterator>
#include <ostream>

// Getters and Setters
# pragma region
const std::vector<Node> &Node::getChildren() const {
    return children_;
}

const std::string &Node::getName() const {
    return name_;
}

void Node::setName(const std::string &name) {
    name_ = name;
}

void Node::setChildren(const std::vector<Node> &children) {
    children_ = children;
}

const std::string &Node::getPath() const {
    return path_;
}

void Node::setPath(const std::string &path) {
    path_ = path;
}

int Node::getDepth() const {
    return depth_;
}

void Node::setDepth(int depth) {
    depth_ = depth;
}

const glm::mat4 &Node::getLocalTransform() const {
    return local_transform_;
}

void Node::setLocalTransform(const glm::mat4 &localTransform) {
    local_transform_ = localTransform;
}

const glm::mat4 &Node::getWorldTransform() const {
    return world_transform_;
}

void Node::setWorldTransform(const glm::mat4 &worldTransform) {
    world_transform_ = worldTransform;
}

void Node::addChildren(const Node &child) {
    children_.push_back(child);
}

Node Node::removeChildren(const std::string &child_name) {
    auto found_child = std::remove_if(children_.begin(), children_.end(), [child_name] (Node const& child) {return child_name == child.name_;});
    return *found_child;
}


std::ostream &operator<<(std::ostream &os, const Node &node) {
    os << "Name: " << node.name_ << std::endl;
    os << "Path in tree: " << node.path_ << std::endl;
    os << "Depth in tree: " << node.depth_ << std::endl;
    os << "Names of direct children: \n";
    for (auto const& child_node : node.children_) {
        os << child_node.name_ << std::endl;
    }
}


#pragma endregion

