#include "scenes/3D_graphics/Projet/Utilities/Utilities.hpp"
#include "math.h"
#include <assert.h>
#include <string> 
vcl::mat3 computeMat2(vcl::vec3 a){

    vcl::mat3 M(a.x*a.x,a.x*a.y,a.x*a.z,
             a.y*a.x,a.y*a.y,a.y*a.z,
             a.z*a.x,a.z*a.y,a.z*a.z);
    return M;
}

vcl::mat3 RotationOnPlanet(vcl::vec3 U, vcl::vec3 pos){ 
    // Rotation to align U on pos 
    // based on Rodrigues formula : https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula
    vcl::vec3 V = normalize(pos);
    vcl::mat3 R = vcl::mat3::identity();
    float cTheta = dot(V,U);

    // U is already normalize we say
    if(cTheta != 1 and cTheta != -1  ){ 
        vcl::vec3 N = cross(U,V);
        // in this case W = V
        float sTheta = dot(V, cross(N,U));
        vcl::mat3 M(0,-N.z,N.y,N.z,0, -N.x,-N.y,N.x,0);
        R = cTheta *vcl::mat3::identity() + (1-cTheta)*computeMat2(N) + sTheta * M;
    }
    else {
        if(cTheta == 1){
            // nothing to do
        }
        else {
            // we have to rotate the dinosaur along the z axis 
            R = vcl::rotation_from_axis_angle_mat3({0,0,1}, M_PI);
        }
    }
    return R;
    
}


// interpolate float
float interpolate(float initialOrientation, float finalOrientation, float dt, float T){
    float currentOrientation = ((T-dt) * initialOrientation  + dt * finalOrientation)/T;
    return currentOrientation;
}
// interpolated vectors and return a normalize vector
vcl::vec3 interpolate(vcl::vec3 initialPosition, vcl::vec3 finalPosition, float dt, float T){
    vcl::vec3 currentPosition = ((T-dt) * initialPosition  + dt * finalPosition)/T;
    return normalize(currentPosition);
}

void __M_Assert(const char* expr_str, bool expr, std::string msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n";
        abort();
    }
}
