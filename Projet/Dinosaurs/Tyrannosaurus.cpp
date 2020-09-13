
#include "Tyrannosaurus.hpp"
#include "math.h"

Tyrannosaurus::Tyrannosaurus(vcl::vec2 startingPositionFace, int terrainFaceInx, Planet *planet, unsigned int id ): Dinosaur(startingPositionFace, terrainFaceInx, planet, id){
    typeDinosaur = 2;
    MinSpeed = 1.2*MinSpeed;
    factorOnSurface = 2* factorOnSurface; // twice as big
}

Tyrannosaurus::~Tyrannosaurus(){

}


void Tyrannosaurus::UpdatePosition(float dt,  std::list<Dinosaur*> listDinos){
    if(m_special){
        m_deltaTimeSinceSpecial += dt;
        if(m_deltaTimeSinceSpecial > specialDelayTime){
            m_special = false;
            m_deltaTimeSinceSpecial = 0.0f;
        }
    }
    // TODO : paralelize it
    m_herdNeighbors = 0;
    m_herdCentre = {0,0,0};
    m_antiCollisionDirection ={0,0,0};
    m_herdDirection = {0,0,0};
    for (std::list<Dinosaur*>::iterator it=listDinos.begin(); it != listDinos.end(); ++it){
        if((*it) != this){
            Dinosaur* dino = (*it);

            vcl::vec3 pos = dino -> getPositionOnPlanet();
            vcl::vec3 dP = m_positionOnPlanet - pos;

           // float cTheta = dot(m_orientationOnPlanet,normalize(dP)); // toi chyeck with the view angle if we want to
            float sqrDist = dP.x*dP.x + dP.y*dP.y + dP.z*dP.z;
            if(dino->getDinoType() == 2  && ! m_special){
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
            else { // FOOD
                if(sqrDist < viewRadius){
                    m_special = true; // ATTACKING
                    m_herdNeighbors=1;
                    m_herdCentre={0,0,0};
                    m_herdDirection = {0,0,0};
                    m_antiCollisionDirection = -5*dP/sqrDist; // going straight for food
                    if(sqrDist < 3*avoidRadius2){
                        m_special = false;
                        dino->setIsDead(); // setting the new dino dead
                    }
                }   
            }
        }
    }

    Dinosaur::UpdatePosition(dt); 

}





