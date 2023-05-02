#include "node.hpp"
#include <algorithm>
#include <iterator>
#include <ostream>
#include <glm/glm.hpp>

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "structs.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>


// Getters and setters, class utility
# pragma region
const std::shared_ptr<Node> Node::getParent() const{
    return parent_;
};

void Node::setParent(std::shared_ptr<Node> node){
    parent_ = node;
};

const std::vector<std::shared_ptr<Node>> &Node::getChildren() const {
    return children_;
}

const std::shared_ptr<Node> Node::getChild(const std::string &name) const {
    auto foundChild = std::find_if(children_.begin(), children_.end(), [name] (std::shared_ptr<Node> const& child) {return name == (*child).name_;});
    return *foundChild;
}

const std::string &Node::getName() const {
    return name_;
}

void Node::setName(const std::string &name) {
    name_ = name;
}

void Node::setChildren(const std::vector<std::shared_ptr<Node>> &children) {
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

void Node::addChild(std::shared_ptr<Node> child) {
    (*child).depth_ = getDepth() + 1;
    children_.push_back(child);
}

std::shared_ptr<Node> Node::removeChild(const std::string &child_name) {
    auto found_child = std::remove_if(children_.begin(), children_.end(), [child_name] (std::shared_ptr<Node> const& child) {return child_name == (*child).name_;});
    return *found_child;
}


std::ostream &operator<<(std::ostream &os, const Node &node) {
    os << "Parent:" << node.parent_ << std::endl;
    os << "Name: " << node.name_ << std::endl;
    os << "Path in tree: " << node.path_ << std::endl;
    os << "Depth in tree: " << node.depth_ << std::endl;
    os << "Names of direct children: \n";
    for (auto child_node : node.children_) {
        os << (*child_node).name_ << std::endl;
    }
}

void Node::renderNode(std::map<std::string, shader_program> const& m_shaders, glm::mat4 const& m_view_transform) {
    glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f}) * local_transform_;
    // model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -3.0f});

    for (auto const& child : children_) {
        child->renderNode(m_shaders, m_view_transform);
    }
}

void Node::applyFunction(const std::function<void(Node)> &functionObject) {
    for (auto child : children_) {
        functionObject((*child));
        (*child).applyFunction(functionObject);
    }
}

Node::~Node() = default;
#pragma endregion




