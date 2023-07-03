// standard library
#include <iostream>

// framework
#include "application_solar.hpp"
#include "window_handler.hpp"
#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "texture_loader.hpp"


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

std::vector<GLfloat> SKYBOX_VERTICES = {
        -1.0f, -1.0f,  1.0f,        //        7--------6
        1.0f, -1.0f,  1.0f,         //       /|       /|
        1.0f, -1.0f, -1.0f,         //      4--------5 |
        -1.0f, -1.0f, -1.0f,        //      | |      | |
        -1.0f,  1.0f,  1.0f,        //      | 3------|-2
        1.0f,  1.0f,  1.0f,         //      |/       |/
        1.0f,  1.0f, -1.0f,         //      0--------1
        -1.0f,  1.0f, -1.0f
};

std::vector<GLint> SKYBOX_INDICES = {
        // Right
        1, 2, 6,
        6, 5, 1,
        // Left
        0, 4, 7,
        7, 3, 0,
        // Top
        4, 5, 6,
        6, 7, 4,
        // Bottom
        0, 3, 2,
        2, 1, 0,
        // Back
        0, 1, 5,
        5, 4, 0,
        // Front
        3, 7, 6,
        6, 2, 3
};
bool greyscale = false;
bool blur = false;
bool vertical = false;
bool horizontal = false;
bool chromatic_aberration = false;

#pragma endregion


ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,enterprise_object{}
 ,star_object{}
 ,orbit_object{}
 ,skybox_object{}
 ,skybox_texture{setupSkybox(m_resource_path + "skyboxes/skybox_lilac_orange/")}
 ,post_process_fbo{}
 ,color_texture{}
 ,depth_texture{}
 ,screen_quad_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
 ,sceneGraph{}
{
    // setup all necessary geometries and shaders
    initializeGeometry();
    initializeShaderPrograms();
    initializeFrameBuffer();

    // fill map with all necessary objects, so they can be accessed by name
    std::map<std::string ,model_object> model_objects {
        std::make_pair("planet-object", planet_object),
        std::make_pair("stars-object", star_object),
        std::make_pair("orbit-object", orbit_object),
        std::make_pair("enterprise-object", enterprise_object),
        std::make_pair("skybox-object", skybox_object),
        std::make_pair("screen-quad-object", screen_quad_object)
    };

    // create graph hierarchy
    sceneGraph = setupSolarSystem(model_objects, resource_path);

    std::cout << initial_resolution[0] << ", " << initial_resolution[1] << std::endl;
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
void ApplicationSolar::render() {
    // things that cannot be handled in geometry node, as it requires information about scene are handled here
    std::shared_ptr<PointLightNode> sun_light = std::static_pointer_cast<PointLightNode>(sceneGraph.getRoot()->getChild("Planet-Sun-Holder"));

    glUseProgram(m_shaders.at("planet").handle);
    gl::glUniform3fv(m_shaders.at("planet").u_locs.at("LightColor"),
                     1, glm::value_ptr(sun_light->getLightColour()));

    gl::glUniform3fv(m_shaders.at("planet").u_locs.at("LightPosition"),
                    1, glm::value_ptr(sun_light->getWorldTransform()[3]));

    gl::glUniform1f(m_shaders.at("planet").u_locs.at("LightIntensity"),
                    sun_light->getLightIntensity());

    glUseProgram(m_shaders.at("enterprise").handle);
    gl::glUniform3fv(m_shaders.at("enterprise").u_locs.at("LightColor"),
                     1, glm::value_ptr(sun_light->getLightColour()));

    gl::glUniform3fv(m_shaders.at("enterprise").u_locs.at("LightPosition"),
                     1, glm::value_ptr(sun_light->getWorldTransform()[3]));

    gl::glUniform1f(m_shaders.at("enterprise").u_locs.at("LightIntensity"),
                    sun_light->getLightIntensity());


    glBindFramebuffer(GL_FRAMEBUFFER, post_process_fbo);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //renderSkybox();
    sceneGraph.getRoot()->renderNode(m_shaders, m_view_transform);
    renderFrameBuffer();
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

   glUseProgram(m_shaders.at("skybox").handle);
   glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ViewMatrix"), 1,
                       GL_FALSE, glm::value_ptr(view_matrix));

    glUseProgram(m_shaders.at("screen-quad").handle);
    glUniformMatrix4fv(m_shaders.at("screen-quad").u_locs.at("ViewMatrix"), 1,
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

    glUseProgram(m_shaders.at("skybox").handle);
    glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ProjectionMatrix"),
                       1, GL_FALSE, glm::value_ptr(m_view_projection));

    glUseProgram(m_shaders.at("screen-quad").handle);
    glUniformMatrix4fv(m_shaders.at("screen-quad").u_locs.at("ProjectionMatrix"),
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
    m_shaders.at("planet").u_locs["CameraPosition"] = -1;
    m_shaders.at("planet").u_locs["TextureSampler"] = -1;
    //m_shaders.at("planet").u_locs["NormalMap"] = -1;

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

    m_shaders.emplace("enterprise", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                                {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
    m_shaders.at("enterprise").u_locs["NormalMatrix"] = -1;
    m_shaders.at("enterprise").u_locs["ModelMatrix"] = -1;
    m_shaders.at("enterprise").u_locs["ViewMatrix"] = -1;
    m_shaders.at("enterprise").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("enterprise").u_locs["PlanetColor"] = -1;
    m_shaders.at("enterprise").u_locs["AmbientColor"] = -1;
    m_shaders.at("enterprise").u_locs["LightIntensity"] = -1;
    m_shaders.at("enterprise").u_locs["LightPosition"] = -1;
    m_shaders.at("enterprise").u_locs["LightColor"] = -1;
    m_shaders.at("enterprise").u_locs["CameraPosition"] = -1;
    m_shaders.at("enterprise").u_locs["TextureSampler"] = -1;
    //m_shaders.at("enterprise").u_locs["NormalMap"] = -1;

    m_shaders.emplace("skybox", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/skybox.vert"},
                                                {GL_FRAGMENT_SHADER, m_resource_path + "shaders/skybox.frag"}}});
    m_shaders.at("skybox").u_locs["ModelMatrix"] = -1;
    m_shaders.at("skybox").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("skybox").u_locs["ViewMatrix"] = -1;
    m_shaders.at("skybox").u_locs["TextureSampler"] = -1;

    m_shaders.emplace("screen-quad", shader_program{{{GL_VERTEX_SHADER, m_resource_path + "shaders/quad.vert"},
                                                     {GL_FRAGMENT_SHADER, m_resource_path + "shaders/quad.frag"}}});

    m_shaders.at("screen-quad").u_locs["ViewMatrix"] = -1;
    m_shaders.at("screen-quad").u_locs["ProjectionMatrix"] = -1;
    m_shaders.at("screen-quad").u_locs["ColorTexture"] = -1;
    m_shaders.at("screen-quad").u_locs["DepthTexture"] = -1;
    m_shaders.at("screen-quad").u_locs["Greyscale"] = -1;
    m_shaders.at("screen-quad").u_locs["Horizontal"] = -1;
    m_shaders.at("screen-quad").u_locs["Vertical"] = -1;
    m_shaders.at("screen-quad").u_locs["Blur"] = -1;
    m_shaders.at("screen-quad").u_locs["ChromaticAberration"] = -1;
}

// initialise all geometries
void ApplicationSolar::initializeGeometry() {
    initializeStarGeometry();
    initializeSkyboxGeometry();
    initializeOrbitGeometry();
    initializePlanetGeometry();
    initializeEnterpriseGeometry();
}

# pragma region GEOMETRY INIT
void ApplicationSolar::initializePlanetGeometry() {
    model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL | model::TEXCOORD);

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

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);

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
    // Load the model from a file
    model enterprise_model = model_loader::obj(m_resource_path + "models/USS_Enterprise_NCC-1701_7.obj", model::NORMAL | model::TEXCOORD);

    // Generate a vertex array object for the enterprise
    glGenVertexArrays(1, &enterprise_object.vertex_AO);
    glBindVertexArray(enterprise_object.vertex_AO);

    // Generate a vertex buffer object for the enterprise vertices and bind it
    glGenBuffers(1, &enterprise_object.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, enterprise_object.vertex_BO);

    // Provide the vertex data for the enterprise and store it in the vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * enterprise_model.data.size(), enterprise_model.data.data(), GL_STATIC_DRAW);

    // Enable and specify the layout of vertex attribute 0 (position)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, enterprise_model.vertex_bytes, enterprise_model.offsets[model::POSITION]);

    // Enable and specify the layout of vertex attribute 1 (normal)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, enterprise_model.vertex_bytes, enterprise_model.offsets[model::NORMAL]);

    // Enable and specify the layout of vertex attribute 2 (texture coordinates)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, enterprise_model.vertex_bytes, enterprise_model.offsets[model::TEXCOORD]);

    // Generate an element buffer object for the enterprise indices and bind it
    glGenBuffers(1, &enterprise_object.element_BO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, enterprise_object.element_BO);

    // Provide the index data for the enterprise and store it in the element buffer object
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * enterprise_model.indices.size(), enterprise_model.indices.data(), GL_STATIC_DRAW);

    // Set the draw mode for the enterprise object to GL_TRIANGLES
    enterprise_object.draw_mode = GL_TRIANGLES;

    // Set the number of elements to be drawn for the enterprise
    enterprise_object.num_elements = static_cast<GLsizei>(enterprise_model.indices.size());

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
    // Create a vector to store the points that define the line segments
    std::vector<float> segment_points;

    // Generate points around a circle to define the line segments
    for (int i = 0; i < LINE_SEGMENT_COUNT; ++i) {
        // Calculate the angle for each point along the circle
        float angle = 2 * static_cast<float>(M_PI) * static_cast<float>(i) / LINE_SEGMENT_COUNT;

        // Calculate the x, y, z coordinates for each point
        segment_points.push_back(static_cast<float>(sin(angle)));
        segment_points.push_back(0);
        segment_points.push_back(static_cast<float>(cos(angle)));
    }

    // Generate a vertex array object for the orbit
    glGenVertexArrays(1, &orbit_object.vertex_AO);
    glBindVertexArray(orbit_object.vertex_AO);

    // Generate a vertex buffer object for the orbit vertices and bind it
    glGenBuffers(1, &orbit_object.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);

    // Provide the vertex data for the orbit and store it in the vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, GLsizei(segment_points.size() * sizeof(float)),
                 segment_points.data(), GL_STATIC_DRAW);

    // Enable and specify the layout of vertex attribute 0 (coordinates)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, nullptr);

    // Generate an element buffer object for the orbit indices and bind it
    glGenBuffers(1, &orbit_object.element_BO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbit_object.element_BO);

    // Provide the index data for the orbit and store it in the element buffer object
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 GLsizei(sizeof(float) * segment_points.size()), segment_points.data(),
                 GL_STATIC_DRAW);

    // Set the draw mode for the orbit object to GL_LINE_LOOP to draw lines between points
    // and finally connect them together to form a circle
    orbit_object.draw_mode = GL_LINE_LOOP;

    // Set the number of elements to be drawn for the orbit
    orbit_object.num_elements = LINE_SEGMENT_COUNT;

}

void ApplicationSolar::initializeSkyboxGeometry() {
    // Generate a vertex array object for the skybox
    glGenVertexArrays(1, &skybox_object.vertex_AO);
    glBindVertexArray(skybox_object.vertex_AO);

    // Generate a vertex buffer object for the skybox vertices and bind it
    glGenBuffers(1, &skybox_object.vertex_BO);
    glBindBuffer(GL_ARRAY_BUFFER, skybox_object.vertex_BO);

    // Provide the vertex data for the skybox and store it in the vertex buffer object
    glBufferData(GL_ARRAY_BUFFER, sizeof(SKYBOX_VERTICES), &SKYBOX_VERTICES, GL_STATIC_DRAW);

    // Enable and specify the layout of vertex attribute 0 (coordinates)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    // Generate an element buffer object for the skybox indices and bind it
    glGenBuffers(1, &skybox_object.element_BO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skybox_object.element_BO);

    // Provide the index data for the skybox and store it in the element buffer object
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SKYBOX_INDICES), &SKYBOX_INDICES, GL_STATIC_DRAW);

    // Set the draw mode for the skybox object to GL_TRIANGLES
    skybox_object.draw_mode = GL_TRIANGLES;

    // Set the number of elements to be drawn for the skybox
    skybox_object.num_elements = static_cast<GLsizei>(SKYBOX_VERTICES.size());
}

void ApplicationSolar::initializeFrameBuffer() {
    // Generate a framebuffer object for post-processing
    glGenFramebuffers(1, &post_process_fbo);

    // Generate textures for color and depth
    glGenTextures(1, &color_texture);
    glGenTextures(1, &depth_texture);

    // Update the buffer with initial resolution
    updateBuffer(initial_resolution[0], initial_resolution[1]);

    // Define vertex data for a rectangular quad
    float rectVertices[] = {
            // Coordinates    // Texture coordinates
            1.f, -1.f, 1.f, 0.f,   // Top right
            -1.f, -1.f, 0.f, 0.f,  // Top left
            -1.f, 1.f, 0.f, 1.f,   // Bottom left

            1.f, 1.f, 1.f, 1.f,    // Bottom right
            1.f, -1.f, 1.f, 0.f,   // Top right
            -1.f, 1.f, 0.f, 1.f    // Bottom left
    };

    // Generate a vertex array object and a vertex buffer object for the quad
    glGenVertexArrays(1, &screen_quad_object.vertex_AO);
    glGenBuffers(1, &screen_quad_object.vertex_BO);
    glBindVertexArray(screen_quad_object.vertex_AO);
    glBindBuffer(GL_ARRAY_BUFFER, screen_quad_object.vertex_BO);

    // Provide the vertex data to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), &rectVertices, GL_STATIC_DRAW);

    // Enable and specify the layout of vertex attribute 0 (coordinates)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);

    // Enable and specify the layout of vertex attribute 1 (texture coordinates)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    // Set the number of elements and draw mode for the quad object
    screen_quad_object.num_elements = 6;
    screen_quad_object.draw_mode = GL_TRIANGLES;
}
#pragma endregion

void ApplicationSolar::renderSkybox() {
    //glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glCullFace(GL_FRONT);
    glUseProgram(m_shaders.at("skybox").handle);

    /*glm::fmat4 model_matrix = getWorldTransform() * getLocalTransform();
    glUniformMatrix4fv(m_shaders.at("skybox").u_locs.at("ModelMatrix"),
                       1, GL_FALSE, glm::value_ptr(model_matrix));*/

    glUniform1i(m_shaders.at("skybox").u_locs.at("TextureSampler"), 0);

    //texture_object skybox_texture = setupSkybox(m_resource_path + "skyboxes/skybox_lilac_orange/");


    // bind the VAO to draw
    gl::glBindVertexArray(skybox_object.vertex_AO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture.handle);
    // draw bound vertex array using bound shader
    glDrawElements(skybox_object.draw_mode, skybox_object.num_elements, model::INDEX.type, nullptr);
    glDepthFunc(GL_LESS);
}

void ApplicationSolar::renderFrameBuffer() {
    // Disable depth testing to ensure the quad is rendered on top of everything
    glDisable(GL_DEPTH_TEST);
    // Bind the default framebuffer (screen) for rendering
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Use the shader program for rendering the screen quad
    glUseProgram(m_shaders.at("screen-quad").handle);

    // Bind the vertex array object for the screen quad
    glBindVertexArray(screen_quad_object.vertex_AO);

    // Activate texture unit 0 and bind the color texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_texture);

    // Activate texture unit 1 and bind the depth texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depth_texture);

    // Draw the screen quad using the specified draw mode, vertex count, and attributes
    glDrawArrays(screen_quad_object.draw_mode, 0, screen_quad_object.num_elements);

    glEnable(GL_DEPTH_TEST);
}

void ApplicationSolar::createBufferTexture(GLuint texture, int width, int height, GLenum target, GLenum format,
                                           GLenum attachment) {
    // Bind the specified texture to the specified target
    glBindTexture(target, texture);

    // Check if the target is GL_TEXTURE_2D_MULTISAMPLE
    /*if (target == GL_TEXTURE_2D_MULTISAMPLE) {
        // If it is a multisample texture, allocate storage for it
        glTexImage2DMultisample(target, 8, format, width, height, GL_TRUE);
    } else {*/
        // If it is a regular texture, allocate storage for it
    glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
    //}

    // Set texture filtering parameters for minification and magnification to GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Set texture wrapping parameters to prevent edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Attach the texture to the specified attachment point of the frame buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, target, texture, 0);
}

void ApplicationSolar::updateBuffer(int width, int height) {
    glBindFramebuffer(GL_FRAMEBUFFER, post_process_fbo);
    createBufferTexture(color_texture, width, height, GL_TEXTURE_2D, GL_RGB, GL_COLOR_ATTACHMENT0);
    createBufferTexture(depth_texture, width, height, GL_TEXTURE_2D, GL_DEPTH_COMPONENT, GL_DEPTH_ATTACHMENT);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Error occurred in frame buffer: " << status << std::endl;
    }

}

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
        greyscale = !greyscale;
        glUseProgram(m_shaders.at("screen-quad").handle);
        glUniform1i(m_shaders.at("screen-quad").u_locs.at("Greyscale"), greyscale);
    }

    else if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        horizontal = !horizontal;
        glUseProgram(m_shaders.at("screen-quad").handle);
        glUniform1i(m_shaders.at("screen-quad").u_locs.at("Horizontal"), horizontal);
    }

    else if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        vertical = !vertical;
        glUseProgram(m_shaders.at("screen-quad").handle);
        glUniform1i(m_shaders.at("screen-quad").u_locs.at("Vertical"), vertical);
    }

    else if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        blur = !blur;
        glUseProgram(m_shaders.at("screen-quad").handle);
        glUniform1i(m_shaders.at("screen-quad").u_locs.at("Blur"), blur);
    }

    else if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        chromatic_aberration = !chromatic_aberration;
        glUseProgram(m_shaders.at("screen-quad").handle);
        glUniform1i(m_shaders.at("screen-quad").u_locs.at("ChromaticAberration"), chromatic_aberration);
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