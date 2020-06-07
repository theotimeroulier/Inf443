#include "ville.hpp"


#include <random>


#ifdef SCENE_TOT


// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::

using namespace vcl;








float evaluate_terrain_z(float u, float v, const gui_scene_structure& gui_scene)
{
    

    // pour coller à tt sol
    const float scaling = 0.35f;
    const int octave = 7;
    const float persistency = 0.7f;


    u = 20.0f / 60.0f + u * 20.0f / 60.0f;
    v = 20.0f / 60.0f + v * 20.0f / 60.0f;
    const float height = evaluate_terrain_z_T(u, v);
    const float z_y = evaluate_terrain_z_y(u, v);
    const float z_X = evaluate_terrain_z_X(u, v);
    const float z_Y = evaluate_terrain_z_Y(u, v);
    const vec2 u0 = { 0.08, 0.92f };
    const float h0 = 15.0f;
    const float sigma0 = 0.1f;
    float d0 = norm(vec2(u, v) - u0) / sigma0;
    const float bosse = h0 * std::exp(-d0 * d0);
    const std::vector<vec2> route_haut = { {0.35f,0.64f} ,{0.45f,0.64f},{0.40f,0.64f} , {0.5f,0.64f},{0.6f,0.64f} ,{0.55f,0.64f},{0.64f,0.64f} };
    float bosse_2 = 0;
    for (int k = 0; k < 7; k++) {
        d0 = norm(vec2(u, v) - route_haut[k]) / 0.022f;
        bosse_2 += -0.2f * std::exp(-d0 * d0);
    }






    // Evaluate Perlin noise
    const float noise = perlin(scaling * u, scaling * v, octave, persistency);


    // 3D vertex coordinates
    const float x = 60 * (u - 0.5f);
    const float y = 60 * (v - 0.5f);
    float z = height;

    const float c = 245 / 255.0f + 0.7f * noise;

    // Compute coordinates


    z = bosse_2 + bosse * noise - 5 * atan(0.5f * (x - 27) + z) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y);

    return z + 0.01f;
}


vec3 evaluate_terrain(float u, float v, const gui_scene_structure& gui_scene)
{
    const float x = 20 * (u - 0.5f);
    const float y = 20 * (v - 0.5f);
    const float z = evaluate_terrain_z(u, v, gui_scene);

    return { x,y,z };
}



mesh create_terrain(const gui_scene_structure& gui_scene)
{
    // Number of samples of the terrain is N x N
    const size_t N = 100;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N * N);
    terrain.texture_uv.resize(N * N);

    // Fill terrain geometry
    for (size_t ku = 0; ku < N; ++ku)
    {
        for (size_t kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);

            // Compute coordinates
            terrain.position[kv + N * ku] = evaluate_terrain(u, v, gui_scene);
            // terrain.texture_uv[kv + N * ku] = { u * 10,v * 10 };
        }
    }


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    const unsigned int Ns = N;
    for (unsigned int ku = 0; ku < Ns - 1; ++ku)
    {
        for (unsigned int kv = 0; kv < Ns - 1; ++kv)
        {
            const unsigned int idx = kv + N * ku; // current vertex offset

            const uint3 triangle_1 = { idx, idx + 1 + Ns, idx + 1 };
            const uint3 triangle_2 = { idx, idx + Ns, idx + 1 + Ns };

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    return terrain;
}








void scene_model::set_gui()
{
    ImGui::SliderFloat("Time", &timer.t, timer.t_min, timer.t_max);
    ImGui::SliderFloat("Time scale", &timer.scale, 0.1f, 3.0f);

    ImGui::Text("Display: "); ImGui::SameLine();
    ImGui::Checkbox("keyframe", &gui_scene.display_keyframe); ImGui::SameLine();
    ImGui::Checkbox("polygon", &gui_scene.display_polygon);

    

}







#endif