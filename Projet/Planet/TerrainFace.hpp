#pragma once

#include "main/scene_base/base.hpp"


using namespace vcl;


class TerrainFace {
    vcl::mesh m_mesh;
    unsigned int m_resolution;
    vcl::vec3 m_localUp;
    vcl::vec3 m_axisA;
    vcl::vec3 m_axisB;
    int m_face; // this is the number of the face here.
    float m_scale; // this is a factor to multiply the scene in the end.

 
    public:
    TerrainFace(int resolution, vcl::vec3 localUp, vcl::mesh mesh, int face, float scale);  // constructor a the face
    void ConstructMesh();

    vcl::vec3 evaluate_terrain(vcl::vec2 vect);
    vcl::vec3 evaluate_PointOnUnitSphere(vcl::vec2 vect);
    vcl::vec3 evaluate_PointOnPlanet(vcl::vec3 vect3);

    // ----------- Setter / Getter --------- //
    int getScale();
    vcl::mesh getMesh();



   
};