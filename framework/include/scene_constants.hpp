#ifndef OPENGL_FRAMEWORK_SCENE_CONSTANTS_HPP
#define OPENGL_FRAMEWORK_SCENE_CONSTANTS_HPP

#include "model.hpp"
#include <ostream>

#pragma region Planets
std::vector<std::string> PLANET_NAMES {
        "Mercury-Holder",
        "Venus-Holder",
        "Earth-Holder",
        "Mars-Holder",
        "Jupiter-Holder",
        "Saturn-Holder",
        "Uranus-Holder",
        "Neptune-Holder",
        "Pluto-Holder"
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

std::vector<model> PLANET_GEOMETRY {


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

std::vector<float> PLANET_REVOLUTION {
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
