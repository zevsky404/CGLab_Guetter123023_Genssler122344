// standard library
#include <iostream>

// framework
#include "application_solar.hpp"
#include "window_handler.hpp"
#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

// self-written classes
#include "scene_graph.hpp"
#include "node.hpp"
#include "point_light_node.hpp"
//#include "geometry_node.hpp"

// OpenGL
#include <glbinding/gl/gl.h>
// use gl definitions from glbinding
using namespace gl;


#pragma region CONSTANTS
    int STAR_COUNT = 1000;
    int LINE_SEGMENT_COUNT = 100;
#pragma endregion


ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,enterprise_object{}
 ,star_object{}
 ,orbit_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
 ,sceneGraph{}
{
    // setup all necessary geometries and shaders
    initializeGeometry();
    initializeShaderPrograms();

    // fill map with all necessary objects, so they can be accessed by name
    std::map<std::string ,model_object> model_objects {
        std::make_pair("planet-object", planet_object),
        std::make_pair("stars-object", star_object),
        std::make_pair("orbit-object", orbit_object),
        std::make_pair("enterprise-object", enterprise_object)
    };

    // create graph hierarchy
    sceneGraph = setupSolarSystem(model_objects);
}

ApplicationSolar::~ApplicationSolar() {
    glDeleteBuffers(1, &planet_object.vertex_BO);
    glDeleteBuffers(1, &planet_object.element_BO);
    glDeleteVertexArrays(1, &planet_object.vertex_AO);

    glDeleteBuffers(1, &enterprise_object.vertex_BO);
    glDeleteBuffers(1, &enterprise_object.element_BO);
    glDeleteVertexArrays(1, &enterprise_object.vertex_AO);

    glDeleteBuffers(1, &star_object.vertex_BO);
    glDeleteBuffers(1, &star_object.element_BO);
    glDeleteVertexArrays(1, &star_object.vertex_AO);

    glDeleteBuffers(1, &orbit_object.vertex_BO);
    glDeleteBuffers(1, &orbit_object.element_BO);
    glDeleteVertexArrays(1, &orbit_object.vertex_AO);

}

// renders the entire scene graph starting from the root
void ApplicationSolar::render() const {
    // things that cannot be handled in geometry node, as it requires information about scene are handled here
    std::shared_ptr<PointLightNode> sun_light = std::static_pointer_cast<PointLightNode>(sceneGraph.getRoot()->getChild("Planet-Sun-Holder"));

    glUseProgram(m_shaders.at("planet").handle);
    gl::glUniform3fv(m_shaders.at("planet").u_locs.at("LightColor"),
                     1, glm::value_ptr(sun_light->getLightColour()));

    gl::glUniform3fv(m_shaders.at("planet").u_locs.at("LightPosition"),
                    1, glm::value_ptr(sun_light->getWorldTransform()[3]));

    gl::glUniform1f(m_shaders.at("planet").u_locs.at("LightIntensity"),
                    sun_light->getLightIntensity());

    sceneGraph.getRoot()->renderNode(m_shaders, m_view_transform);
}

void ApplicationSolar::uploadView() {
    // vertices are transformed in camera space, so camera transform must be inverted
    glm::fmat4 view_matrix = glm::inverse(m_view_transform);

    // upload matrix to gpu
    glUseProgram(m_shaders.at("planet").handle);
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));

    glUseProgram(m_shaders.at("orbit").handle);
    glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ViewMatrix"),
                       1, GL_FALSE, glm::value_ptr(view_matrix));

    glUseProgram(m_shaders.at("stars").handle);
    glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ViewMatrix"), 1,
                       GL_FALSE, glm::value_ptr(view_matrix));

    glUseProgram(m_shaders.at("enterprise").handle);
    glUniformMatrix4fv(m_shaders.at("enterprise").u_locs.at("ViewMatrix"), 1,
                       GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {

    // bind shader to which to upload uniforms
    glUseProgram(m_shaders.at("planet").handle);
    // upload matrix to gpu
    glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));

    glUseProgram(m_shaders.at("orbit").handle);
    glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ProjectionMatrix"),
                       1, GL_FALSE, glm::value_ptr(m_view_projection));

    glUseProgram(m_shaders.at("stars").handle);
    glUniformMatrix4fv(m_shaders.at("stars").u_locs.at("ProjectionMatrix"),
                       1, GL_FALSE, glm::value_ptr(m_view_projection));

    glUseProgram(m_shaders.at("enterprise").handle);
    glUniformMatrix4fv(m_shaders.at("enterprise").u_locs.at("ProjectionMatrix"),
                       1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() {
    // upload uniform values to new locations
    uploadView();
    uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
    // store shader program objects in container
    m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
    // request uniform locations for shader program
    m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
    m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
    m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
    m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("planet").u_locs["PlanetColor"] = -1;
    m_shaders.at("planet").u_locs["AmbientColor"] = -1;
    m_shaders.at("planet").u_locs["LightIntensity"] = -1;
    m_shaders.at("planet").u_locs["LightPosition"] = -1;
    m_shaders.at("planet").u_locs["LightColor"] = -1;
    m_shaders.at("planet").u_locs["Cel"] = -1;
    m_shaders.at("planet").u_locs["CameraPosition"] = -1;

    m_shaders.emplace("orbit", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/orbit.vert"},
                                            {GL_FRAGMENT_SHADER, m_resource_path + "shaders/orbit.frag"}}});
    m_shaders.at("orbit").u_locs["ModelMatrix"] = -1;
    m_shaders.at("orbit").u_locs["ViewMatrix"] = -1;
    m_shaders.at("orbit").u_locs["ProjectionMatrix"] = -1;

    m_shaders.emplace("stars", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/vao.vert"},
                                             {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}}});

    m_shaders.at("stars").u_locs["ModelMatrix"] = -1;
    m_shaders.at("stars").u_locs["ViewMatrix"] = -1;
    m_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;

    m_shaders.emplace("enterprise", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/enterprise.vert"},
                                                {GL_FRAGMENT_SHADER, m_resource_path + "shaders/enterprise.frag"}}});
    m_shaders.at("enterprise").u_locs["NormalMatrix"] = -1;
    m_shaders.at("enterprise").u_locs["ModelMatrix"] = -1;
    m_shaders.at("enterprise").u_locs["ViewMatrix"] = -1;
    m_shaders.at("enterprise").u_locs["ProjectionMatrix"] = -1;
}

// initialise all geometries
void ApplicationSolar::initializeGeometry() {
    initializeStarGeometry();
    initializePlanetGeometry();
    initializeOrbitGeometry();
    initializeEnterpriseGeometry();
}
# pragma region GEOMETRY INIT
void ApplicationSolar::initializePlanetGeometry() {
    model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);

    // generate vertex array object
    glGenVertexArrays(1, &planet_object.vertex_AO);
    // bind the array for attaching buffers
    glBindVertexArray(planet_object.vertex_AO);

    // generate generic buffer
    glGenBuffers(1, &planet_object.vertex_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
    // configure currently bound array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    // first attribute is 3 floats with no offset & stride
    glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
    // activate second attribute on gpu
    glEnableVertexAttribArray(1);
    // second attribute is 3 floats with no offset & stride
    glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

    // generate generic buffer
    glGenBuffers(1, &planet_object.element_BO);
    // bind this as an vertex array buffer containing all attributes
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
    // configure currently bound array buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

    // store type of primitive to draw
    planet_object.draw_mode = GL_TRIANGLES;
    // transfer number of indices to model object
    planet_object.num_elements = GLsizei(planet_model.indices.size());
}

void ApplicationSolar::initializeEnterpriseGeometry() {
    // load model from file
    model enterprise_model = model_loader::obj(m_resource_path + "models/USS_Enterprise_NCC-1701_7.obj", model::NORMAL);

    glGenVertexArrays(1, &enterprise_object.vertex_AO);
    glBindVertexArray(enterprise_object.vertex_AO);

    glGenBuffers(1, &enterprise_object.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, enterprise_object.vertex_BO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * enterprise_model.data.size(), enterprise_model.data.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, enterprise_model.vertex_bytes, enterprise_model.offsets[model::POSITION]);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, enterprise_model.vertex_bytes, enterprise_model.offsets[model::NORMAL]);

    glGenBuffers(1, &enterprise_object.element_BO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, enterprise_object.element_BO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * enterprise_model.indices.size(), enterprise_model.indices.data(), GL_STATIC_DRAW);

    enterprise_object.draw_mode = GL_TRIANGLES;
    enterprise_object.num_elements = GLsizei(enterprise_model.indices.size());
}

// set up geometry for stars
void ApplicationSolar::initializeStarGeometry() {
    std::srand( (unsigned)time(nullptr) );
    std::vector<float> stars_vec;

    // for each star push random position and color values
    for (int i = 0; i < STAR_COUNT; ++i) {
        float x = (static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX/100)))-50;
        float y = (static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX/100)))-50;
        float z = (static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX/100)))-50;
        float r = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
        float g = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);
        float b = static_cast <float> (std::rand()) / static_cast <float> (RAND_MAX);

        for (float const number : {x, y, z, r, g, b}) {
            stars_vec.push_back(number);
        }
    }

    // enable and bind vertex array
    glGenVertexArrays(1, &star_object.vertex_AO);
    glBindVertexArray(star_object.vertex_AO);

    // generate generic buffer
    glGenBuffers(1, &star_object.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);

    // fill buffer with information about stars (each star is made up of 6 floats: {x, y, z}, {r, g, b}
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * STAR_COUNT * 6,
                 stars_vec.data(), GL_STATIC_DRAW);
    // enable attribute 1
    glEnableVertexAttribArray(0);
    // first attribute position {x, y, z}
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, nullptr);

    // enable attribute 2
    glEnableVertexAttribArray(1);
    // second attribute colour {r, g, b}
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(float) * 6, (void*)(sizeof(float) * 3));
    // set the draw_mode to GL_POINTS, so that each star with the given attributes is drawn as a single point
    star_object.draw_mode = GL_POINTS;
    // specify the amount of stars drawn
    star_object.num_elements = GLsizei(STAR_COUNT);
}

// set up geometry for orbit
void ApplicationSolar::initializeOrbitGeometry() {

    std::vector<float> segment_points;
    for (int i = 0; i < LINE_SEGMENT_COUNT; ++i) {
        float angle = 2 * (float)M_PI * (float)i / LINE_SEGMENT_COUNT;
        segment_points.push_back((float)sin(angle));
        segment_points.push_back(0);
        segment_points.push_back((float)cos(angle));
    }

    glGenVertexArrays(1, &orbit_object.vertex_AO);
    glBindVertexArray(orbit_object.vertex_AO);

    glGenBuffers(1, &orbit_object.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);

    glBufferData(GL_ARRAY_BUFFER, GLsizei(segment_points.size() * sizeof(float)),
                 segment_points.data(), GL_STATIC_DRAW);

    // activate first attribute on gpu
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    glGenBuffers(1, &orbit_object.element_BO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbit_object.element_BO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 GLsizei(sizeof(float) * segment_points.size()), segment_points.data(),
                 GL_STATIC_DRAW);

    // set draw mode to GL_LINE_LOOP so lines are drawn between points and finally connect together again to a circle GLsizei(segment_points.size() / 3);
    orbit_object.draw_mode = GL_LINE_LOOP;
    orbit_object.num_elements = LINE_SEGMENT_COUNT;
}

#pragma endregion

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
    //moving forward
    if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.07f});
    uploadView();
    }
    //moving backwards
    else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.07f});
    uploadView();
    }
    //moving down
    else if (key == GLFW_KEY_C  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, -0.07f, 0.0f});
      uploadView();
    }
    //moving up
    else if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.07f, 0.0f});
      uploadView();
    }
    //moving left
    else if (key == GLFW_KEY_A  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      m_view_transform = glm::translate(m_view_transform, glm::fvec3{-0.07f, 0.0f, 0.0f});
      uploadView();
    }
    //moving right
    else if (key == GLFW_KEY_D  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
      m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.07f, 0.0f, 0.0f});
      uploadView();
    }

    else if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        glUseProgram(m_shaders.at("planet").handle);
        glUniform1i(m_shaders.at("planet").u_locs.at("Cel"), false);
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        glUseProgram(m_shaders.at("planet").handle);
        glUniform1i(m_shaders.at("planet").u_locs.at("Cel"), true);
    }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
    // mouse handling, x position movement
    m_view_transform = glm::rotate(m_view_transform,
                                   glm::radians(float(pos_x / 50)),
                                   glm::vec3{0.0f, -1.0f, 0.0f});
    // mouse handling, y position movement
    m_view_transform = glm::rotate(m_view_transform,
                                   glm::radians(float(pos_y / 50)),
                                glm::vec3{-1.0f, 0.0f, 0.0f});
    uploadView();
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}