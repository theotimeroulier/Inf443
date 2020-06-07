
#include "tout.hpp"


#include <random>
#ifdef SCENE_TOT



// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;



#include "bateau/bateau.cpp"
#include "mer/mer.cpp"
#include "ville/ville.cpp"
#include "car/car.cpp"
#include "homme/homme.cpp"
#include "batiment/batiment.cpp"
#include "park/park.cpp"
#include "road/road.cpp"
#include "phare/phare.cpp"
#include "skybox/skybox.cpp"
#include "cygne/cygne.cpp"
#include "girafe/girafe.cpp"

void scene_model::setup_data(std::map<std::string, GLuint>& shaders, scene_structure&, gui_structure&)
{
    // valeurs générales
    t_vol = 8;
    vol = false;
    int r_v = 4; // larg routes verticales
    int r_h = 5;  // larg routes horizontales

    
    // phare
    init_phare(phare, chapeau, barriere, fenetre);
    // skybox
    init_skybox(cube, texture_skybox);
    // bateau
    init_bateau(pos_bateau, bateau, force, par);
    
    

    // rocher et mer
    rocher = mesh_drawable(create_grid());
    rocher.uniform.color = { 0.5f,0.5f,0.5f };
    terrain_2 = mesh_drawable(create_mer());


    //texture
    texture_id = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/mer.png"));
    texture_id_2 = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/roche.png"));
    
    // park
    init_park(square, lac, tree, r_v, r_h, u0, v0, gui_scene, shaders, park_pos, tree_pos, park_pos_2, texture_id_park,texture_lac);
    
    // cygne
    init_cygne( cygne,cygne_pos,texture_plumes,  r_v,  r_h, gui_scene,  park_pos_2,shaders);

    // batiment
    init_bat(batiment, bat_pos, texture_id_bat, gui_scene, r_v, r_h,park_pos_2);

    // road
    texture_id_road_v = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/road_v.png"));
    texture_id_road_h = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/road_h.png"));
    road_h_pos = create_road_h_pos(gui_scene, r_h, 100); // contient les routes horizontales
    road_v_pos = create_road_v_pos(gui_scene, 100, r_v); // contient les routes verticales
    
    // car
    car = create_car();
    car.set_shader_for_all_elements(shaders["mesh"]);
    car_pos = create_car_pos(gui_scene);

    // homme
    homme = create_homme();
    homme.set_shader_for_all_elements(shaders["mesh"]);
    perso_pos = create_pos_perso(r_v, r_h, gui_scene);
    
    // ville
    terrain = create_terrain(gui_scene);
    terrain.uniform.color = { 0.6f,0.6f,0.6f };
    
    // girafe
    init_giraffe(giraffe, keyframe_giraffe, timer_g, timer_g_2,  timer_g_3, timer_g_4, timer_g_5, shaders);

    
    keyframes = car_pos[0]; // pour fixer le temps

    
    // Set timer bounds
    // You should adapt these extremal values to the type of interpolation
    timer.t_min = keyframes[1].t;                   // first time of the keyframe
    timer.t_max = keyframes[keyframes.size() - 2].t;  // last time of the keyframe
    timer.t = timer.t_min;
    
    


}

void scene_model::frame_draw(std::map<std::string, GLuint>& shaders, scene_structure& scene, gui_structure&)
{
    
    int r_v = 4; // larg routes verticales
    int r_h = 5;  // larg routes horizontales
    
    timer.update();
    float dt = timer.scale * 0.01f;
    float t = timer.t;
    if (t < timer.t_min + 0.1f) { // les voitures atterrissent
        vol = false;
    }
    
    // homme
    draw_homme(r_v, r_h, homme, t, scene, gui_scene, perso_pos, dt);
    //phare
    draw_phare(shaders, scene, phare, chapeau, barriere, fenetre);
    // Bateau
    draw_bateau(par, force, dt, t, pos_bateau, bateau, scene, shaders);

    // cars
    draw_cars(t, t_vol, vol, gui_scene, scene, car, car_pos);


    //skybox
    draw_skybox(shaders, scene, cube);
    
    
    // terrain
    draw(terrain, scene.camera, shaders["mesh"]);
    
    // park et arbres
    draw_park(square, lac, tree, r_v, r_h, u0, v0, gui_scene, shaders, park_pos, tree_pos, park_pos_2, texture_id_park,scene, texture_lac);
    
    // cygne 
    cygne.update_local_to_global_coordinates();
    draw_cygne(cygne, t, scene, gui_scene, cygne_pos);
    
    //tree
     int cmp=0;
    for (vec3 position : tree_pos) {
        if (cmp%4==0){
            tree["tronc0-0"].transform.translation = position;
            tree.update_local_to_global_coordinates();
            draw(tree, scene.camera);
        }
        else{
            tree_simple["tronc"].transform.translation = position;
            tree_simple.update_local_to_global_coordinates();
            draw(tree_simple, scene.camera);

        }
        ++cmp;

    }
    

    

    // batiments
    draw_bat(bat_pos, batiment, texture_id_bat, shaders, scene);

    // road
    draw_road(road_v_pos, road_h_pos, texture_id_road_h, texture_id_road_v, shaders, scene);

    // girafe
    draw_giraffes(giraffe, keyframe_giraffe, timer_g, timer_g_2, timer_g_3, timer_g_4, timer_g_5, shaders, scene, gui_scene);

    // rocher
    glBindTexture(GL_TEXTURE_2D, texture_id_2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    draw(rocher, scene.camera, shaders["mesh"]);

    glEnable(GL_POLYGON_OFFSET_FILL); // avoids z-fighting when displaying wireframe
    // Before displaying a textured surface: bind the associated texture id
    glPolygonOffset(1.0, 1.0);


    // mer = terrain_2
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    update_mer(terrain_2, t);
    draw(terrain_2, scene.camera, shaders["mesh"]);



    if (gui_scene.wireframe) { // wireframe if asked from the GUI
        glPolygonOffset(1.0, 1.0);
        draw(rocher, scene.camera, shaders["wireframe"]);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(false);


    glBindTexture(GL_TEXTURE_2D, texture_sirene);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // avoids sampling artifacts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // avoids sampling artifacts
    
    glDepthMask(true);
    
}


#endif
