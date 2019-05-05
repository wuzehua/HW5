//
// Created by RainZhong on 2019/2/1.
//

#ifndef HW1_SPHERE_H
#define HW1_SPHERE_H


#include "object3d.h"
#include "grid.h"
#include "matrix.h"
#include <math.h>


class Sphere:public Object3D
{
public:
    Sphere(Vec3f center, float radius,Material* material):center(center),radius(radius)
    {
        this->material = material;
        Vec3f temp(radius,radius,radius);
        Vec3f min = center - temp;
        Vec3f max = center + temp;
        boundingBox = new BoundingBox(min,max);
        type = SphereType;
    }

    ~Sphere()
    {
        delete material;
        delete boundingBox;
    }

    //几何法求交，存在问题，当direction与scale在同一方向时会一直返回false
    bool intersectGeo(const Ray &r, Hit &h, float tmin);

    //代数法求交
    bool intersect(const Ray &r, Hit &h, float tmin);

    bool intersectShadow(const Ray& r,Hit& h, float tmin);

    void insertIntoGrid(Grid *g, Matrix *m);

    void paint();

    static void setStep(int theta,int phi);
    static void setGouraud(bool gouraud);

private:
    Vec3f center;
    float radius;
    Material* material;
    static int theta_Step;
    static int phi_Step;
    static bool gouraud;

};



#endif //HW1_SPHERE_H
