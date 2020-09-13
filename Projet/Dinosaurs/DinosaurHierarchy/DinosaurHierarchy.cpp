#include "scenes/3D_graphics/Projet/Dinosaurs/DinosaurHierarchy/DinosaurHierarchy.hpp"
#include "scenes/3D_graphics/Projet/Utilities/Utilities.hpp"


DinosaurHierarchy::DinosaurHierarchy(){
}
DinosaurHierarchy::DinosaurHierarchy(Planet *planet, int type): m_planet(planet) {
    
    ScalePlanet = planet -> getRadius();
    m_scale = ScalePlanet*RatioDinoPlanet;
    buildHierarchy();
    if(type == -1){
        //death
        updateMesh(1.0, texture_id_1);

    }
    else if(type == 1){
        updateMesh(1.0, texture_id_1);

    }
    // the case 1 is done by default
    else if(type == 2){ // type 2 dino
        updateMesh(2.0, texture_id_2);
    }   
    
}

void DinosaurHierarchy::buildHierarchy(){

        // dinosaur hierarchy 
        // cf. page 113 of the book for the inspirition image
        // we'll draw everything with cone to start with

        // scale of the dinosaur (should be correlated to the size / scale of the planet)
    float scale(m_scale);
        // lenght
    float l_body(scale*1.0);
    float l_head(scale*0.3);
    float l_leg(scale*1.0); // use for top leg and bottom leg
    float l_joint_body_leg(scale*0.2);
    float l_joint_body_head(scale*0.3);
    float l_joint_body_arm(scale*0.3);
    float l_arm(scale*0.05);
    float l_tail_1(scale*1.0);
    float l_tail_2(scale*0.5);
    float l_tail_3(scale*0.2);

        // radius
    float factor(1/5.0);
    float r_body(l_body*factor);
    float r_head(l_head*factor);
    float r_leg(l_leg*factor); // use for top leg and bottom leg
    float r_joint_body_leg(l_joint_body_leg*factor);
    float r_joint_body_head(l_joint_body_head*factor);
    float r_joint_body_arm(l_joint_body_arm*factor);
    float r_arm(l_arm*factor);
    float r_tail_1(l_tail_1*factor);
    float r_tail_2(l_tail_2*factor);
    float r_tail_3(l_tail_3*factor);


    // every cone is first defined along +z axis

    // we could probably make it more efficient and diminish the resolution which is by default at something like 20*20 = 400 triangles ...
    // which is not really smart since we are generally far from the planet.
    mesh_drawable body = mesh_drawable(mesh_primitive_cone(r_body, {0,0,0}, {0.0,0.0,l_body}) ); 
    mesh_drawable head = mesh_drawable(mesh_primitive_cone(r_head, {0,0,0}, {0.0,0.0,l_head}) ); 
    mesh_drawable leg = mesh_drawable(mesh_primitive_cone(r_leg, {0,0,0}, {0.0,0.0,l_leg}) ); 
    mesh_drawable joint_body_head = mesh_drawable(mesh_primitive_cone(r_joint_body_head, {0,0,0}, {0.0,0.0,l_joint_body_head}) );
    mesh_drawable joint_body_leg = mesh_drawable(mesh_primitive_cone(r_joint_body_leg, {0,0,0}, {0.0,0.0,l_joint_body_leg}) ); 
    mesh_drawable joint_body_arm = mesh_drawable(mesh_primitive_cone(r_joint_body_arm, {0,0,0}, {0.0,0.0,l_joint_body_arm}) );
    mesh_drawable arm = mesh_drawable(mesh_primitive_cone(r_arm, {0,0,0}, {0.0,0.0,l_arm}) ); 
    mesh_drawable tail_1 = mesh_drawable(mesh_primitive_cone(r_tail_1, {0,0,0}, {0.0,0.0,l_tail_1}) );
    mesh_drawable tail_2 = mesh_drawable(mesh_primitive_cone(r_tail_2, {0,0,0}, {0.0,0.0,l_tail_2}) );
    mesh_drawable tail_3 = mesh_drawable(mesh_primitive_cone(r_tail_3, {0,0,0}, {0.0,0.0,l_tail_3}) );
   // Syntax to add element
    //m_hierarchy.add(visual_element, element_name, parent_name, (opt)[translation, rotation])
    m_hierarchy.add(body, "body");

    // head
        // I could have defined everything with only a mesh_drawable - since we can use affine_transform 
    //rotation_from_axis_angle_mat3({0,0,1}, angle) 
    m_hierarchy.add(joint_body_head, "joint_body_head", "body", {0,0,l_body});
    m_hierarchy.add(head,"head","joint_body_head",{0,0,l_joint_body_head}); // maybe I should add a rotation at some point

    // arms

        // we suppose that the dinosaur body is along +z axis, the +y axis is directed toward its front, and the +x is directed towards it right
    affine_transform transform_body_to_arm_right({0,0,0}, rotation_from_axis_angle_mat3({0,1,0}, M_PI/2) );// first translation and then rotation
    affine_transform transform_body_to_arm_left({0,0,0}, rotation_from_axis_angle_mat3({0,1,0}, -M_PI/2) );// first translation and then rotation
    m_hierarchy.add(joint_body_arm,"joint_body_arm_left","joint_body_head",transform_body_to_arm_left);
    m_hierarchy.add(joint_body_arm,"joint_body_arm_right","joint_body_head",transform_body_to_arm_right);

    

    affine_transform transform_arm_left({0,0,l_joint_body_arm}, rotation_from_axis_angle_mat3({-1,0,0}, M_PI/2) );// first translation and then rotation
    affine_transform transform_arm_right({0,0,l_joint_body_arm}, rotation_from_axis_angle_mat3({-1,0,0}, M_PI/2) );// first translation and then rotation
    m_hierarchy.add(arm,"arm_left","joint_body_arm_left",transform_arm_left);
    m_hierarchy.add(arm,"arm_right","joint_body_arm_right",transform_arm_right);

    // legs
    vcl::mat3 R_right = rotation_from_axis_angle_mat3({0,1,0}, M_PI/2);
    vcl::mat3 R_left = rotation_from_axis_angle_mat3({0,1,0},  -M_PI/2);
    affine_transform transform_body_to_leg_right({r_body,0,0}, R_right);// first translation and then rotation
    affine_transform transform_body_to_leg_left({-r_body,0,0}, R_left);// first translation and then rotation
    m_hierarchy.add(joint_body_leg,"joint_body_leg_left","body",transform_body_to_leg_left);
    m_hierarchy.add(joint_body_leg,"joint_body_leg_right","body",transform_body_to_leg_right);

    vcl::mat3 R_right2 = rotation_from_axis_angle_mat3({0,1,0}, -M_PI/2) * rotation_from_axis_angle_mat3({1,0,0}, -M_PI/3) ;
    vcl::mat3 R_left2 = rotation_from_axis_angle_mat3({0,1,0}, M_PI/2) * rotation_from_axis_angle_mat3({1,0,0}, -M_PI/3);

    affine_transform transform_top_leg_right({0,0,l_joint_body_leg},R_right2);// first translation and then rotation
    affine_transform transform_top_leg_left({0,0,l_joint_body_leg},R_left2);// first translation and then rotation
    m_hierarchy.add(leg,"top_leg_left","joint_body_leg_left",transform_top_leg_left);
    m_hierarchy.add(leg,"top_leg_right","joint_body_leg_right",transform_top_leg_right);

    affine_transform transform_bottom_leg_right({0,0,l_leg},rotation_from_axis_angle_mat3({1,0,0}, -M_PI/3));// first translation and then rotation
    affine_transform transform_bottom_leg_left({0,0,l_leg},rotation_from_axis_angle_mat3({1,0,0}, -M_PI/3));// first translation and then rotation
    m_hierarchy.add(leg,"bottom_leg_left","top_leg_left",transform_bottom_leg_left);
    m_hierarchy.add(leg,"bottom_leg_right","top_leg_right",transform_bottom_leg_right);

    // tail
    affine_transform transform_body_to_tail({0,0,0}, rotation_from_axis_angle_mat3({1,0,0}, M_PI) );// first translation and then rotation
    m_hierarchy.add(tail_1, "tail_1", "body", transform_body_to_tail);
    m_hierarchy.add(tail_2, "tail_2", "tail_1", {0,0,l_tail_1});
    m_hierarchy.add(tail_3, "tail_3", "tail_2", {0,0,l_tail_2});

    //
}

void DinosaurHierarchy::animate(float t){
    // TODO: save the constants
    
    double t_ =(t - t_max_2)*alpha;
    
    float angle_left = eq_pos-m_factor1*sin(t_);
    float angle_right = eq_pos+m_factor1*sin(t_);
  
    m_hierarchy["joint_body_leg_left"].transform.rotation = rotation_from_axis_angle_mat3({1,0,0}, angle_left)*m_R_left; 
    m_hierarchy["joint_body_leg_right"].transform.rotation = rotation_from_axis_angle_mat3({1,0,0}, angle_right)*m_R_right;

    // moving the tail
    float angle = 0.1*M_PI_2*sin(t_);
    vcl::mat3 R = rotation_from_axis_angle_mat3({0,1,0},angle);
    vcl::mat3 R_ = rotation_from_axis_angle_mat3({0,-1,0},angle);

    m_hierarchy["tail_1"].transform.rotation = R*m_R;
    m_hierarchy["tail_2"].transform.rotation = R_;
    m_hierarchy["tail_3"].transform.rotation = R_;

}

void DinosaurHierarchy::animateDeath(vcl::vec3 positionOnPlanet){
    m_hierarchy["body"].transform.rotation = RotationOnPlanet({1,0,0},positionOnPlanet); 
}
 
void DinosaurHierarchy::updateMesh(vcl::vec3 translation, vcl::mat3 rotation){

    m_hierarchy["body"].transform.translation = translation;
    m_hierarchy["body"].transform.rotation = rotation;

}

void DinosaurHierarchy::update_local_to_global_coordinates(){
    m_hierarchy.update_local_to_global_coordinates();
}


void DinosaurHierarchy::updateMesh(float scale, GLuint texture_id){

    int N = m_hierarchy.elements.size();
    m_hierarchy["body"].transform.scaling = scale;
    for(int k=0; k<N; ++k){
        m_hierarchy.elements[k].element.texture_id = texture_id;
    }

}


// Getter and setter

vcl::hierarchy_mesh_drawable DinosaurHierarchy::getMeshDrawable(){
    return m_hierarchy;
} 


