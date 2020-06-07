#pragma once



#ifdef SCENE_TOT


// Stores some parameters that can be set from the GUI


struct particle_element
{
    vcl::vec3 p; // Position
    vcl::vec3 v; // Speed
};


// bateau
std::vector<particle_element> par;
std::vector<vcl::vec3> force;
vcl::hierarchy_mesh_drawable bateau;
std::vector<vcl::vec2> pos_bateau;
// bateau





    




    










#endif
