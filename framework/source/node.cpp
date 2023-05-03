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

///
/// \return parent node
const std::shared_ptr<Node> Node::getParent() const{
    return parent_;
};

///
/// \param node
void Node::setParent(std::shared_ptr<Node> node){
    parent_ = node;
};

///
/// \return
const std::vector<std::shared_ptr<Node>> &Node::getChildren() const {
    return children_;
}

///
/// \param name
/// \return
const std::shared_ptr<Node> Node::getChild(const std::string &name) const {
    auto foundChild = std::find_if(children_.begin(), children_.end(), [name] (std::shared_ptr<Node> const& child) {return name == (*child).name_;});
    return *foundChild;
}

///
/// \return
const std::string &Node::getName() const {
    return name_;
}

///
/// \param name
void Node::setName(const std::string &name) {
    name_ = name;
}

///
/// \param children
void Node::setChildren(const std::vector<std::shared_ptr<Node>> &children) {
    children_ = children;
}

///
/// \return
const std::string &Node::getPath() const {
    return path_;
}

///
/// \param path
void Node::setPath(const std::string &path) {
    path_ = path;
}

///
/// \return
int Node::getDepth() const {
    return depth_;
}

///
/// \param depth
void Node::setDepth(int depth) {
    depth_ = depth;
}

///
/// \return
const glm::mat4 &Node::getLocalTransform() const {
    return local_transform_;
}

///
/// \param localTransform
void Node::setLocalTransform(const glm::mat4 &localTransform) {
    local_transform_ = localTransform;
    for (auto child : children_) {
        child->setWorldTransform(world_transform_ * localTransform);
    }
}

///
/// \return
const glm::mat4 &Node::getWorldTransform() const {
    return world_transform_;
}

///
/// \param worldTransform
void Node::setWorldTransform(const glm::mat4 &worldTransform) {
    world_transform_ = worldTransform;
    for (auto child : children_) {
        child->setWorldTransform(worldTransform * local_transform_);
    }
}

///
/// \param child
void Node::addChild(std::shared_ptr<Node> child) {
    (*child).depth_ = getDepth() + 1;
    child->setWorldTransform(world_transform_ * local_transform_);
    children_.push_back(child);
}

///
/// \param child_name
/// \return
std::shared_ptr<Node> Node::removeChild(const std::string &child_name) {
    auto found_child = std::remove_if(children_.begin(), children_.end(), [child_name] (std::shared_ptr<Node> const& child) {return child_name == (*child).name_;});
    return *found_child;
}

///
/// \param os
/// \param node
/// \return
std::ostream& operator<<(std::ostream &os, const Node &node) {
    os << "Parent:" << node.parent_ << std::endl;
    os << "Name: " << node.name_ << std::endl;
    os << "Path in tree: " << node.path_ << std::endl;
    os << "Depth in tree: " << node.depth_ << std::endl;
    os << "Names of direct children: \n";
    for (auto child_node : node.children_) {
        os << (*child_node).name_ << std::endl;
    }
}

///
/// \param m_shaders
/// \param m_view_transform
void Node::renderNode(std::map<std::string, shader_program> const& m_shaders, glm::mat4 const& m_view_transform) {
    glm::mat4 rotation_test = glm::rotate(glm::fmat4{}, glm::radians(0.01f), glm::fvec3{0.0f, 1.0f, 0.0f});
    setLocalTransform(rotation_test * getLocalTransform());

    //glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, 1.0f, 0.0f}) * local_transform_;
    // model_matrix = glm::translate(model_matrix, glm::fvec3{0.0f, 0.0f, -3.0f});

    for (auto const& child : children_) {
        child->renderNode(m_shaders, m_view_transform);
    }
}

///
/// \param translation
void Node::translate(glm::vec3 const& translation){
    local_transform_ = glm::translate(local_transform_, translation);
}

///
/// \param angle
/// \param axis
void Node::rotate(float angle, glm::vec3 const& axis){
    local_transform_ = glm::rotate(local_transform_,angle,axis);
}

///
/// \param scale
void Node::scale(float scale){
    local_transform_=glm::scale(local_transform_, glm::vec3{scale,scale,scale});
}

/*void Node::applyFunction(const std::function<void(Node)> &functionObject) {
    for (auto child : children_) {
        functionObject((*child));
        (*child).applyFunction(functionObject);
    }
}*/

Node::~Node() = default;
#pragma endregion




