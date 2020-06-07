#pragma once



#ifdef SCENE_TOT


// Stores some parameters that can be set from the GUI


struct vec3bat {
    vcl::vec3 p; // position
    int n;     // petit 0 , moyen 1 , grand 2  ou rien 9
    int text; // la texture
};
std::vector<vcl::mesh_drawable> batiment;
std::vector<vec3bat> bat_pos;
std::vector<GLuint> texture_id_bat;


    




    










#endif
