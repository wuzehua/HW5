//
// Created by RainZhong on 2019/2/5.
//

#include "../Include/triangle.h"

float min3(float x1,float x2, float x3)
{
    float result = x1 < x2 ? x1 : x2;
    result = result < x3 ? result : x3;
    return result;
}

float max3(float x1,float x2,float x3)
{
    float result = x1 > x2 ? x1 : x2;
    result = result > x3 ? result : x3;
    return result;
}

Triangle::Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m)
{
    Vec3f ab = b - a;
    Vec3f bc = c - b;
    Vec3f::Cross3(normal,ab,bc);
    normal.Normalize();
    offset = a.Dot3(normal);

    if(offset < 0)
    {
        normal.Negate();
    }

    this->a = a;
    this->b = b;
    this->c = c;
    material = m;

    float xmin = min3(a.x(),b.x(),c.x());
    float ymin = min3(a.y(),b.y(),c.y());
    float zmin = min3(a.z(),b.z(),c.z());

    float xmax = max3(a.x(),b.x(),c.x());
    float ymax = max3(a.y(),b.y(),c.y());
    float zmax = max3(a.z(),b.z(),c.z());

    Vec3f min(xmin,ymin,zmin);
    Vec3f max(xmax,ymax,zmax);

    boundingBox = new BoundingBox(min,max);
}




bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f direction = r.getDirection();
    Vec3f origin = r.getOrigin();
    Vec3f hit;
    float t,Adet;
    float beta,gama;




    Vec3f amb = a - b;
    Vec3f amc = a - c;
    Vec3f amo = a - origin;
    Matrix A;

    A.Set3x3Col(0,amb);
    A.Set3x3Col(1,amc);
    A.Set3x3Col(2,direction);

    Adet = A.Get3x3Det();

    if(Adet == 0)
    {
        return false;
    }


    A.Set3x3Col(0,amo);
    beta = A.Get3x3Det() / Adet;

    A.Set3x3Col(0,amb);
    A.Set3x3Col(1,amo);
    gama = A.Get3x3Det() / Adet;

    A.Set3x3Col(1,amc);
    A.Set3x3Col(2,amo);
    t = A.Get3x3Det() / Adet;

    //交点在三角形内部的条件
    if(beta >= 0 && gama >= 0 && beta + gama <= 1 && t > tmin && t < h.getT())
    {
        Ray ray(origin,direction);
        h.set(t,material,normal,ray);
        return true;
    }

    return false;
}


bool Triangle::intersectShadow(const Ray &r, Hit &h, float tmin)
{
    return intersect(r,h,tmin);
}


void Triangle::paint()
{
    material->glSetMaterial();
    glBegin(GL_TRIANGLES);
    glNormal3f(normal.x(),normal.y(),normal.z());
    glVertex3f(a.x(),a.y(),a.z());
    glVertex3f(b.x(),b.y(),b.z());
    glVertex3f(c.x(),c.y(),c.z());
    glEnd();
}