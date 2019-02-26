//
// Created by RainZhong on 20.02.2019.
//

#ifndef HW4_RAYTRACER_H
#define HW4_RAYTRACER_H


#include <cfloat>
#include "scene_parser.h"
#include "ray.h"
#include "hit.h"
#include "material.h"
#include "group.h"
#include "light.h"
#include "rayTree.h"



class RayTracer
{
public:
    RayTracer(SceneParser* s,int max_bounce, float cutoff_weight, bool shadow, bool shadeBack = false);
    Vec3f traceRay(Ray& ray, float tmin,int bounces, float weight, float indexOfRefraction,Hit& hit) const;
    Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& income) const;
    bool transmittedDirection(const Vec3f& normal, const Vec3f& incoming, float index_i, float index_t,Vec3f& transmitted) const;

private:
    SceneParser* parser;
    int maxBounce;
    bool shadow;
    bool shadeBack;
    float cutOffWeight;
    Group* group;
    Light** lights;
    int numOfLights;
    Vec3f ambient;
};


#endif //HW4_RAYTRACER_H
