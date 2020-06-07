
#include "batiment.hpp"


#include <random>
#ifdef SCENE_TOT



// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;

mesh create_bat_single(float lon, float lar) {
    mesh bat;


    bat.position.push_back(vec3(0, lar, lon));
    bat.position.push_back(vec3(0, lar, 0));
    bat.position.push_back(vec3(0, 0, 0));
    bat.position.push_back(vec3(0, 0, lon));
    bat.position.push_back(vec3(lar, 0, lon));
    bat.position.push_back(vec3(lar, 0, 0));
    bat.position.push_back(vec3(lar, lar, lon));
    bat.position.push_back(vec3(lar, lar, 0));
    bat.position.push_back(vec3(0, lar, lon));
    bat.position.push_back(vec3(0, lar, 0));
    bat.position.push_back(vec3(0, lar, 0));
    bat.position.push_back(vec3(lar, lar, 0));
    bat.position.push_back(vec3(0, lar, lon));
    bat.position.push_back(vec3(lar, lar, lon));

    bat.texture_uv.push_back({ 0,0.16397 });
    bat.texture_uv.push_back({ 0,0.6127 });
    bat.texture_uv.push_back({ 0.2333333,0.6127 });
    bat.texture_uv.push_back({ 0.2333333,0.16397 });
    bat.texture_uv.push_back({ 2 * 0.2333333,0.16397 });
    bat.texture_uv.push_back({ 2 * 0.2333333,0.6127 });
    bat.texture_uv.push_back({ 3 * 0.2333333,0.16397 });
    bat.texture_uv.push_back({ 3 * 0.2333333,0.6127 });
    bat.texture_uv.push_back({ 4 * 0.2333333,0.16397 });
    bat.texture_uv.push_back({ 4 * 0.2333333,0.6127 });
    bat.texture_uv.push_back({ 0.2333333,0.77669 });
    bat.texture_uv.push_back({ 2 * 0.2333333,0.77669 });
    bat.texture_uv.push_back({ 0.224429,0 });
    bat.texture_uv.push_back({ 2 * 0.224429,0.16397 });

    uint3 triangle = { 0,1,2 };
    bat.connectivity.push_back(triangle);
    triangle = { 0,2,3 };
    bat.connectivity.push_back(triangle);
    triangle = { 3,2,5 };
    bat.connectivity.push_back(triangle);
    triangle = { 3,5,4 };
    bat.connectivity.push_back(triangle);
    triangle = { 4,5,7 };
    bat.connectivity.push_back(triangle);
    triangle = { 4,7,6 };
    bat.connectivity.push_back(triangle);
    triangle = { 6,7,9 };
    bat.connectivity.push_back(triangle);
    triangle = { 6,9,8 };
    bat.connectivity.push_back(triangle);
    triangle = { 3,4,12 };
    bat.connectivity.push_back(triangle);
    triangle = { 12,4,13 };
    bat.connectivity.push_back(triangle);
    triangle = { 2,10,11 };
    bat.connectivity.push_back(triangle);
    triangle = { 2,5,11 };
    bat.connectivity.push_back(triangle);

    return bat;
}

std::vector<vcl::mesh_drawable> create_bat() {
    std::vector<vcl::mesh_drawable> b;
    vcl::mesh_drawable bat;
    bat = create_bat_single(2.0f, 0.5f);
    b.push_back(bat);
    bat = create_bat_single(1.0f, 0.5f);
    b.push_back(bat);
    bat = create_bat_single(1.5f, 0.5f);
    b.push_back(bat);
    return b;
}

void ajout_b(int k, int i, int v_larg, int h_high, std::vector<vec3bat>& h, const gui_scene_structure& gui_scene) { // ds carre
    float l = 0.5f + 0.16f; // larg bat + espace entre
    vec3bat o;
    float x;
    float y;
    int nb_bat_v = int(2 * h_high / (l * 5.0f));
    int nb_bat_h = int(2 * v_larg / (l * 5.0f));

    for (int m = 0; m < nb_bat_v; m++) {
        for (int n = 0; n < nb_bat_h; n++) {
            x = -10.0f + (3.0f * k + 1.0f) * v_larg / 5.0f + (l + 2 * v_larg / 5.0f - l * nb_bat_h) * n;
            y = -10.0f + (3.0f * i + 1.0f) * h_high / 5.0f + (l + 2 * h_high / 5.0f - l * nb_bat_v) * m;
            o.p = vec3(x, y, evaluate_terrain_z(x / 20.0f + 0.5f, y / 20.0f + 0.5f, gui_scene));
            o.n = rand() % 3;  // nombre de tailles diff
            o.text = rand() % 4; // nombre de text diff
            if ((m == 1 || m == 0) && n == 1) { o.n = 0; }
            h.push_back(o);
        }
    }
    // return h;
}

bool contains(vcl::vec2& v, std::vector<vcl::vec2>& v_2) {
    for (vec2 p : v_2) {
        if (p[0] == v[0] && p[1] == v[1]) {
            return true;
        }
    }
    return false;
}


std::vector<vec3bat> create_bat_pos(int v_larg, int h_high, const gui_scene_structure& gui_scene, std::vector<vcl::vec2>& o_2) {
    std::vector<vec3bat> h;
    int hole_h = 100.0f / float(3 * v_larg);
    int hole_v = 100.0f / float(3 * h_high);
    for (int k = 0; k < hole_h; k++) {
        for (int i = 0; i < hole_v; i++) {
            if (!contains(vec2(k, i), o_2)) {
                ajout_b(k, i, v_larg, h_high, h, gui_scene);
            }

        }
    }
    return h;
}



void draw_bat(std::vector<vec3bat> pos, std::vector<vcl::mesh_drawable> batiment, std::vector<GLuint> b, std::map<std::string, GLuint>& shaders, scene_structure& scene) {

    for (vec3bat bat : pos) {
        glBindTexture(GL_TEXTURE_2D, b[bat.text]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glEnable(GL_POLYGON_OFFSET_FILL);
        batiment[bat.n].uniform.transform.translation = bat.p;
        draw(batiment[bat.n], scene.camera, shaders["mesh"]);
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

std::vector<GLuint> text_bat() { // différentes text
    std::vector<GLuint> t;
    std::string d;
    for (int k = 0; k < 4; k++) {
        d = "scenes/3D_graphics/projet/assets/bat_" + std::to_string(k) + ".png";
        t.push_back(create_texture_gpu(image_load_png(d)));
    }
    return t;
}

void init_bat(std::vector<vcl::mesh_drawable>& batiment, std::vector<vec3bat>& bat_pos, std::vector<GLuint>& texture_id_bat, const gui_scene_structure& gui_scene,int r_v,int r_h,std::vector<vec2>& park_pos_2) {
    texture_id_bat = text_bat();
    batiment = create_bat();
    bat_pos = create_bat_pos(r_v, r_h, gui_scene, park_pos_2);
}

#endif
