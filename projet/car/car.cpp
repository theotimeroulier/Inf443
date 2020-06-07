#include "car.hpp"


#include <random>


#ifdef SCENE_TOT


// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::

using namespace vcl;

mesh create_wheel(float R, float decal) {
    const size_t N = 40;

    mesh wheel; // temporary terrain storage (CPU only)
    wheel.position.resize(N * 2 + 2);
    wheel.texture_uv.resize(N * 2);
    for (size_t kv = 0; kv < N; ++kv)
    {
        wheel.position[2 * kv] = { R * std::cos(kv * 2.0f * 3.14f / N),0,R * std::sin(kv * 2.0f * 3.14f / N) };
        wheel.position[2 * kv + 1] = { R * std::cos(kv * 2.0f * 3.14f / N),decal,R * std::sin(kv * 2.0f * 3.14f / N) };
        wheel.texture_uv[2 * kv] = { std::cos(kv * 2.0f * 3.14f / N) ,std::sin(kv * 2.0f * 3.14f / N) };
    }

    wheel.position[N * 2] = { 0,0,0 };
    wheel.position[N * 2 + 1] = { 0,decal,0 };


    for (size_t kv = 0; kv < N * 2 + 1; ++kv)
    {
        const uint3 triangle_1 = { kv % (2 * N),(kv + 1) % (2 * N), (kv + 2) % (2 * N) };

        wheel.connectivity.push_back(triangle_1);
    }
    for (size_t kv = 0; kv < N; ++kv)
    {
        const uint3 triangle_1 = { 2 * kv,(2 * kv + 2) % (2 * N), 2 * N };
        const uint3 triangle_2 = { 2 * kv + 1,(2 * kv + 2 + 1) % (2 * N), 2 * N + 1 };
        wheel.connectivity.push_back(triangle_1);
        wheel.connectivity.push_back(triangle_2);
    }
    return wheel;
}

mesh create_triangle(float R) {
    mesh t;
    t.position.push_back(vec3(0, 0, 0));
    t.position.push_back(vec3(-0.01, 0, R * 0.9));
    t.position.push_back(vec3(0.01, 0, R * 0.9));

    const uint3 triangle_1 = { 0,1,2 };
    t.connectivity.push_back(triangle_1);

    return t;
}

mesh create_fond(float lon, float lar) {
    mesh fond;

    fond.position.push_back(vec3(-0.2 * lon, 0, 0));
    fond.position.push_back(vec3(1.2 * lon, 0, 0));
    fond.position.push_back(vec3(1.2 * lon, 0, lon / 2.0f));
    fond.position.push_back(vec3(1.2 * lon / 4.0f, 0, lon / 2.0f));
    fond.position.push_back(vec3(-0.2 * lon, 0, lon / 4.0f));

    fond.position.push_back(vec3(-0.2 * lon, lar, 0));
    fond.position.push_back(vec3(1.2 * lon, lar, 0));
    fond.position.push_back(vec3(1.2 * lon, lar, lon / 2.0f));
    fond.position.push_back(vec3(1.2 * lon / 4.0f, lar, lon / 2.0f));
    fond.position.push_back(vec3(-0.2 * lon, lar, lon / 4.0f));

    uint3 triangle = { 0,1,2 };
    fond.connectivity.push_back(triangle);
    triangle = { 0,2,4 };
    fond.connectivity.push_back(triangle);
    triangle = { 2,3,4 };
    fond.connectivity.push_back(triangle);
    triangle = { 5,6,7 };
    fond.connectivity.push_back(triangle);
    triangle = { 5,7,9 };
    fond.connectivity.push_back(triangle);
    triangle = { 7,8,9 };
    fond.connectivity.push_back(triangle);

    triangle = { 4,5,0 };
    fond.connectivity.push_back(triangle);
    triangle = { 4,5,9 };
    fond.connectivity.push_back(triangle);
    triangle = { 3,8,9 };
    fond.connectivity.push_back(triangle);
    triangle = { 3,9,4 };
    fond.connectivity.push_back(triangle);
    triangle = { 2,7,8 };
    fond.connectivity.push_back(triangle);
    triangle = { 2,3,8 };
    fond.connectivity.push_back(triangle);
    triangle = { 2,6,1 };
    fond.connectivity.push_back(triangle);
    triangle = { 2,6,7 };
    fond.connectivity.push_back(triangle);
    triangle = { 5,1,0 };
    fond.connectivity.push_back(triangle);
    triangle = { 5,1,6 };
    fond.connectivity.push_back(triangle);



    return fond;
}

mesh create_pare(float lon, float lar) {
    mesh pare;
    pare.position.push_back(lon * vec3(0, 0, 0.1 + 0.5 * 0));
    pare.position.push_back(lon * vec3(0.25, 0, 0.1 + 0.5 * 0.25));
    pare.position.push_back(vec3(0, lar - 0.02, lon * (0.1 + 0.5 * 0)));
    pare.position.push_back(vec3(lon * 0.25, lar - 0.02, lon * (0.1 + 0.5 * 0.25)));

    uint3 triangle = { 0,1,2 };
    pare.connectivity.push_back(triangle);
    triangle = { 1,2,3 };
    pare.connectivity.push_back(triangle);


    return pare;
}

vcl::hierarchy_mesh_drawable create_car() {
    vcl::hierarchy_mesh_drawable h;
    float ratio = 0.5f;
    const float decal = 0.02f * ratio;
    const float lon = 0.4f * ratio;
    const float lar = 0.2f * ratio;
    mesh_drawable tronc = create_fond(lon, lar);
    mesh_drawable wheel = create_wheel(0.06f * ratio, decal);
    mesh_drawable t = create_triangle(0.06f * ratio);
    mesh_drawable pare = create_pare(lon, lar);

    tronc.uniform.color = { 1.0f,0.0f,0.0f };
    wheel.uniform.color = { 0.0f,0.0f,0.0f };
    pare.uniform.color = { 1.0f,1.0f,1.0f };
    t.uniform.color = { 1.0f,1.0f,1.0f };


    h.add(tronc, "car");

    h.add(pare, "pare-brise", "car", { -0.001,+0.01f,0.001f + (0.25f) * lon });

    h.add(wheel, "wheel1", "car", { 0, lar - 0.001f - decal / 2,0 });
    h.add(t, "wheel10", "wheel1", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 0 * 6.28f / 6) });
    h.add(t, "wheel11", "wheel1", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 1 * 6.28f / 6) });
    h.add(t, "wheel12", "wheel1", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 2 * 6.28f / 6) });
    h.add(t, "wheel13", "wheel1", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 3 * 6.28f / 6) });
    h.add(t, "wheel14", "wheel1", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 4 * 6.28f / 6) });
    h.add(t, "wheel15", "wheel1", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 5 * 6.28f / 6) });
    h.add(t, "wheel10-", "wheel1", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 0 * 6.28f / 6) });
    h.add(t, "wheel11-", "wheel1", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 1 * 6.28f / 6) });
    h.add(t, "wheel12-", "wheel1", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 2 * 6.28f / 6) });
    h.add(t, "wheel13-", "wheel1", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 3 * 6.28f / 6) });
    h.add(t, "wheel14-", "wheel1", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 4 * 6.28f / 6) });
    h.add(t, "wheel15-", "wheel1", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 5 * 6.28f / 6) });

    h.add(wheel, "wheel2", "car", { 0, -0.001f - decal / 2,0 });
    h.add(t, "wheel20", "wheel2", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 0 * 6.28f / 6) });
    h.add(t, "wheel21", "wheel2", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 1 * 6.28f / 6) });
    h.add(t, "wheel22", "wheel2", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 2 * 6.28f / 6) });
    h.add(t, "wheel23", "wheel2", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 3 * 6.28f / 6) });
    h.add(t, "wheel24", "wheel2", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 4 * 6.28f / 6) });
    h.add(t, "wheel25", "wheel2", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 5 * 6.28f / 6) });
    h.add(t, "wheel20-", "wheel2", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 0 * 6.28f / 6) });
    h.add(t, "wheel21-", "wheel2", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 1 * 6.28f / 6) });
    h.add(t, "wheel22-", "wheel2", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 2 * 6.28f / 6) });
    h.add(t, "wheel23-", "wheel2", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 3 * 6.28f / 6) });
    h.add(t, "wheel24-", "wheel2", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 4 * 6.28f / 6) });
    h.add(t, "wheel25-", "wheel2", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 5 * 6.28f / 6) });

    h.add(wheel, "wheel3", "car", { lon,lar - 0.001f - decal / 2,0 });
    h.add(t, "wheel30", "wheel3", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 0 * 6.28f / 6) });
    h.add(t, "wheel31", "wheel3", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 1 * 6.28f / 6) });
    h.add(t, "wheel32", "wheel3", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 2 * 6.28f / 6) });
    h.add(t, "wheel33", "wheel3", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 3 * 6.28f / 6) });
    h.add(t, "wheel34", "wheel3", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 4 * 6.28f / 6) });
    h.add(t, "wheel35", "wheel3", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 5 * 6.28f / 6) });
    h.add(t, "wheel30-", "wheel3", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 0 * 6.28f / 6) });
    h.add(t, "wheel31-", "wheel3", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 1 * 6.28f / 6) });
    h.add(t, "wheel32-", "wheel3", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 2 * 6.28f / 6) });
    h.add(t, "wheel33-", "wheel3", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 3 * 6.28f / 6) });
    h.add(t, "wheel34-", "wheel3", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 4 * 6.28f / 6) });
    h.add(t, "wheel35-", "wheel3", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 5 * 6.28f / 6) });

    h.add(wheel, "wheel4", "car", { lon, -0.001f - decal / 2,0 });
    h.add(t, "wheel40", "wheel4", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 0 * 6.28f / 6) });
    h.add(t, "wheel41", "wheel4", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 1 * 6.28f / 6) });
    h.add(t, "wheel42", "wheel4", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 2 * 6.28f / 6) });
    h.add(t, "wheel43", "wheel4", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 3 * 6.28f / 6) });
    h.add(t, "wheel44", "wheel4", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 4 * 6.28f / 6) });
    h.add(t, "wheel45", "wheel4", { { 0,-0.001f,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 5 * 6.28f / 6) });
    h.add(t, "wheel40-", "wheel4", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 0 * 6.28f / 6) });
    h.add(t, "wheel41-", "wheel4", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 1 * 6.28f / 6) });
    h.add(t, "wheel42-", "wheel4", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 2 * 6.28f / 6) });
    h.add(t, "wheel43-", "wheel4", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 3 * 6.28f / 6) });
    h.add(t, "wheel44-", "wheel4", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 4 * 6.28f / 6) });
    h.add(t, "wheel45-", "wheel4", { { 0,0.001f + decal,0 },rotation_from_axis_angle_mat3({ 0,1,0 }, 5 * 6.28f / 6) });


    return h;
}



static size_t index_at_value(float t, vcl::buffer<vec3t> const& v)
{
    const size_t N = v.size();
    assert(v.size() >= 2);
    assert(t >= v[0].t);
    assert(t < v[N - 1].t);

    size_t k = 0;
    while (v[k + 1].t < t)
        ++k;
    return k;
}


static vec3 linear_interpolation(float t, float t1, float t2, const vec3& p1, const vec3& p2)
{
    const float alpha = (t - t1) / (t2 - t1);
    const vec3 p = (1 - alpha) * p1 + alpha * p2;

    return p;
}

static vec3 cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3, float mu)
{
    const float s = (t - t1) / (t2 - t1);
    const vec3 d1 = 2.0f * mu * (p2 - p0) / (t2 - t0);
    const vec3 d2 = 2.0f * mu * (p3 - p1) / (t3 - t1);
    const vec3 p = (2.0f * s * s * s - 3.0f * s * s + 1.0f) * p1 + (s * s * s - 2.0f * s * s + s) * d1 + (-2.0f * s * s * s + 3 * s * s) * p2 + (s * s * s - s * s) * d2;
    return p;

}

float get_angle_x(vec3 p1, vec3 p2, const gui_scene_structure& gui_scene, bool vol) { // larg voiture intervient

    if (!vol) {
        vec3 d = (p2 - p1) / norm(p2 - p1);
        vec2 u = vec2(d[1], -d[0]);
        const float dist = 0.2f; // larg
        const vec2 roue = vec2(p1[0] + u[0] * dist, p1[1] + u[1] * dist);
        const float altitude = evaluate_terrain_z(p1[0] / 20.0f + 0.5, p1[1] / 20.0f + 0.5, gui_scene) - evaluate_terrain_z(roue[0] / 20.0f + 0.5, roue[1] / 20.0f + 0.5, gui_scene);
        return -std::atan(altitude / dist);
    }
    return 0;
}
float get_angle_y(vec3 p1, vec3 p2, const gui_scene_structure& gui_scene, bool vol) { // lon voiture intervient

    if (!vol) {
        vec3 d = (p2 - p1) / norm(p2 - p1);
        vec2 u = vec2(-d[0], -d[1]);
        float dist = 0.4f; // lon
        const vec2 roue = vec2(p1[0] + u[0] * dist, p1[1] + u[1] * dist);
        const float altitude = evaluate_terrain_z(p1[0] / 20.0f + 0.5, p1[1] / 20.0f + 0.5, gui_scene) - evaluate_terrain_z(roue[0] / 20.0f + 0.5, roue[1] / 20.0f + 0.5, gui_scene);
        return std::atan(altitude / dist);
    }
    else {

        if ((p2[2] - p1[2]) == 0) {
            return 0;
        }

        float dist2 = norm(vec2(p1[0], p1[1]) - vec2(p2[0], p2[1]));
        float const a = (dist2) / (p2[2] - p1[2]);
        const float re = std::atan(a);

        if ((p2[2] - p1[2]) < 0) {
            return -re - 1.57;
        }
        return -re + 1.57;
    }
}
float get_angle_z(vec3 p1, vec3 p2) {
    if ((p2[0] - p1[0]) == 0) {
        if ((p2[1] - p1[1]) > 0) {
            return -1.57;
        }
        else {
            return 1.57;
        }
        // regrder si v vers le h o b
    }
    float const a = (p2[1] - p1[1]) / (p2[0] - p1[0]);
    const float re = std::atan(a);
    if ((p2[0] - p1[0]) < 0) { return re; }
    return re + 3.14;
}

void vole(vec3 p) {
    // ok
}

vcl::buffer<vec3t> create_keyframes_h(const gui_scene_structure& gui_scene, float u_s, float u_dest) {
    // float u_s = 0.5f; // start
    float v_s = 0.0f;
    // float u_dest = 0.22f; // destination (sur v on va le plus loin possible)

    float u_v = (u_s + u_dest) / 2.0f; // centre du cercle (entre u_s et u_dest )
    float v_v = 1.0f;
    float u_r = (u_s - u_dest) / 2.0f; // rayon vol entre 
    float v_r = 0.25f;



    vcl::buffer<vec3t> key;
    float N = 16;
    vec3t o;
    for (unsigned int k = 0; k < N + 1; k++) { // roule sur la route
        float v = k / N;
        o.p = { 20.0f * (u_s - 0.5f),20.0f * (v + v_s - 0.5f),evaluate_terrain_z(u_s,v + v_s,gui_scene) };
        o.t = v * k / 2.0f;
        key.push_back(o);

    }




    float z = evaluate_terrain_z(0.45f, 1.0f, gui_scene);
    N = N / 4;
    for (unsigned int k = 0; k < N + 1; k++) { // tourne en l'air
        float v = v_v + v_r * std::sin(k / N * 3.14f);
        float u = u_v + u_r * std::cos(k / N * 3.14f);
        o.p = { 20.0f * (u - 0.5f),20.0f * (v - 0.5f),z + k / N * 2.2f };
        o.t = k / 2.0f + 2 * N;
        key.push_back(o);
    }



    for (unsigned int k = 0; k < 7; k++) { // vole au dessus de la ville
        o.p = { 20.0f * (u_dest - 0.5f),20.0f * (v_v - 0.1f - 0.1f * k - 0.5f),z + 2.2f };
        o.t = 0.5f + k / 2.0f + 2.5f * N;
        key.push_back(o);
    }

    // ralenti
    o.p = { 20.0f * (u_dest - 0.5f),20.0f * (v_v - 0.1f - 0.1f * 7 - 0.5f),z + 2.2f };
    o.t = 3.5f + 1 + 2.5f * N;
    key.push_back(o);
    o.p = { 20.0f * (u_dest - 0.5f),20.0f * (v_v - 0.1f - 0.1f * 7.8f - 0.5f),z + 2.24f };
    o.t = 3.5f + 2.5f + 2.5f * N;
    key.push_back(o);

    // descend

    for (unsigned int k = 1; k < 8; k++) {
        o.p = { 20.0f * (u_dest - 0.5f),20.0f * (v_v - 0.1f - 0.1f * 7.8f - 0.004f * k - 0.5f),z + 2.24f - 0.2f / 6.0f * k };
        o.t = 6.0f + 0.3f * k + 2.5f * N;
        key.push_back(o);
    }

    for (unsigned int k = 1; k < 10; k++) { // posée en haut
        o.p = { 20.0f * (u_dest - 0.5f),20.0f * (v_v - 0.1f - 0.1f * 7.8f - 0.004f * 7 - 0.5f),z + 2.24f - 0.2f / 6.0f * 7 };
        o.t = 6.0f + 0.3f * 6 + 2.5f * N + k;
        key.push_back(o);
    }

    for (int k = 1; k < 10; k++) { // décole d'en haut
        o.p = { 20.0f * (u_dest - 0.5f),20.0f * (v_v - 0.1f - 0.1f * 7.8f - 0.004f * (7 + k) - 0.5f),z + 2.24f - 0.2f * (7.0f - k) / 6.0f };
        o.t = 6.0f + 0.3f * 6 + 2.5f * N + 9.0f + 0.3f * k;
        key.push_back(o);
    }




    for (unsigned int k = 0; k < N; k++) { // tourne en l'air
        float v = v_s - v_r * std::sin(k / N * 3.14f);
        float u = u_v - u_r * std::cos(k / N * 3.14f);
        o.p = { 20.0f * (u - 0.5f),20.0f * (v - 0.5f),z + (1 - k / N) * 2.2f };
        o.t = (k + 1) / 2.0f + 6.0f + 0.3f * 6 + 2.5f * N + 9.0f + 0.3f * 9;
        key.push_back(o);
    }

    o.p = { 20.0f * (u_s - 0.5f), -10.008f, 0.0172223f };
    o.t = 32.0f;
    key.push_back(o);
    o.p = { 20.0f * (u_s - 0.5f), -8.750000, 0.013073 };
    o.t = 32.5f;
    key.push_back(o);
    o.p = { 20.0f * (u_s - 0.5f) ,-7.500000 ,0.008556 };
    o.t = 33.0f;
    key.push_back(o);



    return key;
}

vcl::buffer<vec3t> create_keyframes_v(const gui_scene_structure& gui_scene, float v_s, float v_dest) {
    // float v_s = 0.465f; // start
    float u_s = 0.0f;

    // float v_dest = 0.21f; // destination (sur v on va le plus loin possible)

    float v_v = (v_s + v_dest) / 2.0f; // centre du cercle (entre v_s et v_dest )
    float u_v = 1.0f;
    float v_r = (v_s - v_dest) / 2.0f; // rayon vol entre 
    float u_r = 0.25f;



    vcl::buffer<vec3t> key;
    float N = 16;
    vec3t o;
    for (unsigned int k = 0; k < N + 1; k++) { // roule sur la route
        float u = k / N;
        o.p = { 20.0f * (u + u_s - 0.5f),20.0f * (v_s - 0.5f),evaluate_terrain_z(u_s + u,v_s,gui_scene) };
        o.t = u * k / 2.0f;
        key.push_back(o);

    }




    float z = evaluate_terrain_z(0.45f, 1.0f, gui_scene);
    N = N / 4;
    for (unsigned int k = 0; k < N + 1; k++) { // tourne en l'air
        float v = v_v + v_r * std::cos(k / N * 3.14f);
        float u = u_v + u_r * std::sin(k / N * 3.14f);
        o.p = { 20.0f * (u - 0.5f),20.0f * (v - 0.5f),z + k / N * 2.2f };
        o.t = k / 2.0f + 2 * N;
        key.push_back(o);
    }



    for (unsigned int k = 0; k < 7; k++) { // vole au dessus de la ville
        o.p = { 20.0f * (u_v - 0.1f - 0.1f * k - 0.5f),20.0f * (v_dest - 0.5f),z + 2.2f };

        o.t = 0.5f + k / 2.0f + 2.5f * N;
        key.push_back(o);
    }

    // ralenti
    o.p = { 20.0f * (u_v - 0.1f - 0.1f * 7 - 0.5f),20.0f * (v_dest - 0.5f),z + 2.2f };
    o.t = 3.5f + 1 + 2.5f * N;
    key.push_back(o);
    o.p = { 20.0f * (u_v - 0.1f - 0.1f * 7.8f - 0.5f),20.0f * (v_dest - 0.5f),z + 2.24f };
    o.t = 3.5f + 2.5f + 2.5f * N;
    key.push_back(o);

    // descend

    for (unsigned int k = 1; k < 8; k++) {
        o.p = { 20.0f * (u_v - 0.1f - 0.1f * 7.8f - 0.004f * k - 0.5f),20.0f * (v_dest - 0.5f),z + 2.24f - 0.2f / 6.0f * k };
        o.t = 6.0f + 0.3f * k + 2.5f * N;
        key.push_back(o);
    }

    for (unsigned int k = 1; k < 10; k++) { // posée en haut
        o.p = { 20.0f * (u_v - 0.1f - 0.1f * 7.8f - 0.004f * 7 - 0.5f),20.0f * (v_dest - 0.5f),z + 2.24f - 0.2f / 6.0f * 7 };
        o.t = 6.0f + 0.3f * 6 + 2.5f * N + k;
        key.push_back(o);
    }

    for (int k = 1; k < 10; k++) { // décole d'en haut
        o.p = { 20.0f * (u_v - 0.1f - 0.1f * 7.8f - 0.004f * (7 + k) - 0.5f),20.0f * (v_dest - 0.5f),z + 2.24f - 0.2f * (7.0f - k) / 6.0f };
        o.t = 6.0f + 0.3f * 6 + 2.5f * N + 9.0f + 0.3f * k;
        key.push_back(o);
    }




    for (unsigned int k = 0; k < N; k++) { // tourne en l'air
        float v = v_v - v_r * std::cos(k / N * 3.14f);
        float u = u_s - u_r * std::sin(k / N * 3.14f);
        o.p = { 20.0f * (u - 0.5f),20.0f * (v - 0.5f),z + (1 - k / N) * 2.2f };
        o.t = (k + 1) / 2.0f + 6.0f + 0.3f * 6 + 2.5f * N + 9.0f + 0.3f * 9;
        key.push_back(o);
    }

    o.p = { -10.0f, 20.0f * (v_s - 0.5f), 0.0172223f };
    o.t = 32.0f;
    key.push_back(o);
    o.p = { -8.75f, 20.0f * (v_s - 0.5f), 0.013073 };
    o.t = 32.5f;
    key.push_back(o);
    o.p = { -6.25f,20.0f * (v_s - 0.5f) ,0.008556 };
    o.t = 33.0f;
    key.push_back(o);



    return key;
}

std::vector<vcl::buffer<vec3t>> create_car_pos(const gui_scene_structure& gui_scene) {
    std::vector<vcl::buffer<vec3t>> car_pos;
    vcl::buffer<vec3t> o;
    o = create_keyframes_h(gui_scene, 0.5f, 0.22f);
    car_pos.push_back(o);
    o = create_keyframes_v(gui_scene, 0.465f, 0.21f);
    car_pos.push_back(o);
    return car_pos;
}

void draw_voiture_h(float t, vcl::buffer<vec3t>& keyframes, float t_vol, bool vol, const gui_scene_structure& gui_scene, scene_structure& scene, vcl::hierarchy_mesh_drawable& car) {
    const int idx = index_at_value(t, keyframes);

    // Assume a closed curve trajectory
    const size_t N = keyframes.size();


    // Preparation of data for the linear interpolation
    // Parameters used to compute the linear interpolation
    const float t0 = keyframes[idx - 1].t; // = t_i-1
    const float t1 = keyframes[idx].t; // = t_i
    const float t2 = keyframes[idx + 1].t; // = t_{i+1}
    const float t3 = keyframes[idx + 2].t; // = t_i+2

    const vec3& p0 = keyframes[idx - 1].p; // = p_i-1
    const vec3& p1 = keyframes[idx].p; // = p_i
    const vec3& p2 = keyframes[idx + 1].p; // = p_{i+1}
    const vec3& p3 = keyframes[idx + 2].p; // = p_i+2


    // Compute the linear interpolation here
    // const vec3 p = linear_interpolation(t,t1,t2,p1,p2);
    // const vec3 p_apres = linear_interpolation(t + 0.1f,  t1, t2, p1, p2);

    // Create and call a function cardinal_spline_interpolation(...) instead
    vec3 p = cardinal_spline_interpolation(t, t0, t1, t2, t3, p0, p1, p2, p3, 0.4f);
    vec3 p_apres = cardinal_spline_interpolation(t + 0.1f, t0, t1, t2, t3, p0, p1, p2, p3, 0.4f);


    vole(p);

    if (t > t_vol) { vol = true; } // changer
    if (t > 32.0f) { vol = false; } // changer


    if (!vol) {
        p = vec3(p[0], p[1], evaluate_terrain_z(p[0] / 20.0f + 0.5, p[1] / 20.0f + 0.5, gui_scene)) + vec3(0, 0, 0.06f + 0.005f);
        p_apres = vec3(p_apres[0], p_apres[1], evaluate_terrain_z(p_apres[0] / 20.0f + 0.5, p_apres[1] / 20.0f + 0.5, gui_scene)) + vec3(0, 0, 0.06f + 0.005f);
    }

    // car
    mat3 R_tourner_x_w = rotation_from_axis_angle_mat3({ 0,0,1 }, 0);
    mat3 R_tourner_y_w = rotation_from_axis_angle_mat3({ 0,1,0 }, t * 6);

    if (vol && t > t_vol) {  //en vol les roues tournent pas et sont a 90°
        R_tourner_x_w = rotation_from_axis_angle_mat3({ 1,0,0 }, 6.28f / 4.0f);
        R_tourner_y_w = rotation_from_axis_angle_mat3({ 0,1,0 }, 0);
    }
    if (vol && t > t_vol&& t < t_vol + 1) { // la voiture s'envole
        R_tourner_x_w = rotation_from_axis_angle_mat3({ 1,0,0 }, (t - t_vol) * 6.28f / 4.0f);
        R_tourner_y_w = rotation_from_axis_angle_mat3({ 0,1,0 }, 0);
    }

    float angle_z = get_angle_z(p, p_apres);
    float angle_y = get_angle_y(p, p_apres, gui_scene, vol);
    float angle_x = get_angle_x(p, p_apres, gui_scene, vol);
    if (t > 15 && t < 30) { angle_y = 0; angle_z = 1.57f; }
    mat3 const R_tourner_z = rotation_from_axis_angle_mat3({ 0,0,1 }, angle_z);
    mat3 const R_tourner_y = rotation_from_axis_angle_mat3({ 0,1,0 }, angle_y);
    mat3 const R_tourner_x = rotation_from_axis_angle_mat3({ 1,0,0 }, angle_x);


    car["car"].transform.rotation = R_tourner_z * R_tourner_y * R_tourner_x;

    car["wheel1"].transform.rotation = R_tourner_y_w * R_tourner_x_w;
    car["wheel2"].transform.rotation = R_tourner_y_w * R_tourner_x_w;
    car["wheel3"].transform.rotation = R_tourner_y_w * R_tourner_x_w;
    car["wheel4"].transform.rotation = R_tourner_y_w * R_tourner_x_w;


    car["car"].transform.translation = p;
    car.update_local_to_global_coordinates();
    draw(car, scene.camera);
}

void draw_voiture_v(float t, vcl::buffer<vec3t>& keyframes, float t_vol, bool vol, const gui_scene_structure& gui_scene, scene_structure& scene, vcl::hierarchy_mesh_drawable& car) {
    const int idx = index_at_value(t, keyframes);

    // Assume a closed curve trajectory
    const size_t N = keyframes.size();


    // Preparation of data for the linear interpolation
    // Parameters used to compute the linear interpolation
    const float t0 = keyframes[idx - 1].t; // = t_i-1
    const float t1 = keyframes[idx].t; // = t_i
    const float t2 = keyframes[idx + 1].t; // = t_{i+1}
    const float t3 = keyframes[idx + 2].t; // = t_i+2

    const vec3& p0 = keyframes[idx - 1].p; // = p_i-1
    const vec3& p1 = keyframes[idx].p; // = p_i
    const vec3& p2 = keyframes[idx + 1].p; // = p_{i+1}
    const vec3& p3 = keyframes[idx + 2].p; // = p_i+2


    // Compute the linear interpolation here
    // const vec3 p = linear_interpolation(t,t1,t2,p1,p2);
    // const vec3 p_apres = linear_interpolation(t + 0.1f,  t1, t2, p1, p2);

    // Create and call a function cardinal_spline_interpolation(...) instead
    vec3 p = cardinal_spline_interpolation(t, t0, t1, t2, t3, p0, p1, p2, p3, 0.4f);
    vec3 p_apres = cardinal_spline_interpolation(t + 0.1f, t0, t1, t2, t3, p0, p1, p2, p3, 0.4f);


    vole(p);

    if (t > t_vol) { vol = true; } // changer
    if (t > 32.0f) { vol = false; } // changer


    if (!vol) {
        p = vec3(p[0], p[1], evaluate_terrain_z(p[0] / 20.0f + 0.5, p[1] / 20.0f + 0.5, gui_scene)) + vec3(0, 0, 0.06f + 0.005f);
        p_apres = vec3(p_apres[0], p_apres[1], evaluate_terrain_z(p_apres[0] / 20.0f + 0.5, p_apres[1] / 20.0f + 0.5, gui_scene)) + vec3(0, 0, 0.06f + 0.005f);
    }

    // car
    mat3 R_tourner_x_w = rotation_from_axis_angle_mat3({ 0,0,1 }, 0);
    mat3 R_tourner_y_w = rotation_from_axis_angle_mat3({ 0,1,0 }, t * 6);

    if (vol && t > t_vol) {  //en vol les roues tournent pas et sont a 90°
        R_tourner_x_w = rotation_from_axis_angle_mat3({ 1,0,0 }, 6.28f / 4.0f);
        R_tourner_y_w = rotation_from_axis_angle_mat3({ 0,1,0 }, 0);
    }
    if (vol && t > t_vol&& t < t_vol + 1) { // la voiture s'envole
        R_tourner_x_w = rotation_from_axis_angle_mat3({ 1,0,0 }, (t - t_vol) * 6.28f / 4.0f);
        R_tourner_y_w = rotation_from_axis_angle_mat3({ 0,1,0 }, 0);
    }

    float angle_z = get_angle_z(p, p_apres);
    float angle_y = get_angle_y(p, p_apres, gui_scene, vol);
    float angle_x = get_angle_x(p, p_apres, gui_scene, vol);
    if (t > 15 && t < 30) { angle_y = 0; angle_z = 0.0f; }
    mat3 const R_tourner_z = rotation_from_axis_angle_mat3({ 0,0,1 }, angle_z);
    mat3 const R_tourner_y = rotation_from_axis_angle_mat3({ 0,1,0 }, angle_y);
    mat3 const R_tourner_x = rotation_from_axis_angle_mat3({ 1,0,0 }, angle_x);


    car["car"].transform.rotation = R_tourner_z * R_tourner_y * R_tourner_x;

    car["wheel1"].transform.rotation = R_tourner_y_w * R_tourner_x_w;
    car["wheel2"].transform.rotation = R_tourner_y_w * R_tourner_x_w;
    car["wheel3"].transform.rotation = R_tourner_y_w * R_tourner_x_w;
    car["wheel4"].transform.rotation = R_tourner_y_w * R_tourner_x_w;


    car["car"].transform.translation = p;
    car.update_local_to_global_coordinates();
    draw(car, scene.camera);
}

void draw_cars(float t, float t_vol, bool vol, const gui_scene_structure& gui_scene, scene_structure& scene, vcl::hierarchy_mesh_drawable& car, std::vector<vcl::buffer<vec3t>>& car_pos) {
    int N = car_pos.size();
    for (int k = 0; k < N / 2; k++) {
        draw_voiture_h(t, car_pos[k], t_vol, vol, gui_scene, scene, car);
        draw_voiture_v(t, car_pos[N / 2 + k], t_vol, vol, gui_scene, scene, car);
    }

}

#endif