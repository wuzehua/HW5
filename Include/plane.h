//
// Created by RainZhong on 2019/2/5.
//

#ifndef HW1_PLANE_H
#define HW1_PLANE_H

#include "object3d.h"

class Plane:public Object3D
{
public:
    Plane(Vec3f& normal, float offset,Material* material);
    Plane(Vec3f& a,Vec3f& b,Vec3f& c,Material* material);

    bool intersect(const Ray &r, Hit &h, float tmin);
    bool intersectShadow(const Ray &r, Hit &h, float tmin);


    void paint();

private:
    Material* material;
    Vec3f normal;
    float offset;
};

#endif //HW1_PLANE_H
