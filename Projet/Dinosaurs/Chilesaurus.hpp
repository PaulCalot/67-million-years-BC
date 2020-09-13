#pragma once
#include "scenes/3D_graphics/Projet/Dinosaurs/Dinosaur.hpp"
#include "main/scene_base/base.hpp"

using namespace vcl;

class Chilesaurus : public Dinosaur { 
    
    protected :
    
    
    public:
    Chilesaurus(vcl::vec2 startingPositionFace, int terrainFaceInx, Planet *planet, unsigned int id );
    virtual ~Chilesaurus();
    virtual void UpdatePosition(float dt, std::list<Dinosaur*>listDinos); 

};