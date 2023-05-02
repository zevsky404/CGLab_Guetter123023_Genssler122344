#include <string>
#include <vector>
#include "scene_graph.hpp"
#include "node.hpp"
#include "camera_node.hpp"
#include "geometry_node.hpp"
#include "scene_constants.hpp"

const std::__cxx11::basic_string<char> &SceneGraph::getName() const {
    return name_;
}

void SceneGraph::setName(const std::__cxx11::basic_string<char> &name) {
    name_ = name;
}

const Node &SceneGraph::getRoot() const {
    return root_;
}

void SceneGraph::setRoot(const Node &root) {
    root_ = root;
}

std::ostream &operator<<(std::ostream &os, const SceneGraph &graph) {
    os << "name_: " << graph.name_ << " root_: " << graph.root_;
    return os;
}


void SceneGraph::applyFunction(VoidFunctionObject const& functionObject) {
    functionObject(root_);

}


SceneGraph setupSolarSystem() {
    SceneGraph sceneGraph{};

    Node root = Node("root");
    sceneGraph.setRoot(root);

    CameraNode camera = CameraNode("camera");
    root.addChild(camera);

    for (auto const& planet_name : PLANET_NAMES) {
        auto planet_node = Node(planet_name);
        root.addChild(planet_node);
    }

    Node earth_node = root.getChild("Earth");
    Node moon_node = Node("Moon");
    earth_node.addChild(moon_node);

    return sceneGraph;
}
