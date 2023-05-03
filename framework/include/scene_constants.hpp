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

std::vector<std::string> PLANET_GEOMETRY_NAMES {
        "Mercury-Geometry",
        "Venus-Geometry",
        "Earth-Geometry",
        "Mars-Geometry",
        "Jupiter-Geometry",
        "Saturn-Geometry",
        "Uranus-Geometry",
        "Neptune-Geometry",
        "Pluto-Geometry"
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



#pragma endregion

#pragma region Moons
std::vector<std::string> MOON_NAMES {
        "Moon-Holder"
};

std::vector<std::string> MOON_GEOMETRY_NAMES {
        "Moon-Geometry"
};

std::vector<model> MOON_GEOMETRY {

};

#pragma endregion

#pragma region Sun
std::vector<std::string> SUN_NAMES {
        "Sun-Holder"
};

std::vector<std::string> SUN_GEOMETRY_NAMES {
        "Sun-Geometry"
};

std::vector<model> SUN_GEOMETRY {

};

#pragma endregion


#endif //OPENGL_FRAMEWORK_SCENE_CONSTANTS_HPP
