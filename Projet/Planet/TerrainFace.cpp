
#include "scenes/3D_graphics/Projet/Planet/TerrainFace.hpp"
#include "math.h"


TerrainFace::TerrainFace(int resolution, vcl::vec3 localUp, vcl::mesh mesh, int face, float scale): m_scale(scale), m_face(face), m_resolution(resolution), m_localUp(localUp){
    m_axisA = vcl::vec3(localUp.y, localUp.z, localUp.x);
    m_axisB = vcl::cross(localUp, m_axisA);
    ConstructMesh();
}

void TerrainFace::ConstructMesh(){
    m_mesh.position     = {};                       
    m_mesh.normal       = {};                          
    m_mesh.texture_uv   = {};
    m_mesh.connectivity = {};           
    // we do push_back() on all of these

    unsigned int compteur = -1;


    for (int y(0); y<m_resolution; y++){
        for(int x(0); x<m_resolution; x++){
            compteur += 1;

                // point :
            vcl::vec2 percent = vcl::vec2(x,y)/(m_resolution-1);
            
            vcl::vec3 pointOnPlanet = TerrainFace::evaluate_terrain(percent);
          
            m_mesh.texture_uv.push_back({std::max(0.1,std::min(1.0,3*(norm(pointOnPlanet)-0.8*m_scale)/m_scale)), 0.5}); // trial and errors

            m_mesh.position.push_back(pointOnPlanet);

           
            if(x < m_resolution-1 && y < m_resolution && y>0){
                vcl::uint3 triangleA = {compteur, compteur - m_resolution + 1, compteur - m_resolution}; 
                vcl::uint3 triangleB  {compteur, compteur - m_resolution + 1, compteur + 1};

                m_mesh.connectivity.push_back(triangleB);
                m_mesh.connectivity.push_back(triangleA);
            
            }

        }
    }
}

vcl::mesh TerrainFace::getMesh(){
    return m_mesh;
}

int TerrainFace::getScale(){
    return m_scale;
}

vcl::vec3 TerrainFace::evaluate_terrain(vcl::vec2 percent)
{
    return evaluate_PointOnPlanet(evaluate_PointOnUnitSphere(percent));
}

vcl::vec3 TerrainFace::evaluate_PointOnUnitSphere(vcl::vec2 percent){
    float u = (percent.x - 0.5f)*2;
    float v = (percent.y - 0.5f)*2;

        // we place the point on the cube, the center of the cube is the center here too
    vcl::vec3 pointOnUnitCube = m_localUp +  u * m_axisA +  v * m_axisB;
    vcl::vec3 pointOnUnitSphere = normalize(pointOnUnitCube);
    return pointOnUnitSphere;
}

vcl::vec3 TerrainFace::evaluate_PointOnPlanet(vcl::vec3 pointOnUnitSphere){
    float scaling = 1; // bruit sur toute la planète
    int octave = 1;
    double persistency = 0.1; // good like that
    double M(0.8*m_scale);  // we include the m_scale here so the moutainous is the same (planet scale invariant)

    /* 2nd noise has been deleted because it was making the dinos wiggle which was visible at close range 
    float scaling_ = 1; // bruit sur toute la planète
    int octave_ = 20;
    double persistency_ = 1; // good like that
    double M_(40*m_scale);
    */
    
    const float noise3D = 1+(perlin(scaling*pointOnUnitSphere.x, scaling*pointOnUnitSphere.y, scaling*pointOnUnitSphere.z, octave, persistency)-1)/M;
   // const float noise3D_ = 1+(perlin(scaling_*pointOnUnitSphere.x, scaling_*pointOnUnitSphere.y, scaling_*pointOnUnitSphere.z, octave_, persistency_)-1)/M_;

    vcl::vec3 pointOnPlanet(m_scale*pointOnUnitSphere); // *z

    pointOnPlanet *= noise3D;
    return pointOnPlanet;
}

