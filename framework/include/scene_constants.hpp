#ifndef OPENGL_FRAMEWORK_SCENE_CONSTANTS_HPP
#define OPENGL_FRAMEWORK_SCENE_CONSTANTS_HPP

#include "model.hpp"
#include <ostream>



#pragma region Planets
std::vector<std::string> PLANET_NAMES {
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

std::vector<float> PLANET_SIZES {
    0.2f,
    0.4f,
    0.5f,
    0.3f,
    0.8f,
    0.7f,
    0.4f,
    0.4f,
    0.1f
};

std::vector<float> PLANET_DISTANCES {
    2.0f,
    4.0f,
    6.0f,
    8.0f,
    10.0f,
    12.0f,
    14.0f,
    16.0f,
    18.0f
};

std::vector<float> PLANET_ROTATION {
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0,
        1.0

};

std::vector<glm::vec3> PLANET_COLOR {
        glm::vec3(87,103,180),
        glm::vec3(173,141,84),
        glm::vec3(156,87,0),
        glm::vec3(193,68,14),
        glm::vec3(148,105,86),
        glm::vec3(244,165,135),
        glm::vec3(98,174,231),
        glm::vec3(43,55,139),
        glm::vec3(150,133,112)
};

std::vector<std::string> PLANET_TEXTURE {
        "2k_mercury.jpg",
        "2k_venus_surface.jpg",
        "2k_earth_nightmap.jpg",
        "2k_mars.jpg",
        "2k_jupiter.jpg",
        "2k_saturn.jpg",
        "2k_uranus.jpg",
        "2k_neptune.jpg",
        "2k_pluto_1.jpg"
};

#pragma endregion



#pragma region Moons

std::string MOON_TEXTURE = "2k_moon.jpg";

#pragma endregion

#pragma region Sun

glm::vec3 SUN_COLOR = glm::vec3(255,249,0);
//glm::vec3 SUN_COLOR = glm::vec3(255, 255, 255);

std::string SUN_TEXTURE = "2k_sun.jpg";

#pragma endregion


#pragma region Skybox

std::vector<std::string> SKYBOX_FACES = {
        "right.png",
        "left.png",
        "top.png",
        "bottom.png",
        "front.png",
        "back.png"
};

std::vector<std::string> SKYBOX_VARIANTS = {
        "skyboxes/skybox/",
        "skyboxes/skybox_blue_orange/",
        "skyboxes/skybox_lilac_grey/",
        "skyboxes/skybox_lilac_orange/"
};

#pragma endregion


#endif //OPENGL_FRAMEWORK_SCENE_CONSTANTS_HPP
