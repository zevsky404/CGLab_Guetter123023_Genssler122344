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
    std::shared_ptr<Node> parent_;
    std::vector<std::shared_ptr<Node>> children_;
    std::string name_;
    std::string path_;
    int depth_ = 0;
    glm::mat4 local_transform_;
    glm::mat4 world_transform_;

public:
    Node() = default;

    explicit Node(std::shared_ptr<Node> parent, std::string const& name):
    parent_{std::move(parent)},
    children_{},
    name_{std::move(name)},
    path_{},
    depth_{},
    local_transform_{},
    world_transform_{}
    {};

    const std::shared_ptr<Node> getParent() const;
    void setParent(std::shared_ptr<Node> node);

    const std::vector<std::shared_ptr<Node>>& getChildren() const;
    void setChildren(const std::vector<std::shared_ptr<Node>> &children);

    const std::shared_ptr<Node> getChild(std::string const& name) const;

    const std::string& getName() const;
    void setName(const std::string &name);

    const std::string &getPath() const;
    void setPath(const std::string &path);

    int getDepth() const;
    void setDepth(int depth);

    const glm::mat4 &getLocalTransform() const;
    void setLocalTransform(const glm::mat4 &localTransform);

    const glm::mat4 &getWorldTransform() const;
    void setWorldTransform(const glm::mat4 &worldTransform);

    void addChild(std::shared_ptr<Node> node);
    std::shared_ptr<Node> removeChild(std::string const& child_name);

    friend std::ostream& operator<<(std::ostream& os, Node const& node);

    virtual void renderNode(std::map<std::string, shader_program> const& m_shaders, glm::mat4 const& m_view_transform);

    // void renderChildren() const;

    void applyFunction(std::function<void(Node)> const& functionObject);
    virtual ~Node();
};

typedef std::function<void(std::shared_ptr<Node>)> VoidFunctionObject;

#endif //OPENGL_FRAMEWORK_NODE_HPP
