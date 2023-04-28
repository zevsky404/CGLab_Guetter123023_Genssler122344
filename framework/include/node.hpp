#ifndef OPENGL_FRAMEWORK_NODE_HPP
#define OPENGL_FRAMEWORK_NODE_HPP

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Node {
private:
    // Node parent_;
    std::vector<Node> children_;
    std::string name_;
    std::string path_;
    int depth_;
    glm::mat4 local_transform_;
    glm::mat4 world_transform_;

public:
    const std::vector<Node>& getChildren() const;
    void setChildren(const std::vector<Node> &children);

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

    void addChildren(Node const& child);
    Node removeChildren(std::string const& child_name);

    friend std::ostream& operator<<(std::ostream& os, Node const& node);
};

#endif //OPENGL_FRAMEWORK_NODE_HPP
