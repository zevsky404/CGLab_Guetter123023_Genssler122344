#include <string>
#include <vector>
#include "scene_graph.hpp"
#include "node.hpp"
#include "camera_node.hpp"
#include "geometry_node.hpp"
#include "scene_constants.hpp"

const std::string &SceneGraph::getName() const {
    return name_;
}

void SceneGraph::setName(const std::string &name) {
    name_ = name;
}

const std::shared_ptr<Node> &SceneGraph::getRoot() const {
    return root_;
}

void SceneGraph::setRoot(const std::shared_ptr<Node> &root) {
    root_ = root;
}

std::ostream &operator<<(std::ostream &os, const SceneGraph &graph) {
    os << "name_: " << graph.name_ << " root_: " << graph.root_;
    return os;
}


void SceneGraph::applyFunction(VoidFunctionObject const& functionObject) {
    functionObject(*root_);

}

SceneGraph::~SceneGraph() {};


SceneGraph setupSolarSystem() {
    SceneGraph sceneGraph{};

    std::shared_ptr<Node> root = std::make_shared<Node>(Node{nullptr, "root"});
    sceneGraph.setRoot(root);

    std::shared_ptr<Node> camera = std::make_shared<Node>(root,"camera");
    root->addChild(camera);

    for (auto const& planet_name : PLANET_NAMES) {
        auto planet_node = std::make_shared<GeometryNode>(root, planet_name, "models/sphere.obj");
        root->addChild(planet_node);
    }

    std::shared_ptr<Node> earth_node = root->getChild("Earth");
    std::shared_ptr<GeometryNode> moon_node = std::make_shared<GeometryNode>(earth_node,"Moon", "models/sphere.obj");
    earth_node->addChild(moon_node);

    return sceneGraph;
}
