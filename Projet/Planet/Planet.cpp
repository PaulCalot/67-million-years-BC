
#include "math.h"
#include "scenes/3D_graphics/Projet/Planet/Planet.hpp"


Planet::Planet(std::vector<int> resolutions, std::vector<vcl::vec3> localsUp,  std::vector<vcl::mesh> meshes, float scale) : m_radius(scale) {
    m_g = 5*m_radius ; // TODO : include it in a new constructor as a parameter
    for (int i(0); i<6; i++){
        TerrainFace terrainFace = TerrainFace(resolutions[i], localsUp[i], meshes[i], i, scale); // i is the face number
        m_terrainFaces.push_back(terrainFace);
    }

}


float Planet::getRadius(){
    return m_radius; 
}

float Planet::getG(){
    return m_g;
}

std::vector<TerrainFace>* Planet::getTerrainFaces(){
    return &m_terrainFaces;
}   


int Planet::UpdatePositionTerrainFaceIdx(vcl::vec3 positionOnUnitSphere){
    int terrainFaceIdx(0);
    float x(0.0f);
    int sgn_x(0);
    float y(0.0f);
    int sgn_y(0);
    float z(0.0f);
    int sgn_z(0);
    if(positionOnUnitSphere.x > 0){
        x = positionOnUnitSphere.x;
        sgn_x = 1;
    }
    else {
        x= -positionOnUnitSphere.x;
        sgn_x = -1;
    }
    if(positionOnUnitSphere.y > 0){
        y = positionOnUnitSphere.y;
        sgn_y = 1;
    }
    else {
        y = -positionOnUnitSphere.y;
        sgn_y = -1;
    }if(positionOnUnitSphere.z > 0){
        z = positionOnUnitSphere.z;
        sgn_z = 1;
    }
    else {
        z = -positionOnUnitSphere.z;
        sgn_z = -1;
    }

    if(x > y){
        if(x>z){
            if(sgn_x == 1){
                terrainFaceIdx = 0; //+x
            }
            else {
                terrainFaceIdx = 1; // -x
            }
        }
        else if(z>x){
            if(sgn_z == 1){
                terrainFaceIdx = 4; //+z
            }
            else {
                terrainFaceIdx = 5; // -z
            }
        }
    }
    else{
        if(y>z){
            if(sgn_x == 1){
                terrainFaceIdx = 2; //+y
            }
            else {
                terrainFaceIdx = 3; // -y
            }
        }
        else if(z>y){
            if(sgn_z == 1){
                terrainFaceIdx = 4; //+z
            }
            else {
                terrainFaceIdx = 5; // -z
            }
        }
    }
    return terrainFaceIdx;
}
