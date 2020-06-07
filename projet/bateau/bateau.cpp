
#include "bateau.hpp"


#include <random>
#ifdef SCENE_TOT



// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;

// bateau

mesh_drawable create_voile() {
    mesh voi;
    float const ratio = 4;
    const int N = 40;
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < k + 1; i++) {
            float u = ratio * (1 - k / float(N - 1));
            float v = ratio * i / float(N - 1);
            voi.position.push_back(vec3(v, 0, u));
        }
    }
    for (int k = 0; k < N - 1; k++) {
        for (int i = 0; i < k; i++) {
            uint3 triangle_1 = { k * (k + 1) / 2 + i , (k + 2) * (k + 1) / 2 + i ,(k + 2) * (k + 1) / 2 + 1 + i };
            voi.connectivity.push_back(triangle_1);
            uint3 triangle_2 = { k * (k + 1) / 2 + i , k * (k + 1) / 2 + i + 1,(k + 2) * (k + 1) / 2 + 1 + i };
            voi.connectivity.push_back(triangle_2);
        }
        uint3 triangle_1 = { k * (k + 1) / 2.0f + k , (k + 2) * (k + 1) / 2.0f + k ,(k + 2) * (k + 1) / 2.0f + 1 + k };
        voi.connectivity.push_back(triangle_1);
    }
    return voi;
}

float rayon(float r, int n, int i) {
    return 4 * r * i * (i - n) / (n * n);
}

mesh create_cylindre_coque(float r, float length, int i, float y_actuel)
{
    mesh m;
    const size_t N = 20;
    const size_t n = 20;
    float y_courant = y_actuel;
    float y_suivant = y_courant + length / n;
    for (size_t k = 0; k < N; ++k) {
        const float u = k / float(N);

        if (k < n / 2 + 1) {
            float z_courant = rayon(r, n, i) * std::cos(2 * 3.14f * u);
            float z_suivant = rayon(r, n, i + 1) * std::cos(2 * 3.14f * u);
            float x_courant = rayon(r, n, i) * std::sin(2 * 3.14f * u);
            float x_suivant = rayon(r, n, i + 1) * std::sin(2 * 3.14f * u);
            const vec3 p = { x_courant,y_courant, z_courant };
            m.position.push_back(p);
            m.position.push_back(vec3(x_suivant, y_suivant, z_suivant));
        }
        else {
            float z_courant = 0;
            float z_suivant = 0;
            float x_courant = 0;
            float x_suivant = 0;
            const vec3 p = { x_courant,y_courant, z_courant };
            m.position.push_back(p);
            m.position.push_back(vec3(x_suivant, y_suivant, z_suivant));
        }

    }
    for (size_t k = 0; k < N; ++k)
    {
        const unsigned int u00 = 2 * k;
        const unsigned int u01 = (2 * k + 1) % (2 * N);
        const unsigned int u10 = (2 * (k + 1)) % (2 * N);
        const unsigned int u11 = (2 * (k + 1) + 1) % (2 * N);

        const uint3 t1 = { u00, u10, u11 };
        const uint3 t2 = { u00, u11, u01 };
        m.connectivity.push_back(t1);
        m.connectivity.push_back(t2);
    }
    return m;
}

mesh create_coque(float r, float length)
{
    // Number of samples
    const size_t N = 20;
    const size_t n = 20; // nombre de cylindres
    mesh m = create_cylindre_coque(r, length, 0, 0);
    float y_actuel = length / n;
    for (int i = 1; i < n; ++i) {
        // Geometrie
        float x_l = length / n;
        m.push_back(create_cylindre_coque(r, length, i, y_actuel));
        y_actuel += x_l;
    }
    return m;
}

std::vector<particle_element> init_pos() {
    std::vector<particle_element> p;
    float ratio = 1;
    const int N = 40;
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < k + 1; i++) {
            particle_element c;
            float u = ratio * (1 - k / float(N - 1));
            float v = ratio * i / float(N - 1);
            c.p = vec3(v, 0, u);
            p.push_back(c);
        }
    }
    return p;
}

std::vector<vec3> init_force() {
    std::vector<vec3> p;
    const int N = 40;
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < k + 1; i++) {
            p.push_back(vec3(0, 0, 0));
        }
    }
    return p;
}

vec3 spring_force(const vec3& pi, const vec3& pj, float L0, float K)
{
    vec3 f = K * (norm(pi - pj) - L0) * (pj - pi) / norm(pi - pj); // force de pi sur pj
    return f;
}

mesh_drawable update_voile(std::vector<particle_element>& pos, std::vector<vec3>& fo, float dt, float t) {
    int N = 40;
    const float m = 0.01f;        // particle mass
    const float K = 10.0f;         // spring stiffness
    const float mu = 0.002f;       // damping coefficient
    const float l0 = 1 / 40.0f;

    for (int k = 0; k < N - 1; k++) {
        for (int i = 1; i < k; i++) {
            vec3 vent = vec3(0, -0.01, 0);
            if (int(t) % 6 >= 3) { vent = vec3(0, 0.01, 0); }
            vec3 force = spring_force(pos[k * (k + 1) / 2 + i].p, pos[k * (k + 1) / 2 + i + 1].p, l0, K) + spring_force(pos[k * (k + 1) / 2 + i].p, pos[k * (k + 1) / 2 + i - 1].p, l0, K) + spring_force(pos[k * (k + 1) / 2 + i].p, pos[(k + 2) * (k + 1) / 2 + i].p, l0, K) + spring_force(pos[(k + 1) * k / 2 + i].p, pos[(k - 1) * k / 2 + i].p, l0, K) - mu * pos[k * (k + 1) / 2 + i].v + vent; // 0.01f*vec3(0.1*(std::cos(t / 5.0f) + 1),std::cos(t/15.0f)+1,0)
            fo[k * (k + 1) / 2 + i] = force;
        }
    }

    for (int k = 0; k < N - 1; k++) {
        for (int i = 1; i < k; i++) {
            vec3& p = pos[k * (k + 1) / 2 + i].p;
            vec3& v = pos[k * (k + 1) / 2 + i].v;
            v = v + dt * fo[k * (k + 1) / 2 + i] / m;
            p = p + dt * v;
        }
    }

    mesh voi;
    float const ratio = 4.0f;
    for (int k = 0; k < N; k++) {
        for (int i = 0; i < k + 1; i++) {
            float u = ratio * (1 - k / float(N - 1));
            float v = ratio * i / float(N - 1);
            voi.position.push_back(ratio * pos[k * (k + 1) / 2 + i].p);
            voi.color.push_back(vec4(u / ratio, v / ratio, (v + u) / (2.0f * ratio), 1));
        }
    }
    for (int k = 0; k < N - 1; k++) {
        for (int i = 0; i < k; i++) {
            uint3 triangle_1 = { k * (k + 1) / 2 + i , (k + 2) * (k + 1) / 2 + i ,(k + 2) * (k + 1) / 2 + 1 + i };
            voi.connectivity.push_back(triangle_1);
            uint3 triangle_2 = { k * (k + 1) / 2 + i , k * (k + 1) / 2 + i + 1,(k + 2) * (k + 1) / 2 + 1 + i };
            voi.connectivity.push_back(triangle_2);
        }
        uint3 triangle_1 = { k * (k + 1) / 2.0f + k , (k + 2) * (k + 1) / 2.0f + k ,(k + 2) * (k + 1) / 2.0f + 1 + k };
        voi.connectivity.push_back(triangle_1);
    }

    return voi;

}

vcl::hierarchy_mesh_drawable create_bateau() {
    vcl::hierarchy_mesh_drawable bat;
    float ratio = 4.0f;
    mesh_drawable m = create_coque(1.5f, 6.0f);
    m.uniform.transform.rotation = rotation_from_axis_angle_mat3({ 0,0,1 }, -1.57f);
    mesh_drawable mat = mesh_drawable(mesh_primitive_cylinder(0.02 * ratio, { 0,0,0 }, { 0,0,1.5f * ratio }));
    mesh_drawable voile = create_voile();
    mat.uniform.color = { 0,0,0 };

    bat.add(m, "coque");
    bat.add(mat, "mat", "coque", { { 1.0f * ratio,0,0 },{1,0,0,0,0,-1,0,1,0} });
    bat.add(voile, "voile", "mat", { 0,0,0.5f * ratio });
    return bat;
}

std::vector<vcl::vec2> create_bateau_pos() {
    std::vector<vcl::vec2> pos_bat;
    pos_bat.push_back(vec2(0.1f, 0.1f));
    pos_bat.push_back(vec2(0.4f, 0.2f));
    pos_bat.push_back(vec2(0.25f, 0.15f));
    pos_bat.push_back(vec2(0.8f, 0.3f));
    pos_bat.push_back(vec2(0.1f, 0.24f));

    pos_bat.push_back(vec2(0.3f, 0.69f));
    pos_bat.push_back(vec2(0.1f, 0.64f));
    pos_bat.push_back(vec2(0.8f, 0.60f));
    pos_bat.push_back(vec2(0.7f, 0.4f));
    pos_bat.push_back(vec2(0.2f, 0.35f));

    pos_bat.push_back(vec2(0.4f, 0.7f));
    pos_bat.push_back(vec2(0.6f, 0.8f));
    pos_bat.push_back(vec2(0.1f, 0.85f));
    pos_bat.push_back(vec2(0.9f, 0.95f));
    pos_bat.push_back(vec2(0.7f, 0.75f));
    return pos_bat;
}

vcl::hierarchy_mesh_drawable update_bat(std::vector<particle_element>& pos, std::vector<vec3>& fo, float dt, float t) {
    vcl::hierarchy_mesh_drawable bat;
    float ratio = 4.0f;
    mesh_drawable m = create_coque(1.5f / 4.0f * ratio, 9.0f / 4.0f * ratio);
    m.uniform.transform.rotation = rotation_from_axis_angle_mat3({ 0,0,1 }, -1.57f);
    mesh_drawable mat = mesh_drawable(mesh_primitive_cylinder(0.02 * ratio, { 0,0,0 }, { 0,0,1.5f * ratio }));
    mesh_drawable voile = update_voile(pos, fo, dt, t);
    mat.uniform.color = { 0,0,0 };
    bat.add(m, "coque");
    bat.add(mat, "mat", "coque", { { 1.0f * ratio,0,0 },{1,0,0,0,0,-1,0,1,0} });
    bat.add(voile, "voile", "mat", { 0,0,0.5f * ratio });
    return bat;
}

void draw_bateau(std::vector<particle_element>& pos, std::vector<vec3>& fo, float dt, float t, std::vector<vec2>& pos_bateau, vcl::hierarchy_mesh_drawable bateau, scene_structure& scene, std::map<std::string, GLuint>& shaders) {
    bateau = update_bat(pos, fo, dt, t);

    // get gui parameters
    const float scaling = 0.35f;
    const int octave = 7;
    const float persistency = 0.7f;

    for (int k = 0; k < pos_bateau.size(); k++) {
        float& u = pos_bateau[k][0];
        float& v = pos_bateau[k][1];
        u += 0.001f;
        if (u > 1) { u = -0.04; }
        const float noise = perlin(scaling * u, scaling * v, octave, persistency);

        // 3D vertex coordinates
        const float x = 400 * (u - 0.5f);
        const float y = 400 * (v - 0.5f);
        const float c = 245 / 255.0f + 0.7f * noise;

        bateau["coque"].transform.translation = { x,y,15 + 0.4f * cos(noise * x * 2 * 3.14f / 9.0f - t * 2 * 3.14f / 1 + noise * y * 2 * 3.14f / 3.0f) +1.0f };
        bateau["coque"].transform.rotation = rotation_from_axis_angle_mat3({ 1,0,0 }, 0.02 * std::sin(t * 6) - 1.57f) * rotation_from_axis_angle_mat3({ 0,0,1 }, 0.01 * std::sin(t * 6));
        bateau.update_local_to_global_coordinates();
        draw(bateau, scene.camera, shaders["mesh"]);
    }




    // Evaluate Perlin noise

}
// bateau

void init_bateau(std::vector<vcl::vec2>& pos_bateau, vcl::hierarchy_mesh_drawable& bateau, std::vector<vec3>& force, std::vector<particle_element>& par) {
    pos_bateau = create_bateau_pos();

    bateau = create_bateau();

    force = init_force();
    par = init_pos();
}

#endif
