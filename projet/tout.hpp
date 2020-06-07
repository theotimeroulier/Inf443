#pragma once

#include "main/scene_base/base.hpp"

#ifdef SCENE_TOT


// Stores some parameters that can be set from the GUI
struct gui_scene_structure
{
    bool wireframe = false;
    bool surface = true;
    bool skeleton = false;
    bool display_keyframe = true;
    bool display_polygon = true;

    float height = 0.02f;
    float scaling = 8.0f;
    int octave = 3;
    float persistency = 0.2f;
};



struct scene_model : scene_base
{

    /** A part must define two functions that are called from the main function:
     * setup_data: called once to setup data before starting the animation loop
     * frame_draw: called at every displayed frame within the animation loop
     *
     * These two functions receive the following parameters
     * - shaders: A set of shaders.
     * - scene: Contains general common object to define the 3D scene. Contains in particular the camera.
     * - data: The part-specific data structure defined previously
     * - gui: The GUI structure allowing to create/display buttons to interact with the scene.
    */

    void setup_data(std::map<std::string, GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string, GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void set_gui();

    // visual representation of a surface
    //  Surface is a class attribute to be initialized in setup_data and used in frame_draw

    vcl::timer_interval timer;
    gui_scene_structure gui_scene;








};



#endif
