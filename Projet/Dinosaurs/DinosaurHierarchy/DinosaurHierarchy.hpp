#pragma once
#include "main/scene_base/base.hpp"
#include "scenes/3D_graphics/Projet/Planet/Planet.hpp"

class DinosaurHierarchy {
    // can't use dinosaure
    protected :
    float ScalePlanet;
    const float RatioDinoPlanet = 0.02f; // same as in dinosaur
    vcl::hierarchy_mesh_drawable m_hierarchy; 
    Planet *m_planet;
    float m_scale;

    // saving it since we use it every frame for the animation
    vcl::mat3 m_R_right = rotation_from_axis_angle_mat3({0,1,0}, M_PI/2);
    vcl::mat3 m_R_left = rotation_from_axis_angle_mat3({0,1,0},  -M_PI/2);
    vcl::mat3 m_R = rotation_from_axis_angle_mat3({1,0,0}, M_PI);
    float m_factor1 = 0.4*M_PI;
    float eq_pos = -M_PI/8;
    float t_max_2 = 0.5f; 
    float alpha = M_PI/t_max_2;
    //texture for types of dino
    GLuint texture_id_1 = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/Chilesaurus.png"), GL_REPEAT, GL_REPEAT);
    GLuint texture_id_2 = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/Trex.png"), GL_REPEAT, GL_REPEAT);

    public:
    // animation: 
    DinosaurHierarchy();
    DinosaurHierarchy(Planet *planet, int type);

    // additionnal 
    void buildHierarchy();
    void animate(float t); // we'll build 2 frames, and interpolates between them
    void animateDeath(vcl::vec3 positionOnPlanet);
    void updateMesh(vcl::vec3 translation, vcl::mat3 rotation);
    void update_local_to_global_coordinates();
    void updateMesh(float scale, GLuint texture_id);

    // Setter - Getter
    vcl::hierarchy_mesh_drawable getMeshDrawable();
};