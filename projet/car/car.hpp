#pragma once


#ifdef SCENE_TOT
struct vec3t {
    vcl::vec3 p; // position
    float t;     // time
};

vcl::hierarchy_mesh_drawable car;
std::vector<vcl::buffer<vec3t>> car_pos;
vcl::buffer<vec3t> keyframes;
bool vol;
float t_vol;



#endif

