#include "homme.hpp"


#include <random>


#ifdef SCENE_TOT


vcl::hierarchy_mesh_drawable create_homme()
{
    vcl::hierarchy_mesh_drawable h;
    float ratio = 0.25f;
    float largeur_epaule = 0.3f * ratio;
    float taille_tronc = 0.7f * ratio;
    float prof_tronc = 0.2f * ratio;
    float cuisse_l = ratio * 0.4f;
    float cuisse_r = ratio * 0.05f;
    float jambe_l = ratio * 0.35f;
    float jambe_r = ratio * 0.03f;
    float tete_r = 0.2f * ratio;
    float oeil_r = 0.05f * ratio;
    float epaule_r = 0.05f * ratio;
    float bras_l = 0.3f * ratio;
    float av_bras_r = 0.035 * ratio;
    float av_bras_l = 0.26f * ratio;
    float pied_l = 0.25f * ratio;
    float pied_r = 0.035f * ratio;


    mesh_drawable tronc = mesh_drawable(mesh_primitive_parallelepiped(vec3(0, 0, 0), vec3(0, prof_tronc, 0), vec3(largeur_epaule, 0, 0), vec3(0, 0, taille_tronc)));
    mesh_drawable hanche = mesh_drawable(mesh_primitive_sphere(cuisse_r));
    mesh_drawable genou = mesh_drawable(mesh_primitive_sphere(cuisse_r * 1.1f));
    mesh_drawable epaule = mesh_drawable(mesh_primitive_sphere(epaule_r * 1.3f));
    mesh_drawable coude = mesh_drawable(mesh_primitive_sphere(epaule_r * 1.1f));
    mesh_drawable cheville = mesh_drawable(mesh_primitive_sphere(pied_r * 0.1f));
    mesh_drawable cuisse = mesh_drawable(mesh_primitive_cylinder(cuisse_r, vec3(0, 0, 0), vec3(0, 0, -cuisse_l)));
    mesh_drawable jambe = mesh_drawable(mesh_primitive_cylinder(jambe_r, vec3(0, 0, 0), vec3(0, 0, -jambe_l)));
    mesh_drawable bras = mesh_drawable(mesh_primitive_cylinder(epaule_r, vec3(0, 0, 0), vec3(bras_l, 0, 0)));
    mesh_drawable av_bras = mesh_drawable(mesh_primitive_cylinder(av_bras_r, vec3(0, 0, 0), vec3(av_bras_l, 0, 0)));
    mesh_drawable tete = mesh_drawable(mesh_primitive_sphere(tete_r));
    mesh_drawable oeil = mesh_drawable(mesh_primitive_sphere(oeil_r));
    mesh_drawable pied = mesh_drawable(mesh_primitive_parallelepiped(vec3(0, 0, 0), vec3(0, -pied_l, 0), vec3(jambe_r, 0, 0), vec3(0, 0, pied_r)));


    oeil.uniform.color = { 0,0,0 };

    h.add(tronc, "tronc");

    h.add(hanche, "hancheD", "tronc", { largeur_epaule / 6.0f,prof_tronc / 2.0f,0 });
    h.add(hanche, "hancheG", "tronc", { 5.0f * largeur_epaule / 6.0f,prof_tronc / 2.0f,0 });
    h.add(cuisse, "cuisseD", "hancheD");
    h.add(cuisse, "cuisseG", "hancheG");
    h.add(genou, "genouD", "cuisseD", { 0,0,-cuisse_l });
    h.add(genou, "genouG", "cuisseG", { 0,0,-cuisse_l });
    h.add(jambe, "jambeD", "genouD");
    h.add(jambe, "jambeG", "genouG");
    h.add(cheville, "chevilleD", "jambeD", { 0,0,-jambe_l });
    h.add(cheville, "chevilleG", "jambeG", { 0,0,-jambe_l });
    h.add(pied, "piedD", "chevilleD", { 0,pied_l / 4.0f,0 });
    h.add(pied, "piedG", "chevilleG", { 0,pied_l / 4.0f,0 });

    h.add(tete, "tete", "tronc", { largeur_epaule / 2.0f,prof_tronc / 3.0f,tete_r + taille_tronc });
    h.add(oeil, "oeilD", "tete", { tete_r * std::cos(-1.90f),tete_r * std::sin(-1.90f),0 });
    h.add(oeil, "oeilG", "tete", { tete_r * std::cos(-1.30f),tete_r * std::sin(-1.30f),0 });

    h.add(epaule, "epauleG", "tronc", { largeur_epaule ,prof_tronc / 2.0f,taille_tronc });
    h.add(epaule, "epauleD", "tronc", { 0 ,prof_tronc / 2.0f,taille_tronc });
    h.add(bras, "brasD", "epauleD", { {0,0,0},{-1,0,0,0,1,0,0,0,-1} });
    h.add(bras, "brasG", "epauleG");
    h.add(coude, "coudeD", "brasD", { bras_l,0,0 });
    h.add(coude, "coudeG", "brasG", { bras_l,0,0 });
    h.add(av_bras, "avbrasD", "coudeD");
    h.add(av_bras, "avbrasG", "coudeG");





    return h;
}

float rotation(vcl::vec3 v) {
    if (v[0] == 0) {
        if (v[1] < 0) {
            return 0;
        }
        else { return 3.14f; }
    }
    float k = 1;
    if (v[0] < 0) { k = -1; }
    return std::atan(v[1] / v[0]) + 1.57 * k;
}

std::vector<vec3p> create_pos_perso(int v_larg, int h_high, const gui_scene_structure& gui_scene) {
    std::vector<vec3p> pos;
    vec3p m;

    m.v.push_back(vec3(-4.1f, 0, 0));
    m.v.push_back(vec3(0, 4.1f, 0));
    m.v.push_back(vec3(4.1f, 0, 0));
    m.v.push_back(vec3(0, -4.1f, 0));

    m.y = 0;
    m.z = 0;
    m.direction = 0;
    m.traverse = false;

    int bat_v = int(100 / (3 * h_high)); // nb route horizontales
    int bat_h = int(100 / (3 * v_larg)); // nb "" vrticales


    for (int k = 0; k < bat_h; k++) {
        for (int j = 0; j < bat_v-2; j++) {
            m.valeur.clear();
            m.p = vec3(20 * (((k + 1) * 3 * v_larg) / 100.0f - 0.5) - 0.1, 20 * (((j + 1) * 3 * h_high - 2 * h_high) / 100.0f - 0.5) - 0.05f, 0);
            m.valeur.push_back(20 * (((k + 1) * 3 * v_larg - 2 * v_larg) / 100.0f - 0.5) - 0.1);
            m.valeur.push_back(20 * (((j + 1) * 3 * h_high) / 100.0f - 0.5) - 0.1); // 15 = 3 * h_high
            m.valeur.push_back(20 * (((k + 1) * 3 * v_larg) / 100.0f - 0.5) - 0.1);
            m.valeur.push_back(20 * (((j + 1) * 3 * h_high - 2 * h_high) / 100.0f - 0.5) - 0.1);

            if ((k * j) % 4 != 0 && (k * j) % 4 != 2) { pos.push_back(m); }

        }

    }

    return pos;
}
void draw_homme(int v_larg, int h_high, vcl::hierarchy_mesh_drawable homme, float t, scene_structure& scene, const gui_scene_structure& gui_scene, std::vector<vec3p>& pos, float dt) {
    float ratio = 0.25f; // le meme que dans create_homme

    // marche
    float geno_1 = 0.5f * -std::cos(6.28f * t - 3.14f);
    if (geno_1 < 0) { geno_1 = 0; }
    float geno_2 = 0.5f * std::cos(6.28f * t - 3.14f);
    if (geno_2 < 0) { geno_2 = 0; }

    // court
    geno_1 = -std::cos(6.28f * t - 3.14f);
    if (geno_1 < 0) { geno_1 = -geno_1; }
    geno_2 = std::cos(6.28f * t - 3.14f);
    if (geno_2 < 0) { geno_2 = -geno_2; }

    // a adapter si on veut courir ou marcher
    float angle_rotation;
    mat3 R_tot;
    mat3 const R_1 = rotation_from_axis_angle_mat3({ 1,0,0 }, 1.0f * std::cos(6.28f * t - 3.14f));  // court 1 marche 0.5
    mat3 const R_2 = rotation_from_axis_angle_mat3({ 1,0,0 }, 1.0f * -std::cos(6.28f * t - 3.14f));
    mat3 const R_geno_1 = rotation_from_axis_angle_mat3({ 1,0,0 }, geno_1);
    mat3 const R_geno_2 = rotation_from_axis_angle_mat3({ 1,0,0 }, geno_2);
    mat3 const R_epaule_y_d = rotation_from_axis_angle_mat3({ 0,1,0 }, -0.85f);
    mat3 const R_epaule_y_g = rotation_from_axis_angle_mat3({ 0,1,0 }, 0.85f);
    mat3 const R_coude_z = rotation_from_axis_angle_mat3({ 0,0,1 }, -0.85f);
    mat3 const R_epaule_z = rotation_from_axis_angle_mat3({ 0,0,1 }, 0.8f * std::cos(2 * 3.14f * t)); // court 0.8 marche 0.1

    homme["hancheG"].transform.rotation = R_1;
    homme["hancheD"].transform.rotation = R_2;
    homme["genouG"].transform.rotation = R_geno_1;
    homme["genouD"].transform.rotation = R_geno_2;
    homme["epauleG"].transform.rotation = R_epaule_y_g * R_epaule_z;
    homme["epauleD"].transform.rotation = R_epaule_y_d * R_epaule_z;
    homme["coudeG"].transform.rotation = R_coude_z;
    homme["coudeD"].transform.rotation = R_coude_z;

    vec3p o;
    for (int k = 0; k < pos.size(); k++) {
        {
            vec3& p = pos[k].p;
            vec3& v = pos[k].v[pos[k].direction];
            float& valeur = pos[k].valeur[pos[k].direction];
            std::vector<float>& val = pos[k].valeur;
            int& direction = pos[k].direction;
            p = p + dt * v * ratio;
            // if pas dans le vide on fait ca
            // else p += dt*dt*m*9.81f*vec3(0,0,-1)
            if ((p[pos[k].direction % 2] < valeur && (direction == 0 || direction == 3)) || (p[pos[k].direction % 2] > valeur && (direction == 1 || direction == 2))) {
                if (rand() % 2 == 0 && p[0] > -9.0f && p[1] > -9.0f && p[0] < 9.0f && p[1] < 9.0f) { // continue tout droit // si vide, peut pas tourner
                    int k = 1; //  + ou -
                    if (direction == 0 || direction == 3) { k = -1; }
                    if (direction == 0 || direction == 2) {
                        val[0] += k * 20 * 3 * v_larg / 100.0f;
                        val[2] += k * 20 * 3 * v_larg / 100.0f;
                    }
                    if (direction == 1 || direction == 3) {
                        val[1] += k * 20 * 3 * h_high / 100.0f;
                        val[3] += k * 20 * 3 * h_high / 100.0f;
                    }
                }
                else { direction = (direction + 1) % 4; } // tourne

            }

        }
        angle_rotation = rotation(pos[k].v[pos[k].direction]);
        R_tot = rotation_from_axis_angle_mat3({ 0,0,1 }, angle_rotation);

        o = pos[k];


        homme["tronc"].transform.translation = o.p + o.v[pos[k].direction] * dt * ratio + vec3(0, 0, ratio * 0.85f * (2 + abs(std::sin(6.28f * t - 3.14f))) / 3.0f + evaluate_terrain_z((o.p[0] + dt * ratio * o.v[pos[k].direction][0]) / 20.0f + 0.5f, (o.p[1] + dt * ratio * o.v[pos[k].direction][1]) / 20.0f + 0.5f, gui_scene));
        homme["tronc"].transform.rotation = R_tot;
        homme.update_local_to_global_coordinates();
        draw(homme, scene.camera);
    }


    // return pos;

}




#endif