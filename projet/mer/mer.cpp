#include "mer.hpp"


#include <random>


#ifdef SCENE_TOT


// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::

using namespace vcl;

void update_mer(mesh_drawable& mer, float t) {
    mesh terrain;
    const size_t N = 100;
    terrain.position.resize(N * N);
    terrain.color.resize(N * N);
    terrain.texture_uv.resize(N * N);

    // Fill terrain geometry
    for (size_t ku = 0; ku < N; ++ku)
    {
        for (size_t kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);


            // get gui parameters
            const float scaling = 0.35f;
            const int octave = 7;
            const float persistency = 0.7f;



            // Evaluate Perlin noise
            const float noise = perlin(scaling * u, scaling * v, octave, persistency);

            // 3D vertex coordinates
            const float x = 500 * (u - 0.5f);
            const float y = 500 * (v - 0.5f);
            const float c = 245 / 255.0f + 0.7f * noise;

            terrain.position[kv + N * ku] = { x,y,15.5f + 0.4f * cos(noise * x * 2 * 3.14f / 9.0f - t * 2 * 3.14f / 1 + noise * y * 2 * 3.14f / 3.0f) };
            terrain.texture_uv[kv + N * ku] = { 3 * u,3 * v };
            terrain.color[kv + N * ku] = { c,c,c,1 };



        }

    }


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    unsigned int Ns = N;
    for (size_t ku = 0; ku < Ns - 1; ++ku)
    {
        for (size_t kv = 0; kv < Ns - 1; ++kv)
        {
            const unsigned int idx = kv + Ns * ku; // current vertex offset

            const uint3 triangle_1 = { idx, idx + 1 + Ns, idx + 1 };
            const uint3 triangle_2 = { idx, idx + Ns, idx + 1 + Ns };

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }


    for (size_t ku = 0; ku < N; ++ku)
    {
        for (size_t kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);
            const float x = 100 * (u - 0.5f);
            const float y = 100 * (v - 0.5f);



        }
    }
    mer = terrain;


}

float evaluate_terrain_z_T(float u, float v)
{
    const vec2 u0 = { 0.89f, 0.0f };
    const vec2 u1 = { 0.87f, 0.5f };
    const vec2 u2 = { 0.86f, 0.7f };
    const vec2 u3 = { 0.87f, 0.7f };
    const vec2 u4 = { 0.88f, 0.1f };

    const float h0 = 1.5f;
    const float h1 = 1.5f;
    const float h2 = 1.0f;
    const float h3 = 2.0f;
    const float h4 = 2.0f;
    const float sigma0 = 0.5f;
    const float sigma1 = 0.15f;
    const float sigma2 = 0.2f;
    const float sigma3 = 0.2f;
    const float sigma4 = 0.6f;

    const float d0 = norm(vec2(u, v) - u0) / sigma0;
    const float d1 = norm(vec2(u, v) - u1) / sigma1;
    const float d2 = norm(vec2(u, v) - u2) / sigma2;
    const float d3 = norm(vec2(u, v) - u3) / sigma3;
    const float d4 = norm(vec2(u, v) - u4) / sigma4;

    //Perlin noise
    // get gui parameters
    const float scaling = 0.35f;
    const int octave = 7;
    const float persistency = 0.7f;


    // Evaluate Perlin noise
    const float noise = perlin(scaling * u, scaling * v, octave, persistency);


    const float z = h0 * std::exp(-d0 * d0) + h1 * std::exp(-d1 * d1) + h2 * std::exp(-d2 * d2) + h3 * std::exp(-d3 * d3) + h2 * std::exp(-d4 * d4) + h4 * std::exp(-d4 * d4);

    return z * noise;
}
float evaluate_terrain_z_y(float u, float v)
{
    const vec2 u0 = { 0, 0.89f };
    const vec2 u1 = { 0.5f, 0.87f };
    const vec2 u2 = { 0.7f, 0.86f };
    const vec2 u3 = { 0.7f, 0.87f };
    const vec2 u4 = { 0.1f, 0.88f };

    const float h0 = 1.5f;
    const float h1 = 1.5f;
    const float h2 = 1.0f;
    const float h3 = 2.0f;
    const float h4 = 2.0f;
    const float sigma0 = 0.5f;
    const float sigma1 = 0.15f;
    const float sigma2 = 0.2f;
    const float sigma3 = 0.2f;
    const float sigma4 = 0.6f;

    const float d0 = norm(vec2(u, v) - u0) / sigma0;
    const float d1 = norm(vec2(u, v) - u1) / sigma1;
    const float d2 = norm(vec2(u, v) - u2) / sigma2;
    const float d3 = norm(vec2(u, v) - u3) / sigma3;
    const float d4 = norm(vec2(u, v) - u4) / sigma4;

    //Perlin noise
    // get gui parameters
    const float scaling = 0.35f;
    const int octave = 7;
    const float persistency = 0.7f;


    // Evaluate Perlin noise
    const float noise = perlin(scaling * u, scaling * v, octave, persistency);


    const float z = h0 * std::exp(-d0 * d0) + h1 * std::exp(-d1 * d1) + h2 * std::exp(-d2 * d2) + h3 * std::exp(-d3 * d3) + h2 * std::exp(-d4 * d4) + h4 * std::exp(-d4 * d4);

    return z * noise;
}


float evaluate_terrain_z_X(float u, float v)
{
    const vec2 u0 = { 0.04f, 0.9f };
    const vec2 u1 = { 0.03f, 0.5f };
    const vec2 u2 = { 0.02f, 0.7f };
    const vec2 u3 = { 0.03f, 0.2f };
    const vec2 u4 = { 0.84f, 0.1f };

    const float h0 = 1.5f;
    const float h1 = 1.5f;
    const float h2 = 1.0f;
    const float h3 = 2.0f;
    const float h4 = 2.0f;
    const float sigma0 = 0.5f;
    const float sigma1 = 0.15f;
    const float sigma2 = 0.2f;
    const float sigma3 = 0.2f;
    const float sigma4 = 0.6f;

    const float d0 = norm(vec2(u, v) - u0) / sigma0;
    const float d1 = norm(vec2(u, v) - u1) / sigma1;
    const float d2 = norm(vec2(u, v) - u2) / sigma2;
    const float d3 = norm(vec2(u, v) - u3) / sigma3;
    const float d4 = norm(vec2(u, v) - u4) / sigma4;

    //Perlin noise
    // get gui parameters
    const float scaling = 0.35f;
    const int octave = 7;
    const float persistency = 0.7f;

    // Evaluate Perlin noise
    const float noise = perlin(scaling * u, scaling * v, octave, persistency);


    const float z = h0 * std::exp(-d0 * d0) + h1 * std::exp(-d1 * d1) + h2 * std::exp(-d2 * d2) + h3 * std::exp(-d3 * d3) + h2 * std::exp(-d4 * d4) + h4 * std::exp(-d4 * d4);

    return z * noise;
}
float evaluate_terrain_z_Y(float u, float v)
{
    const vec2 u0 = { 0.9f, 0.04f };
    const vec2 u1 = { 0.5f, 0.03f };
    const vec2 u2 = { 0.7f, 0.02f };
    const vec2 u3 = { 0.2f, 0.03f };
    const vec2 u4 = { 0.1f, 0.84f };

    const float h0 = 1.5f;
    const float h1 = 1.5f;
    const float h2 = 1.0f;
    const float h3 = 2.0f;
    const float h4 = 2.0f;
    const float sigma0 = 0.5f;
    const float sigma1 = 0.15f;
    const float sigma2 = 0.2f;
    const float sigma3 = 0.2f;
    const float sigma4 = 0.6f;

    const float d0 = norm(vec2(u, v) - u0) / sigma0;
    const float d1 = norm(vec2(u, v) - u1) / sigma1;
    const float d2 = norm(vec2(u, v) - u2) / sigma2;
    const float d3 = norm(vec2(u, v) - u3) / sigma3;
    const float d4 = norm(vec2(u, v) - u4) / sigma4;

    //Perlin noise
    // get gui parameters
    const float scaling = 0.35f;
    const int octave = 7;
    const float persistency = 0.7f;

    // Evaluate Perlin noise
    const float noise = perlin(scaling * u, scaling * v, octave, persistency);


    const float z = h0 * std::exp(-d0 * d0) + h1 * std::exp(-d1 * d1) + h2 * std::exp(-d2 * d2) + h3 * std::exp(-d3 * d3) + h2 * std::exp(-d4 * d4) + h4 * std::exp(-d4 * d4);

    return z * noise;
}
mesh create_grid()
{
    // Number of samples of the terrain is N x N
    const size_t N = 600;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N * N);
    terrain.color.resize(N * N);
    terrain.texture_uv.resize(N * N);

    // Fill terrain geometry
    for (size_t ku = 0; ku < N; ++ku)
    {
        for (size_t kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);


            // get gui parameters
            const float scaling = 0.35f;
            const int octave = 7;
            const float persistency = 0.7f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            const vec2 u0 = { 0.08, 0.92f };
            const float h0 = 15.0f;
            const float sigma0 = 0.1f;
            const float d0 = norm(vec2(u, v) - u0) / sigma0;
            const float bosse = h0 * std::exp(-d0 * d0);






            // Evaluate Perlin noise
            const float noise = perlin(scaling * u, scaling * v, octave, persistency);


            // 3D vertex coordinates
            const float x = 60 * (u - 0.5f);
            const float y = 60 * (v - 0.5f);
            const float z = height;

            const float c = 245 / 255.0f + 0.7f * noise;

            // Compute coordinates



            terrain.position[kv + N * ku] = { x,y,bosse * noise - 5 * atan(0.5f * (x - 27) + z) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) };
            terrain.texture_uv[kv + N * ku] = { 4 * u,4 * v };
            terrain.color[kv + N * ku] = { c,c,c,1 };
        }


    }



    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    unsigned int Ns = N;
    for (size_t ku = 0; ku < Ns - 1; ++ku)
    {
        for (size_t kv = 0; kv < Ns - 1; ++kv)
        {
            const unsigned int idx = kv + Ns * ku; // current vertex offset

            const uint3 triangle_1 = { idx, idx + 1 + Ns, idx + 1 };
            const uint3 triangle_2 = { idx, idx + Ns, idx + 1 + Ns };

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    return terrain;
}
mesh create_mer()
{
    // Number of samples of the terrain is N x N
    const size_t N = 100;

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N * N);
    terrain.color.resize(N * N);
    terrain.texture_uv.resize(N * N);

    // Fill terrain geometry
    for (size_t ku = 0; ku < N; ++ku)
    {
        for (size_t kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);


            // get gui parameters
            const float scaling = 0.35f;
            const int octave = 7;
            const float persistency = 0.7f;

            // Evaluate Perlin noise
            const float noise = perlin(scaling * u, scaling * v, octave, persistency);

            // 3D vertex coordinates
            const float x = 500 * (u - 0.5f);
            const float y = 500 * (v - 0.5f);

            const float c = 245 / 255.0f + 0.7f * noise;

            terrain.position[kv + N * ku] = { x,y,15 + 0.4f * cos(x * noise * 2 * 3.14f / 9.0f + noise * y * 2 * 3.14f / 3.0f) };
            terrain.texture_uv[kv + N * ku] = { 3 * u,3 * v };
            terrain.color[kv + N * ku] = { c,c,c,1 };



        }
    }


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    unsigned int Ns = N;
    for (size_t ku = 0; ku < Ns - 1; ++ku)
    {
        for (size_t kv = 0; kv < Ns - 1; ++kv)
        {
            const unsigned int idx = kv + Ns * ku; // current vertex offset

            const uint3 triangle_1 = { idx, idx + 1 + Ns, idx + 1 };
            const uint3 triangle_2 = { idx, idx + Ns, idx + 1 + Ns };

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    return terrain;
}
mesh create_cube(float u) {
    mesh cube;
    cube.position.push_back({ -u,-u,-0.1f });
    cube.position.push_back({ -u,-u,-0.1f });
    cube.position.push_back({ -u,-u,-0.1f });
    cube.position.push_back({ u,-u,-0.1f });
    cube.position.push_back({ u,-u,-0.1f });
    cube.position.push_back({ u,-u,2 * u - 0.1f });
    cube.position.push_back({ -u,-u,2 * u - 0.1f });
    cube.position.push_back({ -u,u,2 * u - 0.1f });
    cube.position.push_back({ u,u,2 * u - 0.1f });
    cube.position.push_back({ u,u,-0.1f });
    cube.position.push_back({ u,u,-0.1f });
    cube.position.push_back({ -u,u,-0.1f });
    cube.position.push_back({ -u,u,-0.1f });
    cube.position.push_back({ -u,u,-0.1f });

    //connectivité

    const uint3 triangle_1 = { 1,11,6 };
    const uint3 triangle_2 = { 11,6,7 };
    const uint3 triangle_3 = { 7,6,5 };
    const uint3 triangle_4 = { 7,5,8 };
    const uint3 triangle_5 = { 6,3,0 };
    const uint3 triangle_6 = { 6,3,5 };
    const uint3 triangle_7 = { 13,7,8 };
    const uint3 triangle_8 = { 13,8,9 };
    const uint3 triangle_9 = { 8,10,4 };
    const uint3 triangle_10 = { 8,4,5 };
    const uint3 triangle_11 = { 10,12,2 };
    const uint3 triangle_12 = { 10,4,2 };

    cube.connectivity.push_back(triangle_1);
    cube.connectivity.push_back(triangle_2);
    cube.connectivity.push_back(triangle_3);
    cube.connectivity.push_back(triangle_4);
    cube.connectivity.push_back(triangle_5);
    cube.connectivity.push_back(triangle_6);
    cube.connectivity.push_back(triangle_7);
    cube.connectivity.push_back(triangle_8);
    cube.connectivity.push_back(triangle_9);
    cube.connectivity.push_back(triangle_10);
    cube.connectivity.push_back(triangle_11);
    cube.connectivity.push_back(triangle_12);

    //texture_uv

    cube.texture_uv.push_back({ 0.25f,1 });
    cube.texture_uv.push_back({ 0,2 / 3.0f });
    cube.texture_uv.push_back({ 1,2 / 3.0f });
    cube.texture_uv.push_back({ 2 / 4.0f,1 });
    cube.texture_uv.push_back({ 3 / 4.0f,2 / 3.0f });
    cube.texture_uv.push_back({ 0.5f,2 / 3.0f });
    cube.texture_uv.push_back({ 0.25f,2 / 3.0f });
    cube.texture_uv.push_back({ 0.25f,1 / 3.0f });
    cube.texture_uv.push_back({ 0.5f,1 / 3.0f });
    cube.texture_uv.push_back({ 2 / 4.0f,0 });
    cube.texture_uv.push_back({ 3 / 4.0f,1 / 3.0f });
    cube.texture_uv.push_back({ 0,1 / 3.0f });
    cube.texture_uv.push_back({ 1,1 / 3.0f });
    cube.texture_uv.push_back({ 0.25f,0 });

    return cube;
}


#endif