#pragma once 
#include "main/scene_base/base.hpp"

struct GridPosition{
    int faceIdx;
    int n_1;
    int n_2;

    bool equal(GridPosition other){
        if(faceIdx == other.faceIdx && n_1 == other.n_1 && n_2 == other.n_2){
            return true;
        }
        else{
            return false;
        }
    }
};


float interpolate(float initialOrientation, float finalOrientation, float dt, float T);
vcl::vec3 interpolate(vcl::vec3 initialPosition, vcl::vec3 finalPosition, float dt, float T);
vcl::mat3 computeMat2(vcl::vec3 a);
vcl::mat3 RotationOnPlanet(vcl::vec3 U, vcl::vec3 pos); // based on Rodrigues formula : https://en.wikipedia.org/wiki/Rodrigues%27_rotation_formula

void __M_Assert(const char* expr_str, bool expr, std::string msg);