
#include "road.hpp"


#include <random>
#ifdef SCENE_TOT



// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;

mesh create_road_h(const gui_scene_structure& gui_scene, size_t Nh, size_t Nl, float pos_larg, float pos_high) {
    const size_t N = 200;
    Nh = int(Nh * N / 100);
    Nl = int(Nl * N / 100);



    mesh road;
    road.position.resize(Nh * Nl);
    road.texture_uv.resize(Nh * Nl);


    for (size_t ku = 0; ku < Nl; ++ku)
    {
        for (size_t kv = 0; kv < Nh; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f) + pos_larg;
            const float v = kv / (N - 1.0f) + pos_high;
            // Compute coordinates
            road.position[kv + Nh * ku] = evaluate_terrain(u, v, gui_scene) + vec3(0.0f, 0.0f, 0.005f);
            road.texture_uv[kv + Nh * ku] = { 4 * ku / (Nl - 1.0f),kv / (Nh - 1.0f) };
        }
    }
    for (unsigned int ku = 0; ku < Nl - 1; ++ku)
    {
        for (unsigned int kv = 0; kv < Nh - 1; ++kv)
        {
            const unsigned int idx = kv + Nh * ku; // current vertex offset

            const uint3 triangle_1 = { idx, idx + 1 + Nh, idx + 1 };
            const uint3 triangle_2 = { idx, idx + Nh, idx + 1 + Nh };

            road.connectivity.push_back(triangle_1);
            road.connectivity.push_back(triangle_2);
        }
    }

    return road;

}

mesh create_road_v(const gui_scene_structure& gui_scene, size_t Nh, size_t Nl, float pos_larg, float pos_high) {
    const size_t N = 200;
    Nh = int(Nh * N / 100);
    Nl = int(Nl * N / 100);



    mesh road;
    road.position.resize(Nh * Nl);
    road.texture_uv.resize(Nh * Nl);


    for (size_t ku = 0; ku < Nl; ++ku)
    {
        for (size_t kv = 0; kv < Nh; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f) + pos_larg;
            const float v = kv / (N - 1.0f) + pos_high;
            // Compute coordinates
            road.position[kv + Nh * ku] = evaluate_terrain(u, v, gui_scene) + vec3(0.0f, 0.0f, 0.005f);
            road.texture_uv[kv + Nh * ku] = { ku / (Nl - 1.0f),5 * kv / (Nh - 1.0f) };
        }
    }
    for (unsigned int ku = 0; ku < Nl - 1; ++ku)
    {
        for (unsigned int kv = 0; kv < Nh - 1; ++kv)
        {
            const unsigned int idx = kv + Nh * ku; // current vertex offset

            const uint3 triangle_1 = { idx, idx + 1 + Nh, idx + 1 };
            const uint3 triangle_2 = { idx, idx + Nh, idx + 1 + Nh };

            road.connectivity.push_back(triangle_1);
            road.connectivity.push_back(triangle_2);
        }
    }

    return road;

}

std::vector<vcl::mesh_drawable> create_road_h_pos(const gui_scene_structure& gui_scene, size_t high, size_t larg) {
    std::vector<vcl::mesh_drawable> h;
    mesh m;
    int nb = int(100 / (high * 3)) + 1;

    for (unsigned k = 0; k < nb; k++) {

        m = create_road_h(gui_scene, high, larg, 0.0f, k * 3.0f * high / 100.0f);

        h.push_back(m);
    }
    return h;
}

std::vector<vcl::mesh_drawable> create_road_v_pos(const gui_scene_structure& gui_scene, size_t high, size_t larg) {
    std::vector<vcl::mesh_drawable> h;
    mesh m;
    int nb = int(100 / (larg * 3)) + 1;
    for (unsigned k = 0; k < nb; k++) {
        m = create_road_v(gui_scene, high, larg, k * 3 * larg / 100.0f, 0.0f);
        h.push_back(m);
    }
    return h;
}

void draw_road(std::vector<vcl::mesh_drawable> road_v, std::vector<vcl::mesh_drawable> road_h, GLuint h, GLuint v, std::map<std::string, GLuint>& shaders, scene_structure& scene) {
    glBindTexture(GL_TEXTURE_2D, h);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    for (vcl::mesh_drawable road : road_h) {
        draw(road, scene.camera, shaders["mesh"]);
    }


    glBindTexture(GL_TEXTURE_2D, v);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    for (vcl::mesh_drawable road : road_v) {
        draw(road, scene.camera, shaders["mesh"]);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}




#endif
