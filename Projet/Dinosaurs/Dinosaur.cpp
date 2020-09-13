#include "scenes/3D_graphics/Projet/Dinosaurs/Dinosaur.hpp"
#include "math.h"
#include <assert.h>
#include <string> 

Dinosaur::Dinosaur(){
}

Dinosaur::~Dinosaur(){
}
Dinosaur::Dinosaur(vcl::vec2 startingPositionFace, unsigned int terrainFaceIdx, Planet *planet, unsigned int id ) {
    m_terrainFaceIdx=terrainFaceIdx;
    m_planet=planet;
    m_id=id; 
    ScalePlanet = planet -> getRadius();
    m_scale = ScalePlanet * RatioDinoPlanet;   
    m_herdNeighbors = 0; // neightbors in the herd
    m_herdDirection = {0,0,0};
    m_herdCentre  = {0,0,0} ;
    m_antiCollisionDirection = {0,0,0};
    m_velocity = {0,0,0};

    // position in the grid
    GridPosition m_positionInGrid;
    m_positionInGrid.faceIdx = m_terrainFaceIdx;
    m_positionInGrid.n_1 = 0;
    m_positionInGrid.n_2 = 0;

    //
    MaxSpeed = 2* m_scale * mult_Speed;
    MinSpeed = m_scale * mult_Speed;
    m_speedNorm = MinSpeed;

    // 
    avoidRadius = m_scale*mult_avoidRadius;
    avoidRadius2 = avoidRadius*avoidRadius;
    viewRadius = m_scale *mult_viewRadius;
    // behavior
    m_isDead = false;
    m_special = false;
    //
    Dinosaur::initPosition(startingPositionFace);

    factorOnSurface = 0.8 * m_scale * sin(M_PI/6); // based on the hierarchy
    m_deltaTimeSinceSpecial = 0.0f;

}

// ---------------- additional function ---------------- //




void Dinosaur::initPosition( vcl::vec2 startingPositionFace){

    TerrainFace &terrainFace =(*m_planet -> getTerrainFaces())[m_terrainFaceIdx];
    
    // init position on unit sphere
    m_positionOnUnitSphere =  terrainFace.evaluate_PointOnUnitSphere(startingPositionFace); // point on Sphere

    // Init position on planet
    m_positionOnPlanet = terrainFace.evaluate_PointOnPlanet(m_positionOnUnitSphere);

    // Init orientation on unit sphere
    float x =  ((double) rand() / (RAND_MAX)); // between 0 and 1
    float y =  ((double) rand() / (RAND_MAX)); 
    float z =  ((double) rand() / (RAND_MAX)); 
    vcl::vec3 firstOrientation = {x,y,z};

    // we make sure position and orientation are perpendicular
    m_orientationOnPlanet = normalize(firstOrientation-dot(m_positionOnPlanet,firstOrientation)*m_positionOnPlanet); 

    m_velocity = m_orientationOnPlanet * MinSpeed;
    // Compute the matrix to go from local coordinates to global ones
    vcl::vec3 X = normalize(cross(-m_positionOnUnitSphere, m_orientationOnPlanet));
    vcl::mat3 R(X, -m_positionOnUnitSphere, m_orientationOnPlanet);

    m_rotation = R;
    m_translation = m_positionOnPlanet * (1+2*RatioDinoPlanet); // the size of the dinosaure depends on m_scale and its legs
}


vcl::vec3 Dinosaur::thrustToward(vcl::vec3 vect){
    vcl::vec3 v = normalize(vect)*m_speedNorm - m_velocity;
    double norm_ = norm(v);
    //std::cout << norm_ << std::endl;
    if(norm_ > MaxForce){
        v *= norm_/MaxForce;
    }
    return v;
}

void Dinosaur::limitSpeed(){
    if (m_speedNorm>MaxSpeed){
        m_velocity = normalize(m_velocity) * MaxSpeed;
        m_speedNorm = MaxSpeed;
    }
    else if(m_speedNorm < MinSpeed){
        m_velocity = normalize(m_velocity) * MinSpeed;
        m_speedNorm = MinSpeed;
    }
}

void Dinosaur::UpdatePosition(float dt){

    if(m_herdNeighbors == 0){ // no neighbor -> random movement
        float variationAngle(0.0f);
        float r = ((double) rand() / (RAND_MAX)); // between 0 and 1
        if(r<0.7f){
            // we keep the same direction
            variationAngle = 0.0f;
        }
        else if(r<0.8f){
            variationAngle = - angleVariationDelta * dt;
        }
        else{
            variationAngle = angleVariationDelta * dt;
        }
        m_orientationOnPlanet = rotation_from_axis_angle_mat3(normalize(m_positionOnPlanet), variationAngle) * m_orientationOnPlanet; // in theory already normalized
        m_velocity +=m_orientationOnPlanet*m_speedNorm;
    }

    else {
        vcl::vec3 acceleration;
        if(m_special){
            m_speedNorm = MaxSpeed;
            acceleration = normalize(m_antiCollisionDirection*m_coefAntiCollisionDirection)*m_speedNorm;
            m_velocity = acceleration*dt;
        }
        else{
            
             m_herdCentre /= m_herdNeighbors;

            vcl::vec3 offsetToHerdCenter = m_herdCentre - m_positionOnPlanet;

            //  the forces to apply
            acceleration = thrustToward(offsetToHerdCenter)*m_coefHerdCentre + thrustToward(m_antiCollisionDirection)*m_coefAntiCollisionDirection +  thrustToward(m_herdDirection)*m_coefHerdDirection;
            //std::cout << "acceleration : " << acceleration << std::endl;
            m_velocity += acceleration * dt;
        }
        m_orientationOnPlanet = m_velocity/m_speedNorm; // normalized   

    }
    m_speedNorm = norm(m_velocity);
    limitSpeed();
    
    m_positionOnPlanet += m_velocity *dt;
    m_positionOnUnitSphere = normalize(m_positionOnPlanet);
    m_terrainFaceIdx =  m_planet -> UpdatePositionTerrainFaceIdx(m_positionOnUnitSphere);

    m_positionOnPlanet = (*m_planet -> getTerrainFaces())[m_terrainFaceIdx].evaluate_PointOnPlanet(m_positionOnUnitSphere);

    m_translation = m_positionOnPlanet*(1+factorOnSurface);

    m_orientationOnPlanet = normalize(m_orientationOnPlanet-dot(m_positionOnUnitSphere,m_orientationOnPlanet)*m_positionOnUnitSphere);   
    vcl::vec3 X = normalize(cross(-m_positionOnUnitSphere, m_orientationOnPlanet));
    vcl::mat3 R(X, -m_positionOnUnitSphere, m_orientationOnPlanet);

    m_rotation = R;
    }  


// this functions is useless in Dinosaur (but useful in Tyra / Chile classes)
void Dinosaur::UpdatePosition( float dt,std::list<Dinosaur*> listDinos){

}






// -------------- Getter and Setter ----------------- //

vcl::vec3 Dinosaur::getPositionOnSphereUnit(){
    return m_positionOnUnitSphere;
}

bool Dinosaur::getIsDead(){
    return m_isDead;
}

void Dinosaur::setIsDead(){
    //std::cout << "DEAD" << std::endl;
    m_translation /= (1+2*RatioDinoPlanet);
    m_isDead = true;
}

int Dinosaur::getDinoType(){
    return typeDinosaur;
}

unsigned int Dinosaur::getFaceIdx(){
    return m_terrainFaceIdx;
}

vcl::vec3 Dinosaur::getTranslation(){
    return m_translation;
}

vcl::mat3 Dinosaur::getRotation(){
    return m_rotation;
}

vcl::vec3 Dinosaur::getPositionOnPlanet(){
    return m_positionOnPlanet;
}

void Dinosaur::setPositionInGrid(GridPosition positionInGrid){
    m_positionInGrid = positionInGrid;
}
GridPosition Dinosaur::getPositionInGrid(){
    return m_positionInGrid;
}

unsigned int Dinosaur::getId(){
    return m_id;
}
vcl::vec3 Dinosaur::getOrientation(){
    return m_orientationOnPlanet;
}



/*

void Dinosaur::checkCoordinate(){
    
    int nextFace = m_terrainFaceInx;
    
    if(m_relativePosition.x < 0.0f){

        if(m_terrainFaceInx == 0){ 
            nextFace = 5;//-z
            float tmp = m_relativePosition.x;
            m_relativePosition.x = m_relativePosition.y;
            m_relativePosition.y = -tmp;
        }else if(m_terrainFaceInx == 1){
            nextFace = 4;//+z
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 1-m_relativePosition.y; // < 1
            m_relativePosition.y = 1-tmp;
        }else if(m_terrainFaceInx == 2){ // +y
            nextFace = 1;//-x
            float tmp = m_relativePosition.x;
            m_relativePosition.x = m_relativePosition.y; 
            m_relativePosition.y = -tmp; 
        }else if(m_terrainFaceInx == 3){ // -y
            nextFace = 0;//+x
            float tmp = m_relativePosition.x;
            m_relativePosition.x =1- m_relativePosition.y; 
            m_relativePosition.y = 1+tmp; 
        }else if(m_terrainFaceInx == 4){ // +z
            nextFace = 3;//-y
            float tmp = m_relativePosition.x;
            m_relativePosition.x = m_relativePosition.y; 
            m_relativePosition.y = -tmp; 
        }else { // -z
            nextFace = 2;//+y
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 1-m_relativePosition.y; 
            m_relativePosition.y = 1+tmp;// < 1
        }
    }
    else if(m_relativePosition.x > 1.0f){

        if(m_terrainFaceInx == 0){ 
            nextFace = 4;//+z
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 1-m_relativePosition.y;
            m_relativePosition.y = tmp-1; // >0
        }else if(m_terrainFaceInx == 1){ 
            nextFace = 5; // -z
            float tmp = m_relativePosition.x;
            m_relativePosition.x = m_relativePosition.y; // < 1
            m_relativePosition.y = tmp-1;
        }
        
        else if(m_terrainFaceInx == 2){ // +y
            nextFace = 0;//+x
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 1-m_relativePosition.y; // < 1
            m_relativePosition.y = tmp-1;
        }else if(m_terrainFaceInx == 3){ //-y
            nextFace = 1;//-x
            float tmp = m_relativePosition.x;
            m_relativePosition.x = m_relativePosition.y; // < 1
            m_relativePosition.y = 2-tmp;
        }else if(m_terrainFaceInx == 4){ //+z
            nextFace = 2;//+y
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 1-m_relativePosition.y; // < 1
            m_relativePosition.y = tmp-1;
        }else {  // -z
            nextFace = 3;//-y
            float tmp = m_relativePosition.x;
            m_relativePosition.x = m_relativePosition.y; // < 1
            m_relativePosition.y = 2-tmp;
        }
    }
    else if(m_relativePosition.y < 0.0f){
        if(m_terrainFaceInx == 0){
            nextFace = 2;//+y
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 1+m_relativePosition.y; // < 1
            m_relativePosition.y = 1-tmp;
        }else if(m_terrainFaceInx == 1){
            nextFace = 2;//+y
            float tmp = m_relativePosition.x;
            m_relativePosition.x = -m_relativePosition.y;
            m_relativePosition.y = tmp;
        }
        
        else if(m_terrainFaceInx == 2){ 
            nextFace = 4;//+z
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 1+m_relativePosition.y; // < 1
            m_relativePosition.y = 1-tmp;
        }else if(m_terrainFaceInx == 3){ //-y
            nextFace = 4;//+z
            float tmp = m_relativePosition.x;
            m_relativePosition.x = -m_relativePosition.y; // < 1
            m_relativePosition.y = tmp;
        }else if(m_terrainFaceInx == 4){ //+z
            nextFace = 0;//+x
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 1+m_relativePosition.y; // < 1
            m_relativePosition.y = 1-tmp;
        }else { // -z
            nextFace = 0;//+x
            float tmp = m_relativePosition.x;
            m_relativePosition.x = -m_relativePosition.y; // < 1
            m_relativePosition.y = tmp;
        }
    }
    else if(m_relativePosition.y > 1.0f){
        if(m_terrainFaceInx == 0){
            nextFace = 3;//-y
            float tmp = m_relativePosition.x;
            m_relativePosition.x = m_relativePosition.y-1; // < 1
            m_relativePosition.y = 1-tmp;
        }else if(m_terrainFaceInx == 1){ 
            nextFace = 3;//-y
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 2-m_relativePosition.y; 
            m_relativePosition.y = tmp;
        }else if(m_terrainFaceInx == 2){ 
            nextFace = 5;//+z
            float tmp = m_relativePosition.x;
            m_relativePosition.x = m_relativePosition.y-1; // < 1
            m_relativePosition.y = 1-tmp;
        }else if(m_terrainFaceInx == 3){ //-y
            nextFace = 5;//-z
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 2-m_relativePosition.y; // < 1
            m_relativePosition.y = tmp;
        }else if(m_terrainFaceInx == 4){ //+z
            nextFace = 1;//-x
            float tmp = m_relativePosition.x;
            m_relativePosition.x = m_relativePosition.y-1; // < 1
            m_relativePosition.y = 1-tmp;
        }else { 
            nextFace = 1;//-x
            float tmp = m_relativePosition.x;
            m_relativePosition.x = 2-m_relativePosition.y; // < 1
            m_relativePosition.y = tmp;
        }
    }
    std::string  msg = "Current " + to_string(m_terrainFaceInx) + " and new one : " + to_string(nextFace);
    __M_Assert("Test y>=0",m_relativePosition.y>=0,msg);
    __M_Assert("Test x>=0",m_relativePosition.x>=0,msg);
    __M_Assert("Test y<=1",m_relativePosition.y<=1,msg);
    __M_Assert("Test x<=1",m_relativePosition.x<=1,msg);
      
   m_terrainFaceInx = nextFace;
}
*/