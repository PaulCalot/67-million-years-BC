#include "Meteorite.hpp"

Meteorite::Meteorite(){
}

Meteorite::Meteorite(Planet *planet, float scaleFactor) { 
    g_planet = planet -> getG();
    scalePlanet = planet->getRadius();
    m_distancePlanetCenter = 6*scalePlanet;
    m_speedNorm = 3.5*scalePlanet; // 
    m_scale = scaleFactor*scalePlanet; 
    vcl::vec3 speedVector = {0,-1,0};
    m_speed =  m_speedNorm * speedVector;
    vcl::vec3 positionVector = {0,0,1};
    m_position = m_distancePlanetCenter * positionVector;
    makeMesh(true);
}

Meteorite::Meteorite(float initialDistance, float initialSpeed, Planet *planet, float scaleFactor): m_speedNorm(initialSpeed), m_distancePlanetCenter(initialDistance) {
    g_planet = planet -> getG();
    scalePlanet = planet->getRadius();
    m_scale = scaleFactor*scalePlanet;
    float x = 1.0-2*((float) rand() / (RAND_MAX));
    float y = 1.0-2*((float) rand() / (RAND_MAX));
    vcl::vec3 speedVector = {-y,x,0};
    m_speed = m_speedNorm * normalize(speedVector);
    vcl::vec3 positionVector = {x,y,0};
    m_position = m_distancePlanetCenter * normalize(positionVector);
    makeMesh(false); // also initializes the position
}

void Meteorite::UpdatePosition(float dt, Planet *planet){
    m_dt += dt; // time for physics update
    m_dt_2 += dt; // time for bounces update (to handle the moment the meteorite collides with the planet)

    if(!m_isDead && m_dt > updateTimePeriod){
        m_dt = 0;
        vcl::vec3 oldPosition = m_position;
        
        m_position = m_position * (1 - 0.5f * g_planet * updateTimePeriod * updateTimePeriod/m_distancePlanetCenter) + m_speed * updateTimePeriod;

        vcl::vec3 dP = m_position - oldPosition;
        m_distancePlanetCenter = norm(m_position);
        m_speed = m_speedNorm/norm(dP) * dP;

        vcl::vec3 positionOnUnitSphere = m_position/m_distancePlanetCenter;
        int faceIdx = planet->UpdatePositionTerrainFaceIdx(positionOnUnitSphere);

                // checking if we are not in the planet yet and that we have not collided with the planet already (in the previous updateTimePeriod_2 seconds)
        if(m_dt_2 > updateTimePeriod_2 && m_distancePlanetCenter < norm((*planet->getTerrainFaces())[faceIdx].evaluate_PointOnPlanet(m_position/m_distancePlanetCenter)) ){
            // we have entered the planet
            if(m_speedNorm < MinSpeedNorm){
                m_isDead = true;
            }
            
            vcl::vec3 proj_Speed = dot(-m_speed,positionOnUnitSphere)*positionOnUnitSphere; // projection on the normal to the planet
            m_speedNorm = 0.3f*m_speedNorm; // we change the speed norm
            m_speed = 0.3*(2*proj_Speed + m_speed);  // we bounce 0.3 for the loss in energy
            m_dt_2 == 0;
        }
        m_mesh.uniform.transform.translation = m_position;
        
    }
}

void Meteorite::makeMesh(bool noise=true){ // only the big one has noise - the smaller one don't 
    int Nu(10); // the resolution
    int Nv(10);

    // rought noise
    float scaling = 1;
    int octave = 1;
    double persistency = 0.2; 
    double M(2*m_scale); // multiplying by m_scale is not enough - should we squarred ?

    // for details
    float scaling_= 1; 
    int octave_ = 20;
    double persistency_ = 1; 
    double M_(30*m_scale); // multiplying by m_scale is not enough - should we squarred ?

    mesh shape;
    for( size_t ku=0; ku<Nu; ++ku ) {
        for( size_t kv=0; kv<Nv; ++kv ) {

            // Parametric coordinates
            const float u = static_cast<float>(ku)/static_cast<float>(Nu-1);
            const float v = static_cast<float>(kv)/static_cast<float>(Nv-1);

            // Angles
            const float theta = static_cast<float>( 3.14159f*v );
            const float phi   = static_cast<float>( 2*3.14159f*u );
            
           
            // Spherical coordinates
            const float x = m_scale * std::sin(theta) * std::cos(phi);
            const float y = m_scale * std::sin(theta) * std::sin(phi);
            const float z = m_scale * std::cos(theta);
            vcl::vec3 p;
             // noise
            if(noise){
                const float noise3D = 1+(perlin(scaling*x, scaling*y, scaling*z, octave, persistency)-1)/M;
                const float noise3D_ = 1+(perlin(scaling*x, scaling*y, scaling*z, octave_, persistency_)-1)/M_;
                p  = {x,y,noise3D*noise3D_*z};      // Position (centered)
            }
            else{
                p  = {x,y,z};      // Position (centered)
                
            }
           
            const vec2 uv = {v,u};        // Texture-coordinates

            shape.position.push_back(p); // Add new position (with translation of the center)
            shape.texture_uv.push_back(uv);
        }
    }
    shape.connectivity = connectivity_grid(Nu, Nv, false, false);
    m_mesh = shape;
    m_mesh.uniform.transform.translation += m_position;
    if(noise){
        m_mesh.texture_id = create_texture_gpu( image_load_png("scenes/3D_graphics/Projet/textures/meteorite.png"), GL_REPEAT, GL_REPEAT);
    }
    else{
        m_mesh.uniform.color = {0.9,0.1,0.1};
    }
}


vcl::mesh_drawable Meteorite::getMesh(){
    return m_mesh;
}