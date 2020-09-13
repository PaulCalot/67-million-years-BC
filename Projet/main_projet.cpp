#include "scenes/3D_graphics/Projet/main_projet.hpp"

#ifdef SCENE_PROJECT

using namespace vcl;
#include <math.h>

// setup_data 

void scene_model::setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui)
    {

        // Setup initial camera mode and position
    scene.camera.camera_type = camera_control_trackball;
    //scene.camera.apply_rotation(0,0,0,1.2f);
        
        // Setup Planet
    std::vector<int> resolutions = {100,100,100,100,100,100};
    std::vector<vcl::vec3> localsUp = {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}}; // +x, -x, +y, -y, +z, -z 
    vcl::mesh meshx;
    vcl::mesh mesh_x;
    vcl::mesh meshy;
    vcl::mesh mesh_y;
    vcl::mesh meshz;
    vcl::mesh mesh_z;
    std::vector<vcl::mesh> meshes = {
        meshx, //+x
        mesh_x, // -x
        meshy,
        mesh_y,
        meshz,
        mesh_z
    };
    m_planet = new Planet(resolutions, localsUp, meshes, Scale); 
    m_planetTerrain = *(m_planet -> getTerrainFaces());

    for(int i(0); i<6; i++){
        mesh terrain = m_planetTerrain[i].getMesh();
        m_planetDrawable.push_back(vcl::mesh_drawable(terrain));
        
        //m_planetDrawable[i].uniform.color = {0.0f, 0.4+0.1f*i,0.0f};
        m_planetDrawable[i].uniform.shading.specular = 0.0f; 
        m_planetDrawable[i].uniform.shading.diffuse = 0.1f;
        m_planetDrawable[i].uniform.shading.ambiant = 0.9f;
        m_planetDrawable[i].texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/colors_red_planet.png"), GL_REPEAT, GL_REPEAT);
    }



    // skybox 
    float s = scene_model::Scale*20; // view distance

    mesh skybox_west_cpu; // looking west (along -x)
    skybox_west_cpu.position     = {{-s,-s,s}, {-s,-s,-s}, {-s,s,s}, {-s,s,-s}}; // depends on the orientation 
    skybox_west_cpu.texture_uv   = {{0.0, 1/3.0}, {0.0, 2/3.0}, {1/4.0, 1/3.0 }, {1/4.0, 2/3.0}};
    skybox_west_cpu.connectivity = {{0,1,2}, {2,1,3}}; 

    mesh skybox_east_cpu; // looking east (along +x)
    skybox_east_cpu.position     ={{s,s,s}, {s,s,-s}, {s,-s,s}, {s,-s,-s}}; // depends on the orientation 
    skybox_east_cpu.texture_uv   = {{1/2.0, 1/3.0}, {1/2.0, 2/3.0}, {3/4.0, 1/3.0}, {3/4.0, 2/3.0}};
    skybox_east_cpu.connectivity = {{2,0,1}, {3,2,1}}; 
    

    mesh skybox_top_cpu; // looking top (along +z)
    skybox_top_cpu.position     = {{-s,-s,s}, {s,-s,s}, {-s,s,s}, {s,s,s}}; // depends on the orientation 
    skybox_top_cpu.texture_uv   = {{1/4.0, 0.0}, {1/2.0, 0.0}, {1/4.0, 1/3.0}, {1/2.0, 1/3.0}};
    skybox_top_cpu.connectivity = {{2,1,0}, {3,1,2}}; // not sure of it yet, let's see
    
    mesh skybox_bottom_cpu; // looking bottom (along -z)
    skybox_bottom_cpu.position     =  {{-s,-s,-s}, {s,-s,-s}, {-s,s,-s}, {s,s,-s}};  // depends on the orientation 
    skybox_bottom_cpu.texture_uv   = {{1/4.0, 1.0}, {1/2.0, 1.0}, {1/4.0, 2/3.0}, { 1/2.0, 2/3.0}};
    skybox_bottom_cpu.connectivity = {{0,1,2}, {2,1,3}}; // not sure of it yet, let's see
    
    mesh skybox_north_cpu; // looking west (along +y)
    skybox_north_cpu.position     = {{-s,s,s}, {-s,s,-s}, {s,s,s}, {s,s,-s}}; // depends on the orientation 
    skybox_north_cpu.texture_uv   =  {{1/4.0, 1/3.0 }, {1/4.0, 2/3.0}, {1/2.0, 1/3.0}, { 1/2.0, 2/3.0}};
    skybox_north_cpu.connectivity = {{1,2,0}, {1,3,2}}; // not sure of it yet, let's see
    

    mesh skybox_south_cpu; // looking west (along -y)
    skybox_south_cpu.position     = {{s,-s,s}, {s,-s,-s}, {-s,-s,s}, {-s,-s,-s}}; // depends on the orientation 
    skybox_south_cpu.texture_uv   = {{3/4.0, 1/3.0}, {3/4.0, 2/3.0}, {1.0, 1/3.0}, {1.0, 2/3.0}};
    skybox_south_cpu.connectivity = {{1,2,0}, {1,3,2}}; // not sure of it yet, let's see
   
        // 1st skybox - space one
    skybox_west = skybox_west_cpu;
    skybox_west.uniform.shading.specular = 0.0f; // non-specular terrain material
    skybox_west.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/space_skybox.png"), GL_REPEAT, GL_REPEAT);

    skybox_east = skybox_east_cpu;
    skybox_east.uniform.shading.specular = 0.0f; // non-specular terrain material
    skybox_east.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/space_skybox.png"), GL_REPEAT, GL_REPEAT);

    skybox_top = skybox_top_cpu;
    skybox_top.uniform.shading.specular = 0.0f;
    skybox_top.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/space_skybox.png"), GL_REPEAT, GL_REPEAT);

    skybox_bottom = skybox_bottom_cpu;
    skybox_bottom.uniform.shading.specular = 0.0f; 
    skybox_bottom.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/space_skybox.png"), GL_REPEAT, GL_REPEAT);

    skybox_north = skybox_north_cpu;
    skybox_north.uniform.shading.specular = 0.0f; 
    skybox_north.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/space_skybox.png"), GL_REPEAT, GL_REPEAT);

    skybox_south = skybox_south_cpu;
    skybox_south.uniform.shading.specular = 0.0f; 
    skybox_south.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/space_skybox.png"), GL_REPEAT, GL_REPEAT);

        // 2nd skybox - sky one (useful for 3rd person dino). TODO: find a way to change the night/day depending on where we are on the planet
        // skybox may do it; but I did not find any good one.

    skybox2_west = skybox_west_cpu;
    skybox2_west.uniform.shading.specular = 0.0f; // non-specular terrain material
    skybox2_west.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/dayanday.png"), GL_REPEAT, GL_REPEAT);

    skybox2_east = skybox_east_cpu;
    skybox2_east.uniform.shading.specular = 0.0f; // non-specular terrain material
    skybox2_east.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/dayanday.png"), GL_REPEAT, GL_REPEAT);

    skybox2_top = skybox_top_cpu;
    skybox2_top.uniform.shading.specular = 0.0f;
    skybox2_top.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/dayanday.png"), GL_REPEAT, GL_REPEAT);

    skybox2_bottom = skybox_bottom_cpu;
    skybox2_bottom.uniform.shading.specular = 0.0f; 
    skybox2_bottom.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/dayanday.png"), GL_REPEAT, GL_REPEAT);

    skybox2_north = skybox_north_cpu;
    skybox2_north.uniform.shading.specular = 0.0f; 
    skybox2_north.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/dayanday.png"), GL_REPEAT, GL_REPEAT);

    skybox2_south = skybox_south_cpu;
    skybox2_south.uniform.shading.specular = 0.0f; 
    skybox2_south.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/dayanday.png"), GL_REPEAT, GL_REPEAT);

    // dinosaurs

        // grid
    m_grid = new Grid(10); // 10*10 per face - TODO: lowering it below a certain limit changes the group bahavior of the dinos (since there are more or less dinos in the sight of dinos)
    
        // 3 hierarchies
    m_hierarchy_1 = new DinosaurHierarchy(m_planet, 1);
    m_hierarchy_2 = new DinosaurHierarchy(m_planet, 2);
    m_hierarchy_death1 = new DinosaurHierarchy(m_planet, -1);


    // creating dinos 
    for (int k(0); k < ChilesauriNumber; k++){
        //int idx = (int) (5* rand() / (RAND_MAX)); // for random spot on the map
        float x = ((double) rand() / (RAND_MAX));
        float y = ((double) rand() / (RAND_MAX));
        m_Dinosaurs[k] = new Chilesaurus({x,y}, 4, m_planet, k); // they all start on the same face to start with
        m_grid -> addToGrid(m_Dinosaurs[k]);
    }
    for (int k(0); k < TyrannosauriNumber; k++){
       // int idx = (int) (5* rand() / (RAND_MAX));
        float x = ((double) rand() / (RAND_MAX));
        float y = ((double) rand() / (RAND_MAX));
        m_Dinosaurs[k+ChilesauriNumber] = new Tyrannosaurus({x,y}, 3, m_planet,k+ChilesauriNumber); // they all start on the same face to start with
        m_grid -> addToGrid(m_Dinosaurs[k+ChilesauriNumber]);
    }

    // init parameters useful to track the number of dinos
    DinosaursNumber = ChilesauriNumber+TyrannosauriNumber;
    m_remaining_1 = ChilesauriNumber;
    m_remaining_2 = ChilesauriNumber+TyrannosauriNumber;
    m_dead_1 = 0;

    // animation of the dino
    timer.t_min = 0.0;
    timer.t_max = 1.0;

    // camera
    dino_cam = m_Dinosaurs[0];
    dino_cam2 = m_Dinosaurs[DinosaursNumber-1];
    timer_camera.t_min = 0.0;
    timer_camera.t_max = 50.0;
    
    // meteor
    meteor = Meteorite(m_planet, 0.1);   // the big one
    // the smaller one
    for (int k(0);  k<meteoritesNumber; k++){
        float x = ((double) rand() / (RAND_MAX));
        float y = ((double) rand() / (RAND_MAX));
        float initSpeed = Scale * (2.5+0.2*x);
        float initPos = Scale *(2+3*y);
        listMeteorites.push_back(Meteorite(initPos, initSpeed, m_planet, 0.01));
    }

}

void scene_model::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui){
    set_gui();
    
    
    // camera
    timer_camera.update();
    if(timer_camera.t<40){
        scene.camera.translation = {0,0,0};
        scene.camera.scale = 1.3*Scale + (40- timer_camera.t);
        scene.camera.orientation =rotation_from_axis_angle_mat3({0,1,0}, M_PI/16*timer_camera.t);
    }
    else if(timer_camera.t<45){
        scene.camera.scale = 1.0f;
        int k = 0;
        while(dino_cam->getIsDead()){
            dino_cam = m_Dinosaurs[k++];
        }
        scene.camera.scale = 1.0f;
        scene.camera.orientation =dino_cam->getRotation()*rotation_from_axis_angle_mat3({0,0,1}, M_PI)*rotation_from_axis_angle_mat3({0,1,0}, M_PI);
        //scene.camera.translation = -1.006*dino.getTranslation()-1.1*dino.getOrientation();
        scene.camera.translation = -dino_cam->getTranslation()-0.5*dino_cam->getOrientation();
    }
    else{
        scene.camera.orientation =dino_cam2->getRotation()*rotation_from_axis_angle_mat3({0,0,1}, M_PI)*rotation_from_axis_angle_mat3({0,1,0}, M_PI);
        scene.camera.translation = -1.01*dino_cam2->getTranslation()-0.5*dino_cam2->getOrientation();
        //scene.camera.translation = -1.01*dino.getTranslation()-1.2*dino.getOrientation();

    }
    
   

    // display skybox

    vcl::vec3 cameraPosition = {scene.camera.camera_position().x, scene.camera.camera_position().y,scene.camera.camera_position().z};
    if(norm(cameraPosition) > Scale * 1.5){ 
        skybox_west.uniform.transform.translation  = scene.camera.camera_position();
        skybox_east.uniform.transform.translation  = scene.camera.camera_position();
        skybox_top.uniform.transform.translation  = scene.camera.camera_position();
        skybox_north.uniform.transform.translation  = scene.camera.camera_position();
        skybox_south.uniform.transform.translation  = scene.camera.camera_position();
        skybox_bottom.uniform.transform.translation  = scene.camera.camera_position();

        draw(skybox_west, scene.camera, shaders["mesh"]);
        draw(skybox_east, scene.camera, shaders["mesh"]);
        draw(skybox_top, scene.camera, shaders["mesh"]);
        draw(skybox_north, scene.camera, shaders["mesh"]);
        draw(skybox_south, scene.camera, shaders["mesh"]);
        draw(skybox_bottom, scene.camera, shaders["mesh"]);  
    }
    else{
        skybox2_west.uniform.transform.translation  = scene.camera.camera_position();
        skybox2_east.uniform.transform.translation  = scene.camera.camera_position();
        skybox2_top.uniform.transform.translation  = scene.camera.camera_position();
        skybox2_north.uniform.transform.translation  = scene.camera.camera_position();
        skybox2_south.uniform.transform.translation  = scene.camera.camera_position();
        skybox2_bottom.uniform.transform.translation  = scene.camera.camera_position();

        draw(skybox2_west, scene.camera, shaders["mesh"]);
        draw(skybox2_east, scene.camera, shaders["mesh"]);
        draw(skybox2_top, scene.camera, shaders["mesh"]);
        draw(skybox2_north, scene.camera, shaders["mesh"]);
        draw(skybox2_south, scene.camera, shaders["mesh"]);
        draw(skybox2_bottom, scene.camera, shaders["mesh"]);  
    }
    
    
    for(int i(0); i<6; i++){
        draw(m_planetDrawable[i], scene.camera,shaders["mesh"]);

     //   if(gui_scene.wireframe) // Display the hierarchy as wireframe
     //     draw(m_planetDrawable[i], scene.camera, shaders["wireframe"]);
    }
    
    
    float old_time = timer.t;
    timer.update();
    const float t = timer.t;

    // computing dt - usef for physics part (to make physcis not depends on frames rate)
    float dt = t - old_time;
    if(dt<0){
        dt = timer.t_max - old_time + t - timer.t_min;
    }

    for (int k(0); k<m_dead_1 ; k++){ // the dead ones
        m_hierarchy_death1 -> updateMesh(m_Dinosaurs[k]->getTranslation(), m_Dinosaurs[k]->getRotation());
        m_hierarchy_death1 -> animateDeath(m_Dinosaurs[k]->getPositionOnPlanet());
        m_hierarchy_death1 -> update_local_to_global_coordinates();
        draw(m_hierarchy_death1 -> getMeshDrawable(), scene.camera,shaders["mesh"]);
    }

    m_hierarchy_1-> animate(t); // we animated the hierarchy for the type 1 dino
    for (int k(m_dead_1); k < m_remaining_1; k++){ // type 1

        // update position
        std::list<Dinosaur*> listDinos = m_grid->getGrid(m_Dinosaurs[k]); // get the dinos close enough of the current one
        m_Dinosaurs[k]-> UpdatePosition(dt, listDinos); // polymorphism 

        // update mesh and draw
        m_hierarchy_1 -> updateMesh(m_Dinosaurs[k]->getTranslation(), m_Dinosaurs[k]->getRotation());
        m_hierarchy_1 -> update_local_to_global_coordinates();
        draw(m_hierarchy_1 -> getMeshDrawable(), scene.camera,shaders["mesh"]);
    
    }

    m_hierarchy_2-> animate(t); // type 2
    for (int k(m_remaining_1); k < m_remaining_2; k++){

        // same as before
        std::list<Dinosaur*> listDinos = m_grid->getGrid(m_Dinosaurs[k]);
        m_Dinosaurs[k]-> UpdatePosition(dt,listDinos); 
        m_hierarchy_2 -> updateMesh(m_Dinosaurs[k]->getTranslation(), m_Dinosaurs[k]->getRotation());
        m_hierarchy_2 -> update_local_to_global_coordinates();
        draw(m_hierarchy_2 -> getMeshDrawable(), scene.camera,shaders["mesh"]);
    }

    int death = 0;
    for (int k(m_dead_1); k < m_remaining_1; k++){

        if(m_Dinosaurs[k] -> getIsDead()){
            m_grid -> remove(m_Dinosaurs[k]); // removed from the grid

            // swaping with the live ones (dead dinos are always at the front of the list)
            Dinosaur *tmp = m_Dinosaurs[death+m_dead_1]; 
            m_Dinosaurs[death+m_dead_1] = m_Dinosaurs[k]; 
            m_Dinosaurs[k] = tmp;
            death+=1;
        }
    }    
    m_dead_1 = m_dead_1 + death;

    // update grid with new coordinates
    for(int k(m_dead_1); k<DinosaursNumber; k++){
        m_grid->updateGrid(m_Dinosaurs[k]);
    }

        // meteor
    meteor.UpdatePosition(dt, m_planet); 
    draw(meteor.getMesh(),scene.camera,shaders["mesh"]);
    for (int k(0); k<meteoritesNumber; k++){
        listMeteorites[k].UpdatePosition(dt, m_planet);
        draw(listMeteorites[k].getMesh(),scene.camera,shaders["mesh"]);
    }
    
}


void scene_model::set_gui()
{
    ImGui::Text("Display: "); ImGui::SameLine();
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe); ImGui::SameLine();
    ImGui::Checkbox("Surface", &gui_scene.surface);     ImGui::SameLine();
    ImGui::Checkbox("Skeleton", &gui_scene.skeleton);   ImGui::SameLine();

    ImGui::Spacing();
    ImGui::SliderFloat("Time", &timer.t, timer.t_min, timer.t_max);
    ImGui::SliderFloat("Time scale", &timer.scale, 0.1f, 3.0f);
}



#endif