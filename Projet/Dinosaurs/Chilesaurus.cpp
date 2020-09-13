
#include "scenes/3D_graphics/Projet/Dinosaurs/Chilesaurus.hpp"
#include "math.h"
Chilesaurus::Chilesaurus(vcl::vec2 startingPositionFace, int terrainFaceInx, Planet *planet, unsigned int id ): Dinosaur(startingPositionFace, terrainFaceInx, planet, id){
    typeDinosaur = 1;
    MaxSpeed = 1.3*MaxSpeed;
}

Chilesaurus::~Chilesaurus(){
}

void Chilesaurus::UpdatePosition(float dt,  std::list<Dinosaur*> listDinos){
    if(m_special){ // if special behavior (FLEE here, attack for trex - then we want to keep it a little bit longer)
        m_deltaTimeSinceSpecial += dt;
        if(m_deltaTimeSinceSpecial > specialDelayTime){
            m_special = false;
            m_deltaTimeSinceSpecial = 0.0f;
        }
    }
    m_herdNeighbors = 0;
    m_herdCentre = {0,0,0};
    m_antiCollisionDirection ={0,0,0};
    m_herdDirection = {0,0,0};

    // TODO : paralelize the update on neigbors
    for (std::list<Dinosaur*>::iterator it=listDinos.begin(); it != listDinos.end(); ++it){
        if((*it) != this){
            Dinosaur* dino = (*it);
            
            vcl::vec3 pos = dino -> getPositionOnPlanet();
            vcl::vec3 dP = m_positionOnPlanet - pos;
        
           // float cTheta = dot(m_orientationOnPlanet,normalize(dP)); // toi check with the view angle if we want to (deleted for now - check Dinosaur)
            float sqrDist = dP.x*dP.x + dP.y*dP.y + dP.z*dP.z;

            if(dino->getDinoType() ==1  && ! m_special){ // group bahavior
                if(sqrDist < viewRadius){
                    vcl::vec3 orientation = dino->getOrientation();
                    
                    m_herdNeighbors+=1;
                    m_herdCentre+= pos; // weird.
                    m_herdDirection+= orientation; // always a norm

                    // in case we are too close, we avoid each other
                    if(sqrDist < avoidRadius2){
                        m_antiCollisionDirection += dP/sqrDist;
                    }
                }
            }
            else if(dino->getDinoType()==2){ // T-REX !!
                if(sqrDist < viewRadius){
                    m_special = true; // FLEEING
                    m_herdNeighbors=1;
                    m_herdCentre={0,0,0};
                    m_herdDirection = {0,0,0};
                    m_antiCollisionDirection = 5*dP/sqrDist;
                }   
            }
        }
    }

    Dinosaur::UpdatePosition(dt); 
}



