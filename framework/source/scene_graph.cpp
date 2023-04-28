#include <string>
#include <vector>
#include "scene_graph.hpp"
#include "node.hpp"
#include "camera_node.hpp"
#include "geometry_node.hpp"

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

/*void Node::printGraph(bool show_transformations) {
    std::cout << *this << std::endl;
    if (show_transformations) {
        std::cout << local_transform_
    }
}*/

std::vector<std::string> PLANETS {
    "Mercury",
    "Venus",
    "Earth",
    "Mars",
    "Jupiter",
    "Saturn",
    "Uranus",
    "Neptune",
    "Pluto"
};

SceneGraph setupSolarSystem() {
    SceneGraph sceneGraph{};

    Node root = Node("root");
    sceneGraph.setRoot(root);

    CameraNode camera = CameraNode("camera");
    root.addChild(camera);

    for (auto const& planet_name : PLANETS) {
        auto planet_node = Node(planet_name);
        root.addChild(planet_node);
    }

    Node earth_node = root.getChild("Earth");
    earth_node.addChild(Node("Moon"));

    return sceneGraph;
}
