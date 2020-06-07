#include "phare.hpp"


#include <random>


#ifdef SCENE_TOT


// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::

using namespace vcl;



void init_phare(vcl::mesh_drawable& phare, vcl::mesh_drawable& chapeau, vcl::mesh_drawable& barriere, vcl::mesh_drawable& fenetre) {
    //PHARE

    mesh_drawable cone;
    cone = mesh_drawable(mesh_primitive_cylinder(1.0f, { -25,25,27 }, { -25,25,34 }, 10, 10));
    //phare.uniform.color={1,0,0};
   // phare.uniform.transform.translation={-29, 29,29};
    //ovale.uniform.transform.translation={-28, 28,30};
    cone.texture_id = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/phare.png"));
    phare = cone;


    mesh_drawable rond;
    rond = mesh_drawable(mesh_primitive_sphere(0.9f, { -25,25,36 }, 20, 40));
    //rond.uniform.transform.translation={-29, 29,29};
    rond.uniform.color = { 1,1,0 };

    chapeau = rond;
    mesh_drawable bar = mesh_primitive_cylinder(1.1f, { -25,25,34 }, { -25,25,36 }, 10, 10);
    bar.texture_id = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/barriere.png"));
    barriere = bar;

    mesh_drawable fen = mesh_primitive_cylinder(1.0f, { -25,25,34 }, { -25,25,36.5f }, 10, 10);
    fen.texture_id = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/fenetre.png"));
    fenetre = fen;
}

void draw_phare(std::map<std::string, GLuint>& shaders, scene_structure& scene, vcl::mesh_drawable& phare, vcl::mesh_drawable& chapeau, vcl::mesh_drawable& barriere, vcl::mesh_drawable& fenetre) {
    draw(chapeau, scene.camera, shaders["mesh"]);
    draw(fenetre, scene.camera, shaders["mesh"]);


    draw(barriere, scene.camera, shaders["mesh"]);

    draw(phare, scene.camera, shaders["mesh"]);
}










#endif