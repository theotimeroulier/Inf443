
#include "park.hpp"


#include <random>
#ifdef SCENE_TOT

float u0 = 0.3;
float v0 = 0.4;



// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;

float evaluate_terrain_2_z(float u, float v, const gui_scene_structure& gui_scene) {
    float height = 0.15f;
    float scaling = 2.0f;
    int octave = 8;
    float persistency = 0.25f;


    // Evaluate Perlin noise
    const float noise = perlin(scaling * u, scaling * v, octave, persistency);
    float z = 0;
    std::vector<vcl::vec2> p({ {0.5,0.4},{0.7,0.6},{0.3,0.7},{0.5,0.7},{0.3,0.3},{0.3,0.5} });
    std::vector<float> h({ 0.2,0.2,0.3,0.2,0.5,0.4 });

    for (int i = 0; i < h.size(); ++i) {
        h[i] = 0.5 * h[i];
    }
    std::vector<float> s({ 0.05,0.02,0.08,0.1,0.04,0.02 });
    for (int i = 0; i < 6; ++i) {
        const float ni = norm(vec2(u, v) - p[i]);
        z += h[i] * std::exp(-(ni * ni) / s[i]) + height * noise;

    }
    double d = sqrt((u - u0) * (u - u0) + (v - v0) * (v - v0));
    if (d <= 0.3 && d > 0.1)
        z = z * (d - 0.1) / 0.2;
    else if (d <= 0.1)
        z = 0;
    return z;
}

vec3 evaluate_park(int v_larg, int h_high, const gui_scene_structure& gui_scene, float u, float v) {

    return { 20 * (u * 2 * v_larg / 100.0f + v_larg / 100.0f - 0.5f), 20 * (v * 2 * h_high / 100.0f + h_high / 100.0f - 0.5f),
        evaluate_terrain_2_z(u, v, gui_scene) + evaluate_terrain_z(u,v,gui_scene) +0.1f};

}

vec3 evaluate_lac(int v_larg, int h_high, const gui_scene_structure& gui_scene, float u, float v) {
    return { 20 * (u * 2 * v_larg / 100.0f + v_larg / 100.0f - 0.5f), 20 * (v * 2 * h_high / 100.0f + h_high / 100.0f - 0.5f),
         0.3f + evaluate_terrain_z(u,v,gui_scene) };

}

mesh create_park(int v_larg, int h_high, float u0, float v0, const gui_scene_structure& gui_scene) {
    mesh park;
    int N = 20;
    park.position.resize(N * N);
    park.connectivity.resize(N * N);
    park.texture_uv.resize(N * N);

    for (size_t ku = 0; ku < N; ++ku)
    {
        for (size_t kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);

            // Compute coordinates
            park.position[kv + N * ku] = evaluate_park(v_larg, h_high, gui_scene, u, v);
            park.texture_uv[kv + N * ku] = { u * 3 ,v * 3 };
        }
    }

    const unsigned int Ns = N;
    for (unsigned int ku = 0; ku < Ns - 1; ++ku)
    {
        for (unsigned int kv = 0; kv < Ns - 1; ++kv)
        {
            const unsigned int idx = kv + N * ku; // current vertex offset

            const uint3 triangle_1 = { idx, idx + 1 + Ns, idx + 1 };
            const uint3 triangle_2 = { idx, idx + Ns, idx + 1 + Ns };

            park.connectivity.push_back(triangle_1);
            park.connectivity.push_back(triangle_2);
        }
    }

    return park;



}
mesh create_lac(int v_larg, int h_high, float u0, float v0, const gui_scene_structure& gui_scene) {
    mesh lac;
    int N = 20;
    lac.position.resize(N * N);
    lac.connectivity.resize(N * N);
    lac.texture_uv.resize(N * N);

    for (size_t ku = 0; ku < N; ++ku)
    {
        for (size_t kv = 0; kv < N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku / (N - 1.0f);
            const float v = kv / (N - 1.0f);

            // Compute coordinates
            if (std::abs(u - u0) <= 0.2 && std::abs(v - v0 )<= 0.2) {
                lac.position[kv + N * ku] = evaluate_lac(v_larg, h_high, gui_scene, u, v);
                lac.texture_uv[kv + N * ku] = { u * 3 ,v * 3 };
            }
        }
    }

    const unsigned int Ns = N;
    for (unsigned int ku = 0; ku < Ns - 1; ++ku)
    {
        for (unsigned int kv = 0; kv < Ns - 1; ++kv)
        {
            const unsigned int idx = kv + N * ku; // current vertex offset

            const uint3 triangle_1 = { idx, idx + 1 + Ns, idx + 1 };
            const uint3 triangle_2 = { idx, idx + Ns, idx + 1 + Ns };

            lac.connectivity.push_back(triangle_1);
            lac.connectivity.push_back(triangle_2);
        }
    }

    return lac;

}

std::vector<vcl::vec3> create_park_pos(int v_larg, int h_high, const gui_scene_structure& gui_scene, std::vector<vcl::vec2>& o_2) {
    std::vector<vcl::vec3> park_pos;
    for (vec2 o : o_2) {
        park_pos.push_back(vec3(20 * (v_larg * 3 * o[0] / 100.0f), 20 * (h_high * 3 * o[1] / 100.0f), 0));
    }
    return park_pos;
}

std::vector<vcl::vec2> create_park_pos_2() {
    std::vector<vcl::vec2> o_2;
    o_2.push_back(vec2(4, 4));
    o_2.push_back(vec2(7, 5));
    o_2.push_back(vec2(1, 2));
    return o_2;
}




std::vector<vcl::vec3> create_tree_pos(int v_larg, int h_high, const gui_scene_structure& gui_scene, std::vector<vcl::vec2>& o_2) {
    std::vector<vcl::vec3> tree_pos;
    for (vec2 p : o_2) {
        for (unsigned int k = 0; k < 2; k++) {
            float u = rand_interval(); //u0 
            float v = rand_interval();  //v0
            vcl::vec3 m = evaluate_park(v_larg, h_high, gui_scene, u, v) + vec3(20 * (v_larg * 3 * p[0] / 100.0f), 20 * (h_high * 3 * p[1] / 100.0f), +0.1f);
            bool ajout = true;
            for (vcl::vec3 l : tree_pos) {
                if (norm(l - m) < 0.3 || std::abs(u - u0) <= 0.2 && std::abs(v - v0 <= 0.2)) { ajout = false; }
            }
            if (ajout) {
                tree_pos.push_back(m);
            }

        }
    }
    return tree_pos;
}

vcl::hierarchy_mesh_drawable create_tree_simple() {
    float ratio = 1.5f;
    vcl::hierarchy_mesh_drawable tree;
    mesh_drawable tronc = mesh_drawable(mesh_primitive_cylinder(0.05f * ratio, { 0,0,0 }, { 0,0,0.4f * ratio }));
    mesh_drawable cime = mesh_drawable(mesh_primitive_cone(0.15f * ratio, { 0,0,0 }, { 0,0,0.1f * ratio }));
    tronc.texture_id = create_texture_gpu(image_load_png("scenes/projet/assets/ecorce.png"));
    cime.texture_id = create_texture_gpu(image_load_png("scenes/projet/assets/feuillage.png"));
    tree.add(tronc, "tronc");
    tree.add(cime, "cime1", "tronc", { 0,0,0.2f * ratio });
    tree.add(cime, "cime2", "tronc", { 0,0,0.3f * ratio });
    tree.add(cime, "cime3", "tronc", { 0,0,0.4f * ratio });
    return tree;
}


vcl::hierarchy_mesh_drawable create_tree() {
    float ratio = 1.0f;
    GLint text_arbre = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/ecorce.png"));
    GLint text_feuillage = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/feuillage.png"));
    vcl::hierarchy_mesh_drawable tree;
    mesh_drawable tronc = mesh_drawable(mesh_primitive_cylinder(0.05f * ratio, { 0,0,0 }, { 0,0,0.4f * ratio }));
    tronc.texture_id = text_arbre;

    tree.add(tronc, "tronc0-0");
    for (int k = 1; k < 5; k++) {
        for (int j = 0; j < pow(4, k - 1); j++) {
            float x = 0.12f;
            tronc = mesh_drawable(mesh_primitive_cylinder(0.05f * ratio / float(pow(1.7f, k)), { 0,0,0 }, { 0,0,0.4f * ratio / float(pow(1.7f,k)) }));
            tronc.texture_id = text_arbre;
            if (k > 0) { tronc.texture_id = text_feuillage; }

            std::string x_l = "tronc" + std::to_string(k) + "-" + std::to_string(4 * j);
            std::string x_r = "tronc" + std::to_string(k) + "-" + std::to_string(4 * j + 1);
            std::string parent = "tronc" + std::to_string(k - 1) + "-" + std::to_string(j);
            tree.add(tronc, x_l, parent, { { 0,0,0.4f * ratio / float(pow(1.7f,k - 1)) },rotation_from_axis_angle_mat3({ 0,1,0 }, 2 * 3.14f * x) });
            tree.add(tronc, x_r, parent, { { 0,0,0.4f * ratio / float(pow(1.7f,k - 1)) }, rotation_from_axis_angle_mat3({ 0,1,0 }, -2 * 3.14f * x) });
            
            std::string x_l_2 = "tronc" + std::to_string(k) + "-" + std::to_string(4 * j + 2);
            std::string x_r_2 = "tronc" + std::to_string(k) + "-" + std::to_string(4 * j + 3);

            tree.add(tronc, x_l_2, parent, { { 0,0,0.4f * ratio / float(pow(1.7f,k - 1)) },rotation_from_axis_angle_mat3({ 1,0,0 }, 2 * 3.14f * x) });
            tree.add(tronc, x_r_2, parent, { { 0,0,0.4f * ratio / float(pow(1.7f,k - 1)) }, rotation_from_axis_angle_mat3({ 1,0,0 }, -2 * 3.14f * x) });
            
        }
    }

    return tree;
}

void init_park(vcl::mesh_drawable& square, vcl::mesh_drawable& lac, vcl::hierarchy_mesh_drawable& tree,int r_v,int r_h,float u0,float v0, const gui_scene_structure& gui_scene, std::map<std::string, GLuint>& shaders, std::vector<vcl::vec3>& park_pos, std::vector<vcl::vec3>& tree_pos, std::vector<vcl::vec2>& park_pos_2, GLuint& texture_id_park, GLuint& texture_lac) {
    texture_id_park = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/test.png"));
    texture_lac = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/mer.png"));
    square = create_park(r_v, r_h, u0, v0, gui_scene);
    square.uniform.color = { 1.0f,1,1 };
    square.uniform.shading.specular = 0.0f; // non-specular terrain material
    
    park_pos_2 = create_park_pos_2();
    park_pos = create_park_pos(r_v, r_h, gui_scene, park_pos_2);
    tree_pos = create_tree_pos(r_v, r_h, gui_scene, park_pos_2);/*
    tree = create_tree();
    tree.set_shader_for_all_elements(shaders["mesh"]);
    */
    lac = create_lac(r_v, r_h, u0, v0, gui_scene);
    lac.uniform.color = { 1.0f,1,1 };
    lac.uniform.shading.specular = 0.0f; // non-specular terrain material


    
}

void draw_park(vcl::mesh_drawable& square, vcl::mesh_drawable& lac, vcl::hierarchy_mesh_drawable& tree, int r_v, int r_h, float u0, float v0, const gui_scene_structure& gui_scene, std::map<std::string, GLuint>& shaders, std::vector<vcl::vec3>& park_pos,  std::vector<vcl::vec3>& tree_pos, std::vector<vcl::vec2>& park_pos_2, GLuint& texture_id_park, scene_structure& scene, GLuint& texture_lac) {
    glBindTexture(GL_TEXTURE_2D, texture_id_park);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    for (vec3 o : park_pos) {
        square.uniform.transform.translation = o;
        draw(square, scene.camera, shaders["mesh"]);
    }
    /*
    for (vec3 position : tree_pos) {
        tree["tronc0-0"].transform.translation = position; // le premier tronc
        tree.update_local_to_global_coordinates();
        draw(tree, scene.camera);
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    */
    glBindTexture(GL_TEXTURE_2D, texture_lac);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    for (vec3 o : park_pos) {
        int i = 0;
        lac.uniform.transform.translation = o;
        lac.uniform.transform.rotation = rotation_from_axis_angle_mat3({ 0,0,1 }, i * 3.14 / 2);
        draw(lac, scene.camera, shaders["mesh"]);
        ++i;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

#endif
