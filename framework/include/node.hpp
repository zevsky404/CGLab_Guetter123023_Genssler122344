#ifndef OPENGL_FRAMEWORK_NODE_HPP
#define OPENGL_FRAMEWORK_NODE_HPP

#include <utility>
#include <vector>
#include <iostream>
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include "structs.hpp"

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node {

private:
    //parent node
    std::shared_ptr<Node> parent_;
    //container to store all children
    std::vector<std::shared_ptr<Node>> children_;
    //name of node
    std::string name_;
    //path of node
    std::string path_;
    //states how deep the node is in the hierarchy
    int depth_ = 0;
    glm::mat4 local_transform_;
    glm::mat4 world_transform_;

public:
    //default constructor
    Node() = default;

    explicit Node(std::shared_ptr<Node> parent, std::string name):
    parent_{std::move(parent)},
    children_{},
    name_{std::move(name)},
    path_{},
    depth_{},
    local_transform_{},
    world_transform_{}
    {};

    //get parent node
    const std::shared_ptr<Node> getParent() const;
    //set parent node
    void setParent(std::shared_ptr<Node> node);

    //get vector of children
    const std::vector<std::shared_ptr<Node>>& getChildren() const;
    //set vector of children
    void setChildren(const std::vector<std::shared_ptr<Node>> &children);

    //get one specific child from the vector
    const std::shared_ptr<Node> getChild(std::string const& name) const;

    //get name of the node
    const std::string& getName() const;
    //set name of the node
    void setName(const std::string &name);

    //get path of the node
    const std::string &getPath() const;
    //set path of the node
    void setPath(const std::string &path);

    //get depth of node
    int getDepth() const;
    //set depth of node
    void setDepth(int depth);

    //get local transformation Matrix
    const glm::mat4 &getLocalTransform() const;
    //set local transformation Matrix
    void setLocalTransform(const glm::mat4 &localTransform);

    //get world transformation Matrix
    const glm::mat4 &getWorldTransform() const;
    //set world transformation Matrix
    void setWorldTransform(const glm::mat4 &worldTransform);

    //add Child node to Children Vector
    void addChild(std::shared_ptr<Node> node);
    //remove specific Child from the Children Vector
    std::shared_ptr<Node> removeChild(std::string const& child_name);

    //print information of node on the console
    friend std::ostream& operator<<(std::ostream& os, Node const& node);

    //render function for Node
    virtual void renderNode(std::map<std::string, shader_program> const& m_shaders, glm::mat4 const& m_view_transform);

    void translate(glm::vec3 const& translation);

    void rotate(float angle, glm::vec3 const& axis);

    void scale(float scale);

    //void applyFunction(std::function<void(Node)> const& functionObject);

    //frees allocated memory
    virtual ~Node();
};

//typedef std::function<void(std::shared_ptr<Node>)> VoidFunctionObject;

#endif //OPENGL_FRAMEWORK_NODE_HPP
