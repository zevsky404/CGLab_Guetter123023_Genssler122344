#ifndef OPENGL_FRAMEWORK_NODE_HPP
#define OPENGL_FRAMEWORK_NODE_HPP

#include <utility>
#include <vector>
#include <iostream>
#include <functional>
#include <glm/glm.hpp>

class Node {

private:
    // Node parent_;
    std::vector<Node> children_;
    std::string name_;
    std::string path_;
    int depth_ = 0;
    glm::mat4 local_transform_;
    glm::mat4 world_transform_;

public:
    Node() = default;

    explicit Node(std::string name):
    children_{},
    name_{std::move(name)},
    path_{},
    depth_{},
    local_transform_{},
    world_transform_{}
    {};

    const std::vector<Node>& getChildren() const;
    void setChildren(const std::vector<Node> &children);

    const Node getChild(std::string const& name) const;

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

    void addChild(Node& child);
    Node removeChild(std::string const& child_name);

    friend std::ostream& operator<<(std::ostream& os, Node const& node);

    // void renderNode() const;

    // void renderChildren() const;

    void applyFunction(std::function<void(Node)> const& functionObject);
};

typedef std::function<void(Node)> VoidFunctionObject;

#endif //OPENGL_FRAMEWORK_NODE_HPP
