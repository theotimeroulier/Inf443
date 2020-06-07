#include "skybox.hpp"


#include <random>


#ifdef SCENE_TOT

using namespace vcl;


void init_skybox(vcl::mesh_drawable& cube, GLuint& texture_skybox) {
	//SKYBOX
	cube = create_cube(200.0f);
	cube.uniform.shading = { 1,0,0 };
	cube.uniform.transform.rotation = rotation_from_axis_angle_mat3({ 1,0,0 }, 3.14f / 2.0f);
	cube.uniform.transform.translation = { 0,200,15 };
	texture_skybox = create_texture_gpu(image_load_png("scenes/3D_graphics/projet/assets/skybox_3.png"));
}

void draw_skybox(std::map<std::string, GLuint>& shaders, scene_structure& scene, vcl::mesh_drawable& cube) {
	glBindTexture(GL_TEXTURE_2D, texture_skybox);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	draw(cube, scene.camera, shaders["mesh"]);

	glEnable(GL_POLYGON_OFFSET_FILL); // avoids z-fighting when displaying wireframe
	// Before displaying a textured surface: bind the associated texture id
	glPolygonOffset(1.0, 1.0);
}


















#endif