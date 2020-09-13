#pragma once

#include "main/scene_base/base.hpp"
#include "scenes/3D_graphics/Projet/Planet/TerrainFace.hpp"
using namespace vcl;

class Planet {
    protected:
    std::vector<TerrainFace> m_terrainFaces; // already initialized with a default constructor
    float m_radius;
    float m_g;

    public:
    std::vector<TerrainFace>* getTerrainFaces();
    Planet(std::vector<int> resolutions, std::vector<vcl::vec3> localsUp,  std::vector<vcl::mesh> meshes, float scale);
    float getRadius();
    float getG();
    int UpdatePositionTerrainFaceIdx(vcl::vec3 positionOnUnitSphere);
};