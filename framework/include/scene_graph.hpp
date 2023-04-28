#ifndef OPENGL_FRAMEWORK_SCENE_GRAPH_HPP
#define OPENGL_FRAMEWORK_SCENE_GRAPH_HPP

#include <iostream>
#include "node.hpp"

class SceneGraph {
private:
    std::string name_;
    Node root_;

public:
    SceneGraph() = default;

    const std::string &getName() const;
    void setName(const std::string &name);

    const Node &getRoot() const;
    void setRoot(const Node &root);

    friend std::ostream &operator<<(std::ostream &os, const SceneGraph &graph);

    // void printGraph(bool show_transformation = false);
};

SceneGraph setupSolarSystem();

#endif //OPENGL_FRAMEWORK_SCENE_GRAPH_HPP
