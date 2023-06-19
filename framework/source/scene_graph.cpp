#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "scene_graph.hpp"
#include "node.hpp"
#include "camera_node.hpp"
#include "geometry_node.hpp"
#include "scene_constants.hpp"
#include "point_light_node.hpp"
#include "texture_loader.hpp"


/// get name of the scene
/// \return name of scene
const std::string &SceneGraph::getName() const {
    return name_;
}

/// set name of scene
/// \param name
void SceneGraph::setName(const std::string &name) {
    name_ = name;
}

/// get root of scene
/// \return shared_ptr root
const std::shared_ptr<Node> &SceneGraph::getRoot() const {
    return root_;
}

/// set root of scene
/// \param root
void SceneGraph::setRoot(const std::shared_ptr<Node> &root) {
    root_ = root;
}

/// print scene
/// \param os
/// \param graph
/// \return cli output
std::ostream &operator<<(std::ostream &os, const SceneGraph &graph) {
    os << "name_: " << graph.name_ << " root_: " << graph.root_ << std::endl;
    os << "children: \n";
    for (auto child : graph.getRoot()->getChildren()) {
        os << "\t" << *child << std::endl;
    }
    return os;
}

SceneGraph::~SceneGraph() = default;

texture_object setupTexture(const std::string &textureFileName) {
    pixel_data pixelData = texture_loader::file(textureFileName);

    texture_object textureObject{};
    glGenTextures(1, &textureObject.handle);
    glBindTexture(GL_TEXTURE_2D, textureObject.handle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int)pixelData.width, (int)pixelData.height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData.ptr());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);



    glBindTexture(GL_TEXTURE_2D, 0);
    //stbi_image_free(pixelData);
    return textureObject;
}

/// setup the scene graph of the solar system
/// \param planet_model
/// \return scene graph
SceneGraph setupSolarSystem(std::map<std::string, model_object> const& model_objects, std::string const& resource_path) {
    //initialize empty scene graph
    SceneGraph sceneGraph{};
    std::string texturePath = resource_path + "textures/";

    //initialize root
    std::shared_ptr<Node> root = std::make_shared<Node>(Node{nullptr, "root"});
    //set root of scene graph
    sceneGraph.setRoot(root);

    //initialize sun as a point light
    auto sun_light_node = std::make_shared<PointLightNode>(root,"Planet-Sun-Holder",glm::vec3{1.0f,1.0f,1.0f},4.0f);
    //initialize geometry node for sun
    auto sun_geometry_node = std::make_shared<GeometryNode>(sun_light_node,"Planet-Sun-Geometry",
                                                            model_objects.at("planet-object"), SUN_COLOR);
    //add geometry node as child to sun node
    sun_light_node->addChild(sun_geometry_node);
    //add sun node as child to root
    root->addChild(sun_light_node);

    auto star_geometry = std::make_shared<GeometryNode>(root, "Star-Geometry", model_objects.at("stars-object"));
    root->addChild(star_geometry);

    /*std::shared_ptr<CameraNode> camera = std::make_shared<CameraNode>(root,"camera");
    root->addChild(camera);*/
    // use local transform of camera, not view transform

    //for all planets do
    for (size_t i = 0; i <= PLANET_NAMES.size() - 1; ++i) {
        //initialize orbit as geometry node
        auto orbit_geometry = std::make_shared<GeometryNode>(sun_light_node, "Orbit",
                                                             model_objects.at("orbit-object"));
        sun_light_node->addChild(orbit_geometry);
        orbit_geometry->scale(PLANET_DISTANCES[i]);
        //initialize planet as a node
        auto planet_node = std::make_shared<Node>(sun_light_node, "Planet-" + PLANET_NAMES[i] + "-Holder", PLANET_COLOR[i]);
        //initialize geometry node for said planet
        auto geometry_node = std::make_shared<GeometryNode>(planet_node, "Planet-" + PLANET_NAMES[i] + "-Geometry",
                                                            model_objects.at("planet-object"), PLANET_COLOR[i]);

        geometry_node->setTexture(setupTexture(texturePath + "2k_mercury.jpg"));
        
        //add geometry node as a child to planet node
        planet_node->addChild(geometry_node);
        //add planet as a child to sun node
        sun_light_node->addChild(planet_node);
        //translate planet that it has a distance to the sun
        planet_node->translate(glm::vec3{0.0f, 0.0f, PLANET_DISTANCES[i]});
        //change size of planet
        planet_node->scale(PLANET_SIZES[i]);
    }

    //_______MOON SETUP________

    //get node of earth
    std::shared_ptr<Node> earth_node = sun_light_node->getChild("Planet-Earth-Holder");
    auto orbit_geometry_moon = std::make_shared<GeometryNode>(earth_node, "Orbit", model_objects.at("orbit-object"));
    earth_node->addChild(orbit_geometry_moon);
    orbit_geometry_moon->scale(2.0);
    //initialize moon node
    std::shared_ptr<Node> moon_node = std::make_shared<Node>(earth_node,"Planet-Moon-Holder");
    //initialize moon geometry node
    std::shared_ptr<GeometryNode> moon_geometry = std::make_shared<GeometryNode>(moon_node, "Planet-Moon-Geometry", model_objects.at("planet-object"));
    moon_geometry->setTexture(setupTexture(texturePath + "2k_moon.jpg"));

    //moon_node->translate(glm::vec3{0.0f,0.0f,-2.0f});
    //add geometry node as child to moon node
    moon_node->addChild(moon_geometry);
    //add moon node as child to earth node
    earth_node->addChild(moon_node);

    //translate moon so it has a distance to the earth
    moon_node->translate(glm::vec3{0.0, 0.0, 2.0});
    //moon_node->setLocalTransform(glm::translate(glm::mat4{}, glm::vec3{0.0, 0.0, -2.0}));
    //scale moon
    moon_geometry->scale(0.5f);

    //_______ENTERPRISE SETUP__________

    //get jupiter node
    std::shared_ptr<Node> jupiter_node = sun_light_node->getChild("Planet-Jupiter-Holder");
    auto orbit_geometry_jupiter = std::make_shared<GeometryNode>(jupiter_node, "Orbit", model_objects.at("orbit-object"));
    jupiter_node->addChild(orbit_geometry_jupiter);
    orbit_geometry_jupiter->scale(2.0);
    //initialise enterprise node
    std::shared_ptr<Node> enterprise_node = std::make_shared<Node>(earth_node,"Enterprise-Holder");
    // initialise enterprise geometry node
    std::shared_ptr<GeometryNode> enterprise_geometry = std::make_shared<GeometryNode>(enterprise_node, "Enterprise-Geometry", model_objects.at("enterprise-object"));

    enterprise_node->addChild(enterprise_geometry);
    jupiter_node->addChild(enterprise_node);
    //enterprise_node->setLocalTransform(glm::translate(glm::mat4{}, glm::vec3{0.0f, 0.0f, -2.0f}));
    enterprise_node->translate(glm::vec3{0.0f, 0.0f, -2.0f});
    enterprise_geometry->rotate(glm::radians(-90.0f));
    enterprise_geometry->scale(0.6f);

    return sceneGraph;
}
