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
#include <utility>

std::vector<std::pair<std::string, float>> PLANET_REVOLUTION {
        std::make_pair("Mercury-Holder", 0.04f),
        std::make_pair("Venus-Holder", 0.03f),
        std::make_pair("Earth-Holder", 0.01f),
        std::make_pair("Mars-Holder", 0.009f),
        std::make_pair("Jupiter-Holder", 0.006f),
        std::make_pair("Saturn-Holder", 0.003f),
        std::make_pair("Uranus-Holder", 0.001f),
        std::make_pair("Neptune-Holder", 0.0009f),
        std::make_pair("Pluto-Holder", 0.0006f)
};


// Getters and setters, class utility
# pragma region

/// getter for parent node
/// \return shared_ptr parent node
const std::shared_ptr<Node> Node::getParent() const{
    return parent_;
};

/// setter for parent node
/// \param node
void Node::setParent(std::shared_ptr<Node> node){
    parent_ = std::move(node);
};

/// getter for children vector
/// \return vector children
const std::vector<std::shared_ptr<Node>> &Node::getChildren() const {
    return children_;
}

/// getter for one specific child
/// \param name of specific child
/// \return Node child
const std::shared_ptr<Node> Node::getChild(const std::string &name) const {
    auto foundChild = std::find_if(children_.begin(), children_.end(), [name] (std::shared_ptr<Node> const& child) {return name == (*child).name_;});
    return *foundChild;
}

/// getter for node name
/// \return string name of node
const std::string &Node::getName() const {
    return name_;
}

/// setter for node name
/// \param name
void Node::setName(const std::string &name) {
    name_ = name;
}

/// setter for children vector
/// \param children
void Node::setChildren(const std::vector<std::shared_ptr<Node>> &children) {
    children_ = children;
}

/// getter for path
/// \return string path
const std::string &Node::getPath() const {
    return path_;
}

/// setter for path
/// \param path
void Node::setPath(const std::string &path) {
    path_ = path;
}

/// getter for depth
/// \return integer depth
int Node::getDepth() const {
    return depth_;
}

/// setter for depth
/// \param depth
void Node::setDepth(int depth) {
    depth_ = depth;
}

/// getter for LocalTransformation
/// \return mat4 local transformation
const glm::mat4 &Node::getLocalTransform() const {
    return local_transform_;
}

/// setter for LocalTransformation
/// \param localTransform
void Node::setLocalTransform(const glm::mat4 &localTransform) {
    local_transform_ = localTransform;
    for (auto child : children_) {
        //child->setWorldTransform(world_transform_ * child->getLocalTransform()); // local transform of child, not current node
        child->setWorldTransform(world_transform_ * localTransform);
    }
}

/// getter for WorldTransformation
/// \return worldTransformation
const glm::mat4 &Node::getWorldTransform() const {
    return world_transform_;
}

/// setter for woldTransformation
/// \param worldTransform
void Node::setWorldTransform(const glm::mat4 &worldTransform) {
    world_transform_ = worldTransform;
    //setting also the worldTransformation for the children of the node
    for (auto child : children_) {
        child->setWorldTransform(worldTransform * child->local_transform_);
    }
}

/// add child to children vector
/// \param child
void Node::addChild(std::shared_ptr<Node> child) {
    (*child).depth_ = getDepth() + 1;
    child->setWorldTransform(world_transform_ * local_transform_);
    children_.push_back(child);
}

/// remove specific child form children vector
/// \param child_name
/// \return Node deleted child
std::shared_ptr<Node> Node::removeChild(const std::string &child_name) {
    auto found_child = std::remove_if(children_.begin(), children_.end(), [child_name] (std::shared_ptr<Node> const& child) {return child_name == (*child).name_;});
    return *found_child;
}

/// print out node
/// \param os
/// \param node
/// \return ostream / cli output
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

/// render node
/// \param m_shaders
/// \param m_view_transform
void Node::renderNode(std::map<std::string, shader_program> const& m_shaders, glm::mat4 const& m_view_transform) {
    //get value to compute speed of revolution around the sun
    float revolution_value = std::find_if(PLANET_REVOLUTION.begin(), PLANET_REVOLUTION.end(), [&] (std::pair<std::string, float> const& pair) {return name_ == pair.first; })->second;
    //compute rotation value
    glm::mat4 rotation_mat = glm::rotate(glm::fmat4{}, glm::radians(revolution_value), glm::fvec3{0.0f, 1.0f, 0.0f});
    glm::mat4 moon_rotation = glm::rotate(glm::fmat4{}, glm::radians(0.01f), glm::fvec3{0.0f, 1.0f, 0.0f});
    glm::mat4 enterprise_rotation = glm::rotate(glm::fmat4{}, glm::radians(-0.01f), glm::fvec3{0.0f, 1.0f, 0.0f});
    //set local Transformation Matrix
    setLocalTransform(rotation_mat * getLocalTransform());
    // set local transform of moon separately so it spins around earth
    if (name_ == "Moon-Holder") {
        setLocalTransform(moon_rotation * getLocalTransform());
    }

    if (name_ == "Enterprise-Holder") {
        setLocalTransform(moon_rotation * getLocalTransform());
    }

    //call function also for children of node
    for (auto const& child : children_) {
        child->renderNode(m_shaders, m_view_transform);
    }
}

/// translate Node
/// \param translation
void Node::translate(glm::vec3 const& translation){
    local_transform_ = glm::translate(local_transform_, translation);
}

/// rotate Node
/// \param angle
/// \param axis
void Node::rotate(float angle){
    glm::mat4 rotation_matrix = glm::rotate(glm::fmat4{}, angle, glm::fvec3{0.0f, 1.0f, 0.0f});
    setLocalTransform(rotation_matrix * local_transform_);
}

/// scale Node
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

//free allocated memory
Node::~Node() = default;
#pragma endregion




