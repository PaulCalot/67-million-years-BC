#pragma once

#include <list>
#include <time.h> 
#include "main/scene_base/base.hpp"
#ifdef SCENE_PROJECT
#include "scenes/3D_graphics/Projet/Dinosaurs/DinosaurHierarchy/DinosaurHierarchy.hpp"
#include "scenes/3D_graphics/Projet/Dinosaurs/Chilesaurus.hpp"
#include "scenes/3D_graphics/Projet/Dinosaurs/Tyrannosaurus.hpp"
#include "scenes/3D_graphics/Projet/Meteorite/Meteorite.hpp"
#include "scenes/3D_graphics/Projet/PlanetGrid/Grid.hpp"
struct gui_scene_structure
{
    bool wireframe   = false;
    bool surface     = false;
    bool skeleton    = false;

    // interpolation
    bool display_keyframe = true;
    bool display_polygon  = true;

};


struct scene_model : scene_base {

    
    void setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);

    void set_gui();
    
    gui_scene_structure gui_scene;

    // camera 
    vcl::timer_interval timer_camera;
    Dinosaur *dino_cam;
    Dinosaur *dino_cam2;


    std::vector<TerrainFace> m_planetTerrain;
    std::vector<vcl::mesh_drawable> m_planetDrawable;
    Planet *m_planet;
    // skybox
    vcl::mesh_drawable skybox_west;
    vcl::mesh_drawable skybox_east;
    vcl::mesh_drawable skybox_bottom;
    vcl::mesh_drawable skybox_top;
    vcl::mesh_drawable skybox_north;
    vcl::mesh_drawable skybox_south;

    vcl::mesh_drawable skybox2_west;
    vcl::mesh_drawable skybox2_east;
    vcl::mesh_drawable skybox2_bottom;
    vcl::mesh_drawable skybox2_top;
    vcl::mesh_drawable skybox2_north;
    vcl::mesh_drawable skybox2_south;


    
    const float Scale = 5; // TODO : changing it changes the planet (the noise is not exactly the same + color will change too)

    // dinosaurs

    vcl::timer_interval timer; // timer use for the animation of the dinos
    const int ChilesauriNumber = 50;
    const int TyrannosauriNumber = 1;
    Dinosaur *m_Dinosaurs[51];
    int DinosaursNumber;
    int m_remaining_1 ;
    int m_remaining_2 ;
    int m_dead_1;

    // hiearchy used to draw the dinosaur - instead of having one / dino, we have 1 per type.
    // drawback : same animation for each type
    // advantage : less memory used - better perfs probably
    DinosaurHierarchy *m_hierarchy_1;
    DinosaurHierarchy *m_hierarchy_death1;
    DinosaurHierarchy *m_hierarchy_2;

    // meteorite
    Meteorite meteor;
    const int meteoritesNumber = 50;
    std::vector<Meteorite> listMeteorites;
    
    // grid dinosaurs
    Grid *m_grid;


};

#endif
