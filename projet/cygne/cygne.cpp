#include "Cygne.h"

//  rouge : x bleu : z vert y

#ifdef SCENE_TOT

using namespace vcl;
std::vector<float> liste_x;
std::vector<float> liste_z;





vcl::hierarchy_mesh_drawable create_cygne() {
    vcl::hierarchy_mesh_drawable hierarchy;
    mesh create_corps(float, float);
    mesh create_aile(float, float);
    mesh create_cou(float, float);
    texture_plumes = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/plume.png"));


    const float radius_body = 0.25f;
    const float length_body = 1.0f;


    // body
    mesh_drawable body = create_corps(radius_body, length_body);
    body.uniform.transform.translation = { -0.01*0.1,-0.26*0.1,0 };
    body.uniform.transform.scaling = 0.1f;

    // yeux
    mesh_drawable eye = mesh_drawable(mesh_primitive_sphere(0.005f, { 0,0,0 }, 20, 20));
    mesh_drawable tete = mesh_drawable(mesh_primitive_sphere(0.07f, { 0,0,0 }, 20, 20));

    tete.uniform.transform.translation = { 0,0.23*0.1,0.95*0.1 };
    tete.uniform.transform.scaling =  0.1f;
    eye.uniform.color = { 0,0,0 };
    eye.uniform.transform.translation = { 0,0.17*0.1,0.98*0.1 };
    eye.uniform.transform.scaling =  0.1f;
    // ailes
    mesh_drawable aile = create_aile(0.15, 0.7);
    aile.uniform.color = { 1,1,1 };
    aile.uniform.transform.rotation = rotation_from_axis_angle_mat3({ 1 ,0,0 }, 0.4);
    aile.uniform.transform.scaling = 1.4*0.1f;


    body.texture_id = texture_plumes;
    //cou
    mesh_drawable cou = create_cou(0.8, 1.0);
    cou.uniform.transform.scaling = 0.05*0.1f;
    cou.uniform.transform.translation = { 0,0.15*0.1,0.9*0.1 };
    cou.uniform.transform.translation = { 0,0.15*0.1,0.9*0.1 };


    hierarchy.add(body, "body");




    hierarchy.add(aile, "aile_right", "body", { radius_body*0.1,0,0 });// the arm start at the center of the elbow
    hierarchy.add(aile, "aile_left", "body", { -radius_body*0.1 , 0, 0, });
    hierarchy.add(cou, "cou", "body");
    hierarchy.add(tete, "tete", "cou");

    // Eyes 
    hierarchy.add(eye, "eye_left", "tete", vec3(0.04 * 0.1, 0, 0));
    hierarchy.add(eye, "eye_right", "tete", vec3(-0.04 * 0.1, 0, 0));
    // Set the same shader for all the elements
    return hierarchy;
}
// COU


float y(int i, int n)
{
    return 0.5 * std::exp(3 * float(i) / n) * std::sin(1.2 * 3.14 * float(i) / n);
}

float h_locale(float height, int n, int i) {
    return height * i / n * std::cos(1.4 * 3.14 * float(i) / n);
}

float rayon_cygne(float r, int n, int i) {
    return 4 * r * i * (i - n) / (n * n);
}

void draw_cygne(vcl::hierarchy_mesh_drawable cygne, float t, scene_structure& scene, const gui_scene_structure& gui_scene, std::vector<vcl::vec3> pos)
{
    for (int i = 0; i <pos.size(); ++i) {
        mat3 R_aile;
        mat3 R_cou;
        if (t-(int)t > 0.5) {
            R_aile = rotation_from_axis_angle_mat3({ 0,0,1 }, std::abs(std::sin(2 * 3.14f * (10 * t - 0.6f + i / 2))));
            R_cou = rotation_from_axis_angle_mat3({ 1,0,0 }, 0.05 * std::sin(2 * 3.14f * (t - 0.6f + i / 2)));
        }
        else {
            R_aile = { {1,0,0},{0,1,0} ,{0,0,1} };
            R_cou = { {1,0,0},{0,1,0} ,{0,0,1} };
        }
        // Symmetry in the x-direction between the left/right parts
        mat3 const Symmetry = { -1,0,0, 0,1,0, 0,0,1 };
        cygne["body"].transform.translation = { 0,0,0.2f * (1 + std::sin(2 * 3.14f * t)) };
        cygne["cou"].transform.rotation = R_cou;

        cygne["aile_left"].transform.rotation = R_aile;

        //hierarchy["shoulder_right"].transform.rotation = Symmetry * R_shoulder; // apply the symmetry
        cygne["aile_right"].transform.rotation = Symmetry * R_aile; //note that the symmetry is already applied by the parent element

        cygne["body"].transform.translation = pos[i] + vec3(0,0,-0.2f);
   

        cygne.update_local_to_global_coordinates();
        draw(cygne, scene.camera);
}
}

std::vector<vcl::vec3> create_cygne_pos(int v_larg, int h_high, const gui_scene_structure& gui_scene, std::vector<vcl::vec2>& o_2) {
    std::vector<vcl::vec3> cygne_pos;
    for (vec2 p : o_2) {
            float u = u0;
            float v = v0;
            vcl::vec3 m = evaluate_park(v_larg, h_high, gui_scene, u, v) + vec3(20 * (v_larg * 3 * p[0] / 100.0f), 20 * (h_high * 3 * p[1] / 100.0f), 0.5);
            cygne_pos.push_back(m);            
        }
    return cygne_pos;
}

mesh create_cylindre_courbe(float r, float height, int i, float h_actuelle)
{
    mesh m;
    const size_t N = 20;
    const size_t n = 60;
    float h_l = h_locale(height, n, i);
    float z_courant = h_actuelle;
    float z_suivant = z_courant + h_l;
    for (size_t k = 0; k < N; ++k) {
        const float u = k / float(N);
        float y_courant = r * std::sin(2 * 3.14f * u) + y(i, n);
        float y_suivant = r * std::sin(2 * 3.14f * u) + y(i + 1, n);
        const vec3 p = { r * std::cos(2 * 3.14f * u),y_courant, z_courant };
        m.position.push_back(p);
        m.position.push_back(vec3(r * std::cos(2 * 3.14f * u), y_suivant, z_suivant));
    }
    for (size_t k = 0; k < N; ++k)
    {
        const unsigned int u00 = 2 * k;
        const unsigned int u01 = (2 * k + 1) % (2 * N);
        const unsigned int u10 = (2 * (k + 1)) % (2 * N);
        const unsigned int u11 = (2 * (k + 1) + 1) % (2 * N);

        const uint3 t1 = { u11, u10, u00 };
        const uint3 t2 = { u01, u11, u00 };
        m.connectivity.push_back(t1);
        m.connectivity.push_back(t2);
    }
    return m;
}

mesh create_cou(float r, float height)
{
    // Number of samples
    const size_t N = 20;
    const size_t n = 60; // nombre de cylindres
    mesh m = create_cylindre_courbe(r, height, 0, 0);
    float h_actuelle = height / n;
    for (int i = 1; i < n; ++i) {
        // Geometrie
        float h_l = h_locale(height, n, i);
        m.push_back(create_cylindre_courbe(r, height, i, h_actuelle));
        h_actuelle += h_l;
    }
    return m;
}

//AILES

mesh create_cylindre_aile(float r, float length, int i, float x_actuel)
{
    mesh m;

    const size_t N = 20; //nb points par cercles
    const size_t n = 30; //nb cercles
    m.texture_uv.resize(2 * N);
    float a = (std::exp(3 * 3.14 / 2) - std::exp(3.14 / 2)) / n;
    float b = std::exp(3.14 / 2);
    float r_courant = r * std::cos(std::log(a * float(i) + b));
    float r_suivant = r * std::cos(std::log(a * (float(i) + 1) + b));
    float y_courant = x_actuel;
    float y_suivant = y_courant + length / n;
    for (size_t k = 0; k < N; ++k) {
        const float u = k / float(N);
        float z_courant = r_courant * std::cos(2 * 3.14f * u);
        float z_suivant = r_suivant * std::cos(2 * 3.14f * u);
        float x_courant = 0.01 * std::sin(2 * 3.14f * u);
        float x_suivant = 0.01 * std::sin(2 * 3.14f * u);
        const vec3 p = { x_courant,y_courant, z_courant };
        m.texture_uv[k] = { x_courant,z_courant };
        m.texture_uv[2 * k] = { x_suivant,z_suivant };
        m.position.push_back(p);
        m.position.push_back(vec3(x_suivant, y_suivant, z_suivant));
    }
    for (size_t k = 0; k < N; ++k)
    {
        const unsigned int u00 = 2 * k;
        const unsigned int u01 = (2 * k + 1) % (2 * N);
        const unsigned int u10 = (2 * (k + 1)) % (2 * N);
        const unsigned int u11 = (2 * (k + 1) + 1) % (2 * N);

        const uint3 t1 = { u11, u10, u00 };
        const uint3 t2 = { u01, u11, u00 };
        m.connectivity.push_back(t1);
        m.connectivity.push_back(t2);
    }
    return m;
}

mesh create_aile(float r, float length)
{
    // Number of samples
    const size_t N = 20;
    const size_t n = 30; // nombre de cylindres
    mesh m = create_cylindre_aile(r, length, 0, 0);
    float x_actuel = length / n;
    for (int i = 1; i < n; ++i) {
        // Geometrie
        float x_l = length / n;
        m.push_back(create_cylindre_aile(r, length, i, x_actuel));
        x_actuel += x_l;
    }
    return m;
}

// CORPS
mesh create_cylindre_corps(float r, float length, int i, float x_actuel, std::vector<float> liste_x, std::vector<float> liste_z)
{

    mesh m;
    const float scaling = 0.0;
    const int octave = 8;
    const float persistency = 0.5;
    const float height = 0.1;

    // Evaluate Perlin noise
    const size_t N = 100; //nb points par cercles
    const size_t n = 200; //nb cercles

    m.texture_uv.resize(2 * N);
    float r2;
    float r_courant = rayon_cygne(r, n, i);
    float r_suivant = rayon_cygne(r, n, i + 1);
    float y_courant = x_actuel;
    float y_suivant = y_courant + length / n;
    float noisex;
    float noisez;
    for (size_t k = 0; k < N; ++k) {
        r2 = rand_interval();
        const float u = k / float(N);
        float x_courant = ::liste_x[k];
        float z_courant = ::liste_z[k];
        noisex = 0.005 * rand_interval();
        noisez = 0.005 * rand_interval();
        float x_suivant = r_suivant * std::sin(2 * 3.14f * u) + noisex;
        float z_suivant = r_suivant * std::cos(2 * 3.14f * u) + noisez;
        const vec3 p = { x_courant,y_courant, z_courant };
        m.position.push_back(p);
        m.position.push_back(vec3(x_suivant, y_suivant, z_suivant));
        ::liste_x[k] = x_suivant;
        ::liste_z[k] = z_suivant;
    }
    for (size_t k = 0; k < N; ++k)
    {
        const unsigned int u00 = 2 * k;
        const unsigned int u01 = (2 * k + 1) % (2 * N);
        const unsigned int u10 = (2 * (k + 1)) % (2 * N);
        const unsigned int u11 = (2 * (k + 1) + 1) % (2 * N);

        const uint3 t1 = { u00, u10, u11 };
        const uint3 t2 = { u00, u11, u01 };
        m.texture_uv[k] = { x_actuel / length,k / float(N) };
        m.texture_uv[N + k] = { x_actuel / length + 1 / n,k / float(N) };
        m.connectivity.push_back(t1);
        m.connectivity.push_back(t2);
    }
    return m;
}

mesh create_corps(float r, float length)
{
    // Number of samples
    const size_t N = 100;
    const size_t n = 200; // nombre de cylindres
    for (int i = 0; i < N; ++i) {
        liste_x.push_back(0.0);
        liste_z.push_back(0.0);
    }
    mesh m = create_cylindre_corps(r, length, 0, 0, liste_x, liste_z);
    float x_actuel = length / n;
    for (int i = 1; i < n; ++i) {
        // Geometrie
        float x_l = length / n;
        m.push_back(create_cylindre_corps(r, length, i, x_actuel, liste_x, liste_z));
        x_actuel += x_l;
    }
    return m;
}

// BEC

void init_cygne(vcl::hierarchy_mesh_drawable& cygne, std::vector<vcl::vec3>& cygne_pos, GLuint& texture_plumes, float r_v, float r_h, gui_scene_structure gui_scene, std::vector<vcl::vec2>& park_pos_2, std::map<std::string, GLuint>& shaders) {
    cygne = create_cygne();
    cygne.set_shader_for_all_elements(shaders["mesh"]);
    cygne_pos = create_cygne_pos(r_v, r_h, gui_scene, park_pos_2);
}
#endif