#pragma once
#include "math.h"
#include "main/scene_base/base.hpp"
#include "scenes/3D_graphics/Projet/Planet/Planet.hpp"

class Meteorite {

    protected:

    // movement
    vcl::vec3 m_speed;
    vcl::vec3 m_position;

    // interpolation - in order to not update each frame
    static constexpr float updateTimePeriod = 0.02f; // update time
    static constexpr float updateTimePeriod_2 = 1.0f; // update time - time before we consider we have re-entered the planet
    float m_dt = 0.0f;
    float m_dt_2 = 0.0f;

    float m_speedNorm;
    float m_distancePlanetCenter;
    float m_scale; // radius meteorite
    vcl::mesh_drawable m_mesh;
    
    float MinSpeedNorm = 0.01f; // min  norm before we consider it dead
    bool m_isDead = false; // used to stop updating it once it had stopped its movmeent
    
    // planet
    float scalePlanet;
    float g_planet;
    
    public:

    Meteorite();
    Meteorite(Planet *planet,float scaleFactor);
    Meteorite(float initialDistance, float initialSpeed, Planet *planet,float scaleFactor);
    void UpdatePosition(float dt, Planet *planet);
    void makeMesh(bool noise);
    vcl::mesh_drawable getMesh();
};