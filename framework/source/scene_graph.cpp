#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
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
    os << "name_: " << graph.name_ << " root_: " << graph.root_ << std::endl;
    os << "children: \n";
    for (auto child : graph.getRoot()->getChildren()) {
        os << "\t" << *child << std::endl;
    }
    return os;
}


void SceneGraph::applyFunction(VoidFunctionObject const& functionObject) const {
    functionObject(root_);

}

SceneGraph::~SceneGraph() = default;


SceneGraph setupSolarSystem() {
    SceneGraph sceneGraph{};

    std::shared_ptr<Node> root = std::make_shared<Node>(Node{nullptr, "root"});
    sceneGraph.setRoot(root);

    std::shared_ptr<CameraNode> camera = std::make_shared<CameraNode>(root,"camera");
    root->addChild(camera);

    for (auto const& planet_name : PLANET_NAMES) {
        auto planet_node = std::make_shared<Node>(root, planet_name + "-Holder");
        auto geometry_node = std::make_shared<GeometryNode>(planet_node, planet_name + "-Geometry");
        planet_node->addChild(geometry_node);
        //planet_node->setLocalTransform(glm::translate(glm::mat4(1), glm::f32vec3(0.0f, 0.0f, -1.0)));
        root->addChild(planet_node);
    }

    std::shared_ptr<Node> earth_node = root->getChild("Earth-Holder");
    // earth_node->setLocalTransform(glm::translate(glm::mat4(1), glm::f32vec3(0.0f, 0.0f, -1.0)));
    std::shared_ptr<Node> moon_node = std::make_shared<Node>(earth_node,"Moon-Holder");
    std::shared_ptr<GeometryNode> moon_geometry = std::make_shared<GeometryNode>(moon_node, "Moon-Geometry");
    moon_node->addChild(moon_geometry);
    earth_node->addChild(moon_node);

    // std::cout << sceneGraph <<std::endl;

    return sceneGraph;
}
