#pragma once
#include "girafe.hpp"
//  rouge : x bleu : z vert y

#ifdef SCENE_TOT

using namespace vcl;
float r_corps=0.3;
float l_corps=1.0;
float h_cou=2.5;
float r_cou=0.1;
float r_tete=0.2;
float r_pate = 0.1;
float l_pate=0.8;

mesh create_cou_ane(float r, float height)
{
    return mesh_primitive_cylinder(r, { 0,0,0 }, { ::l_corps/5,0,height });
}
mesh create_corps_ane(float r, float length)
{
    // Number of samples
    mesh m;
    m.push_back(mesh_primitive_cylinder(r, { 0,0,0 }, { length,0,0 }));
    m.push_back(mesh_primitive_sphere(r*0.97, { 0,0,0 }));
    m.push_back(mesh_primitive_sphere(r*0.97, { length,0,0 }));

    return m;

}

vcl::hierarchy_mesh_drawable create_giraffe() {

    vcl::hierarchy_mesh_drawable hierarchy;
    GLuint texture_girafe;

    texture_girafe = create_texture_gpu(image_load_png("scenes/3D_GRAPHICS/projet/assets/giraffe.png"));


    mesh create_cou_ane(float r, float height);
    mesh create_corps_ane(float r, float length);


    // body
    mesh_drawable body = mesh_drawable(create_corps_ane(r_corps,l_corps));
    body.texture_id = texture_girafe;
    // yeux
    mesh_drawable tete = mesh_drawable(mesh_primitive_sphere(r_tete));
    float l_t = l_corps * 0.8f + l_corps / 5;
    tete.uniform.transform.translation = { l_t,0.0f,h_cou };
    mesh_drawable eye = mesh_drawable(mesh_primitive_sphere(0.05f, { l_t,0,h_cou }, 20, 20));
    eye.uniform.color = { 0,0,0 };



    mesh_drawable pate_h = mesh_drawable(mesh_primitive_cylinder(r_pate, { 0,0,0 }, { 0,0,-l_pate / 2 }));
    pate_h.uniform.transform.translation = { 0,0,-r_corps * 0.7f };
    mesh_drawable genou = mesh_drawable(mesh_primitive_sphere(r_pate*1.1f));
    mesh_drawable pate_b = mesh_drawable(mesh_primitive_cylinder(r_pate, { 0,0,0 }, { 0,0,-l_pate/2 }));





    //cou
    mesh_drawable cou = create_cou_ane(r_cou,h_cou);
    cou.uniform.transform.translation = {l_corps*0.8f,0,0};

    // Build the hierarchy:
    // Syntax to add element
    // hierarchy.add(visual_element, element_name, parent_name, (opt)[translation, rotation])
    hierarchy.add(body, "body");

    // Eyes positions are set with respect to some ratio of the
    hierarchy.add(cou, "cou", "body");

    hierarchy.add(tete, "tete", "cou");

    hierarchy.add(eye, "eye_left", "tete",r_tete * vec3( 1/2.0f, -1/2.0f, 1/1.5f));
    hierarchy.add(eye, "eye_right", "tete",{r_tete*0.5f,r_tete/2.0f,r_tete*2/3.0f});


    // Set the left part of the body arm: shoulder-elbow-arm


    //pates
    //arriere droite
    hierarchy.add(pate_h, "pate_h_ad", "body",vec3(0, r_corps/3, 0));
    hierarchy.add(genou, "genou_ad", "pate_h_ad", vec3({0,0,-r_corps * 0.7f -l_pate/2}));
    hierarchy.add(pate_b, "pate_b_ad", "genou_ad");

    //arriere gauche
    hierarchy.add(pate_h, "pate_h_ag", "body", vec3(0, -r_corps / 3, 0));
    hierarchy.add(genou, "genou_ag", "pate_h_ag", vec3({ 0,0,-r_corps * 0.7f - l_pate / 2 }));
    hierarchy.add(pate_b, "pate_b_ag", "genou_ag");

    //avant droite
    hierarchy.add(pate_h, "pate_h_fd", "body",vec3(0.8f*l_corps, r_corps / 3, 0));
    hierarchy.add(genou, "genou_fd", "pate_h_fd", vec3({ 0,0,-r_corps * 0.7f - l_pate / 2 }));
    hierarchy.add(pate_b, "pate_b_fd", "genou_fd");

    //avant gauche
    hierarchy.add(pate_h, "pate_h_fg", "body", vec3(0.8f * l_corps, -r_corps / 3, 0));
    hierarchy.add(genou, "genou_fg", "pate_h_fg", vec3({ 0,0,-r_corps * 0.7f - l_pate / 2 }));
    hierarchy.add(pate_b, "pate_b_fg", "genou_fg");


    return hierarchy;
}
void draw_giraffe(std::map<std::string, GLuint>& shaders, scene_structure& scene, const gui_scene_structure& gui_scene, float t, vcl::hierarchy_mesh_drawable& hierarchy, std::vector<vec3t> keyframes)
{
    /** *************************************************************  **/
    /** Compute the (animated) transformations applied to the elements **/
    /** *************************************************************  **/
    mat3 const Symmetry = { -1,0,0, 0,1,0, 0,0,1 };
    mat3 const Identity = { 1,0,0, 0,1,0, 0,0,1 };
    mat3 R_pate_h0 = rotation_from_axis_angle_mat3({ 0,1,0 }, 0.2 * std::sin(2 * 3.14 * t));
    mat3 R_pate_h1 = rotation_from_axis_angle_mat3({ 0,1,0 }, -0.2 * std::sin(2 * 3.14 * t));
    mat3 R_pate_h2 = rotation_from_axis_angle_mat3({ 0,1,0 }, -0.2 * std::sin(2 * 3.14 * t));
    mat3 R_pate_h3 = rotation_from_axis_angle_mat3({ 0,1,0 }, 0.2 * std::sin(2 * 3.14 * t));
    mat3 R_pate_b = (rotation_from_axis_angle_mat3({ 0,1,0 }, 0.2 * std::sin(2 * 3.14 * t + 0.3)) + 0.2 * rotation_from_axis_angle_mat3({ 0,1,0 }, 1.0f)) / sqrt(1.5);
    hierarchy["pate_h_ag"].transform.rotation = R_pate_h0;
    hierarchy["pate_h_ad"].transform.rotation = R_pate_h1;
    hierarchy["pate_h_fg"].transform.rotation = R_pate_h2;
    hierarchy["pate_h_fd"].transform.rotation = R_pate_h3;
    hierarchy["pate_b_ag"].transform.rotation = R_pate_b;
    hierarchy["pate_b_ad"].transform.rotation = R_pate_b;
    hierarchy["pate_b_fg"].transform.rotation = R_pate_b;
    hierarchy["pate_b_fd"].transform.rotation = R_pate_b;
    hierarchy.update_local_to_global_coordinates();
    //trajectoire
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
    vec3 p = cardinal_spline_interpolation(t, t0, t1, t2, t3, p0, p1, p2, p3, 0.4f);
    vec3 p_after = cardinal_spline_interpolation(t + 0.1f, t0, t1, t2, t3, p0, p1, p2, p3, 0.4f);
    float angle_z = get_angle_z(p, p_after);
    mat3 const R_tourner_z = rotation_from_axis_angle_mat3({ 0,0,1 }, angle_z + 3.14f);
    hierarchy["body"].transform.translation = p;
    hierarchy["body"].transform.rotation = R_tourner_z;
    draw(hierarchy, scene.camera);
}
std::vector<vec3t> create_keyframes_g(float x0, float y0) {
    std::vector<vec3t> keyframe_giraffe;
    float t = 0;
    if (0 < x0 < 11 && y0 < -1) {
        y0 = -11;
        if (x0 > 10) {
            x0 = 10;
        }
        for (float x = x0; x < 12; ++x) {
            float y = y0;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float y = -10; y < 11; ++y) {
            float x = 11;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float x = 9; x > -12; --x) {
            float y = 10;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float y = 9; y > -12; --y) {
            float x = -11;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float x = -10; x < x0 + 3; x++) {
            float y = -11;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
    }
    if (0 < x0 && -1 < y0 < 11) {
        if (y0 > 10) {
            y0 = 10;
        }
        for (float y = y0; y < 11; ++y) {
            float x = 11;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float x = 9; x > -12; --x) {
            float y = 10;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float y = 9; y > -12; --y) {
            float x = -11;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float x = -11; x < 11; ++x) {
            float y = -11;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float y = -10; y < y0 + 3; ++y) {
            float x = 10;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
    }
    if (x0<1 && y0>-1) {
        y0 = 10;
        if (x0 < -11) {
            x0 = -11;
        }
        for (float x = x0; x > -12; --x) {
            float y = y0;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float y = 9; y > -12; --y) {
            float x = -11;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float x = -10; x < 11; ++x) {
            float y = -11;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float y = -10; y < 11; ++y) {
            float x = 10;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float x = 9; x > x0 - 3; --x) {
            float y = 10;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
    }
    if (x0 < 1 && y0 < -1) {
        x0 = -11;
        if (y0 < -11) {
            y0 = -11;
        }
        for (float y = y0; y > -12; --y) {
            float x = x0;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float x = -10; x < 11; ++x) {
            float y = -11;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float y = -10; y < 11; ++y) {
            float x = 10;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float x = 9; x > -12; --x) {
            float y = 10;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
        for (float y = 9; y > y0 - 3; --y) {
            float x = x0;
            float u = x / 60.0f + 0.5f;
            float v = y / 60.0f + 0.5f;
            const float height = evaluate_terrain_z_T(u, v);
            const float z_y = evaluate_terrain_z_y(u, v);
            const float z_X = evaluate_terrain_z_X(u, v);
            const float z_Y = evaluate_terrain_z_Y(u, v);
            float z = -5 * atan(0.5f * (x - 27) + height) + 5 * atan(0.5f * (x + 13) + z_X) - 5 * atan(0.5f * (y - 28) + z_y) + 5 * atan(0.5f * (y + 13) + z_Y) + 0.8f;
            keyframe_giraffe.push_back({ {x,y,z},t });
            t++;
        }
    }
    return keyframe_giraffe;
}
void init_time(  vcl::timer_interval& timer, std::vector<vec3t> &keyframe_giraffe_2){
    timer.t_min = keyframe_giraffe_2[1].t;                   // first time of the keyframe
    timer.t_max = keyframe_giraffe_2[keyframe_giraffe_2.size() - 2].t;  // last time of the keyframe
    timer.t = timer.t_min;

}

void init_giraffe(vcl::hierarchy_mesh_drawable& giraffe, std::vector<std::vector<vec3t>>& keyframe_giraffe, vcl::timer_interval& timer_giraffe, vcl::timer_interval& timer_giraffe_2,  vcl::timer_interval& timer_giraffe_3, vcl::timer_interval& timer_giraffe_4, vcl::timer_interval& timer_giraffe_5, std::map<std::string, GLuint>& shaders) {
    giraffe = create_giraffe();
    giraffe.set_shader_for_all_elements(shaders["mesh"]);

    keyframe_giraffe.push_back(create_keyframes_g(-11, -10));
    keyframe_giraffe.push_back(create_keyframes_g(-4, 6));
    keyframe_giraffe.push_back(create_keyframes_g(3, -6));
    keyframe_giraffe.push_back(create_keyframes_g(1, -7));
    keyframe_giraffe.push_back(create_keyframes_g(3, 9));

    init_time(timer_g, keyframe_giraffe[0]);
    init_time(timer_g_2, keyframe_giraffe[1]);
    init_time(timer_g_3, keyframe_giraffe[2]);
    init_time(timer_g_4, keyframe_giraffe[3]);
    init_time(timer_g_5, keyframe_giraffe[4]);
}

void draw_giraffes(vcl::hierarchy_mesh_drawable& giraffe, std::vector<std::vector<vec3t>>& keyframe_giraffe, vcl::timer_interval& timer_giraffe, vcl::timer_interval& timer_giraffe_2,  vcl::timer_interval& timer_giraffe_3, vcl::timer_interval& timer_giraffe_4, vcl::timer_interval& timer_giraffe_5, std::map<std::string, GLuint>& shaders, scene_structure& scene, const gui_scene_structure& gui_scene) {
    timer_g.update();
    float t_g = timer_g.t;
    timer_g_2.update();
    float t_g_2 = timer_g.t;
    timer_g_3.update();
    float t_g_3 = timer_g.t;
    timer_g_4.update();
    float t_g_4 = timer_g.t;
    timer_g_5.update();
    float t_g_5 = timer_g.t;

    draw_giraffe(shaders, scene, gui_scene, t_g, giraffe, keyframe_giraffe[0]);
    draw_giraffe(shaders, scene, gui_scene, t_g_2, giraffe, keyframe_giraffe[1]);
    draw_giraffe(shaders, scene, gui_scene, t_g_3, giraffe, keyframe_giraffe[2]);
    draw_giraffe(shaders, scene, gui_scene, t_g_4, giraffe, keyframe_giraffe[3]);
    draw_giraffe(shaders, scene, gui_scene, t_g_5, giraffe, keyframe_giraffe[4]);
}

#endif


