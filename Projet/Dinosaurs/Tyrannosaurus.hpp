#pragma once
#include "scenes/3D_graphics/Projet/Dinosaurs/Dinosaur.hpp"
#include "main/scene_base/base.hpp"

#include <list>
using namespace vcl;

class Tyrannosaurus : public Dinosaur {
    
    protected :
    public:
    Tyrannosaurus(vcl::vec2 startingPositionFace, int terrainFaceInx, Planet *planet,unsigned int id );
    virtual ~Tyrannosaurus();
    virtual void UpdatePosition(float dt,std::list<Dinosaur*> listDinos); 
  
}; 