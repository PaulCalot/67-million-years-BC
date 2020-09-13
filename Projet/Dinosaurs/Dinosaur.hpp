#pragma once

#include "main/scene_base/base.hpp"
#include "scenes/3D_graphics/Projet/Planet/Planet.hpp"
#include "scenes/3D_graphics/Projet/Utilities/Utilities.hpp"

using namespace vcl;


class Dinosaur {

    protected: 

    static constexpr float RatioDinoPlanet = 0.02f; // dino = 1/50 planet (for his body part - see DinosaurHiearchy)
    // -> this is the same as in the DinosaurHiearchy class - make it extern ?

    // different scales
    float ScalePlanet; // which is also the scale of the face (that apepars in TerrainFace)
    float m_scale; // scale of dino
    int typeDinosaur; // 1 is Chilesaurus - 2 for T-REX

    Planet *m_planet;

    unsigned int m_id; // id dino        
    unsigned int m_terrainFaceIdx; // terrain face on which the dino is

        // Moving + herd behavior
    vcl::vec3 m_positionOnUnitSphere;
    vcl::vec3 m_positionOnPlanet;
    vcl::vec3 m_orientationOnPlanet; // the vector along which the dino is currently looking
    vcl::vec3 m_velocity; // on the planet
    float m_speedNorm;
    float MaxSpeed;
    float MinSpeed;
    
            // different coefs used for the group behavior
    static constexpr float m_coefHerdDirection = 1.0f; // speed of the dinos tend to become the same
    static constexpr float m_coefHerdCentre = 1.0f; // they pack 
    static constexpr float m_coefAntiCollisionDirection = 1.0f; // but avoid collision
    static constexpr float MaxForce = 3.0f;
    static constexpr float mult_avoidRadius = 2.0f; // to be multiplied by the scale of the dino to get the true radius
    static constexpr float mult_viewRadius = 10.0f; 
    static constexpr float mult_Speed = 6.0;
    
    unsigned int m_herdNeighbors; // neightbors in the herd
    vcl::vec3 m_herdDirection;
    vcl::vec3 m_herdCentre;
    vcl::vec3 m_antiCollisionDirection;
    float avoidRadius;
    float avoidRadius2;
    float viewRadius;
    
    bool m_isDead;
    bool m_special; // could be "FLEEE" for the chilosaurus, or "ATTACK" for the Trex 
    float m_deltaTimeSinceSpecial; 
    static constexpr float specialDelayTime = 3.0f; // time for m_special effect to last 
    static constexpr float angleVariationDelta = M_PI/2.0f; // max variation in 1 sec while in random deplacement

    //static constexpr float updateTimePeriod = 0.0001f; // as little as we can ... // not used anymore 
    //static constexpr float AngleOfView = M_PI/2.0f;  // not used anymore (it should be added in the UpdatePosition of the chilosaurus and trex classes)


    // useful for drawing
    vcl::vec3 m_translation;
    vcl::mat3 m_rotation;
    float factorOnSurface; // multiplicatif factor to assert the dino will appear on the surface (and not with a part of it in the ground)
   
   
    // the position of the dino in the grid - ignore warning because it is used on in the grid class
    GridPosition m_positionInGrid;

    
    
    
    public:
        // constructors
    Dinosaur();
    Dinosaur(vcl::vec2 startingPositionFace, unsigned int terrainFaceInx, Planet *planet, unsigned int id); 
    virtual ~Dinosaur();

    // movement
    void initPosition(vcl::vec2 startingPositionFace);
    void UpdatePosition(float dt);
    vcl::vec3 thrustToward(vcl::vec3 vect);
    void limitSpeed();
    virtual void UpdatePosition(float dt,  std::list<Dinosaur*> listDinos);  // declared here but not defined - it's used for polymorphism 


    bool operator == (const Dinosaur& s) const { return m_id == s.m_id; }
    bool operator != (const Dinosaur& s) const { return !operator==(s); }

    // ------------- Getter and Setter ---------- //
    vcl::vec3 getPositionOnSphereUnit();
    bool getIsDead();
    void setIsDead();
    int getDinoType();
    unsigned int getFaceIdx();
    vcl::vec3 getTranslation();
    vcl::mat3 getRotation();
    vcl::vec3 getPositionOnPlanet();
    void setPositionInGrid(GridPosition positionInGrid);
    GridPosition getPositionInGrid();
    unsigned int getId();
    vcl::vec3 getOrientation();
    
    // useless ones 

   // void checkCoordinate();
   // void UpdateRelativePosition(float dTheta);



};

