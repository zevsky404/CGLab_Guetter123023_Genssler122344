#ifndef OPENGL_FRAMEWORK_SCENE_GRAPH_HPP
#define OPENGL_FRAMEWORK_SCENE_GRAPH_HPP

#include <iostream>
#include "node.hpp"

class SceneGraph {
private:
    //
    std::string name_;
    //
    std::shared_ptr<Node> root_;

public:
    //
    SceneGraph() = default;

    //
    const std::string &getName() const;
    //
    void setName(const std::string &name);

    //
    const std::shared_ptr<Node> &getRoot() const;
    //
    void setRoot(const std::shared_ptr<Node> &root);

    //
    friend std::ostream &operator<<(std::ostream &os, const SceneGraph &graph);

    // void printGraph(bool show_transformation = false);

    //void applyFunction(VoidFunctionObject const& functionObject) const;

    //
    ~SceneGraph();
};

//
SceneGraph setupSolarSystem(model_object const& planet_model);

#endif //OPENGL_FRAMEWORK_SCENE_GRAPH_HPP
