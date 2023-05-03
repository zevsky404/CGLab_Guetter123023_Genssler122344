#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "scene_graph.hpp"
#include "node.hpp"
#include "camera_node.hpp"
#include "geometry_node.hpp"
#include "scene_constants.hpp"
#include "point_light_node.hpp"

///
/// \return
const std::string &SceneGraph::getName() const {
    return name_;
}

///
/// \param name
void SceneGraph::setName(const std::string &name) {
    name_ = name;
}

///
/// \return
const std::shared_ptr<Node> &SceneGraph::getRoot() const {
    return root_;
}

///
/// \param root
void SceneGraph::setRoot(const std::shared_ptr<Node> &root) {
    root_ = root;
}

///
/// \param os
/// \param graph
/// \return
std::ostream &operator<<(std::ostream &os, const SceneGraph &graph) {
    os << "name_: " << graph.name_ << " root_: " << graph.root_ << std::endl;
    os << "children: \n";
    for (auto child : graph.getRoot()->getChildren()) {
        os << "\t" << *child << std::endl;
    }
    return os;
}


/*void SceneGraph::applyFunction(VoidFunctionObject const& functionObject) const {
    functionObject(root_);

}*/

SceneGraph::~SceneGraph() = default;

///
/// \param planet_model
/// \return
SceneGraph setupSolarSystem(model_object const& planet_model) {
    SceneGraph sceneGraph{};

    std::shared_ptr<Node> root = std::make_shared<Node>(Node{nullptr, "root"});
    sceneGraph.setRoot(root);

    //auto sun_light_node = std::make_shared<PointLightNode>(root,"Sun",glm::vec3{0.0f,0.0f,0.0f},1.0f);
    auto sun_light_node = std::make_shared<Node>(root,"Sun");
    auto sun_geometry_node = std::make_shared<GeometryNode>(sun_light_node,"Sun-Geometry",planet_model);
    sun_light_node->addChild(sun_geometry_node);
    root->addChild(sun_light_node);

    /*std::shared_ptr<CameraNode> camera = std::make_shared<CameraNode>(root,"camera");
    root->addChild(camera);*/

    for (auto const& planet_name : PLANET_NAMES) {
        auto planet_node = std::make_shared<Node>(root, planet_name + "-Holder");
        auto geometry_node = std::make_shared<GeometryNode>(planet_node, planet_name + "-Geometry", planet_model);
        planet_node->addChild(geometry_node);
        //planet_node->setLocalTransform(glm::translate(glm::mat4(1), glm::f32vec3(0.0f, 0.0f, -1.0)));
        root->addChild(planet_node);
        geometry_node->scale(0.5f);
    }

    std::shared_ptr<Node> earth_node = root->getChild("Earth-Holder");
    std::shared_ptr<Node> moon_node = std::make_shared<Node>(earth_node,"Moon-Holder");
    earth_node->setLocalTransform(glm::translate(glm::mat4{}, glm::vec3{0.0, 0.0, -2.0}));
    std::shared_ptr<GeometryNode> moon_geometry = std::make_shared<GeometryNode>(moon_node, "Moon-Geometry", planet_model);
    moon_node->setLocalTransform(glm::translate(glm::mat4{}, glm::vec3{0.0, 0.0, -2.0}));
    moon_node->addChild(moon_geometry);
    earth_node->addChild(moon_node);
    moon_geometry->scale(0.5f);

    return sceneGraph;
}
