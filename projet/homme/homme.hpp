#pragma once


#ifdef SCENE_TOT

struct vec3p {
    vcl::vec3 p; // position
    std::vector<vcl::vec3> v;     // speed
    float z = 0; // angle z pour la tête
    float y = 0; // angle y
    bool traverse = false;
    int direction; // 0 : droite , 1 gauche , 2 haut , 3 bas
    std::vector<float> valeur; // la valeur a dépasser pour tourner


};

vcl::hierarchy_mesh_drawable homme;
std::vector<vec3p> perso_pos;



#endif

