//
// Created by RainZhong on 2019/2/5.
//

#include "../Include/plane.h"

Plane::Plane(Vec3f &normal, float offset, Material *material)
{
    this->normal = normal;
    this->normal.Normalize();
    this->offset = offset;
    this->material = material;
    boundingBox = NULL;
}


Plane::Plane(Vec3f &a, Vec3f &b, Vec3f &c, Material *material)
{
    Vec3f ab = b - a;
    Vec3f ac = c - a;
    Vec3f::Cross3(normal,ab,ac);
    normal.Normalize();
    offset = a.Dot3(normal);
    if(offset < 0)
    {
        normal.Negate();
        offset = -offset;//规定由原点指向并垂直于平面的方向为法向量的正方向
    }

    this->material = material;
}


//向量求射线与平面的交
bool Plane::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f direction = r.getDirection();
    Vec3f origin = r.getOrigin();
    float t,dot;


    dot = direction.Dot3(normal);


    if(dot == 0)
    {
        return false;
    }


    t = (offset - origin.Dot3(normal)) / dot;

    if(t > tmin && t < h.getT())
    {
        Ray ray(r.getOrigin(),direction);
        h.set(t,material,normal,ray);
        return true;
    }

    return false;
}


bool Plane::intersectShadow(const Ray &r, Hit &h, float tmin)
{
    return intersect(r,h,tmin);
}



void Plane::paint()
{
    Vec3f center;
    Vec3f v(1,0,0);
    Vec3f x,y;
    float big = 9e5;
    center = Vec3f(0,0,0) + offset * normal;


    if(fabsf(v.Dot3(normal)) == 1)
    {
        v.Set(0,1,0);
    }

    Vec3f::Cross3(x,v,normal);
    x.Normalize();

    Vec3f::Cross3(y,normal,x);
    y.Normalize();

    Vec3f v1 = big * x + big * y + center;
    Vec3f v2 = (-big) * x + big * y + center;
    Vec3f v3 = (-big) * x - big * y + center;
    Vec3f v4 = big * x - big * y + center;

    material->glSetMaterial();
    glBegin(GL_QUADS);
    glNormal3f(normal.x(),normal.y(),normal.z());
    glVertex3f(v1.x(),v1.y(),v1.z());
    glVertex3f(v2.x(),v2.y(),v2.z());
    glVertex3f(v3.x(),v3.y(),v3.z());
    glVertex3f(v4.x(),v4.y(),v4.z());
    glEnd();


}