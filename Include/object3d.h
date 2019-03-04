//
// Created by RainZhong on 2019/2/1.
//

#ifndef UNTITLED_OBJECT3D_H
#define UNTITLED_OBJECT3D_H


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "ray.h"
#include "hit.h"
#include "boundingbox.h"
#include "material.h"

class Grid;

class Object3D
{
public:
    Object3D(){}
    virtual ~Object3D(){}
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual bool intersectShadow(const Ray& r,Hit& h,float tmin){};
    virtual void paint(void) = 0;
    virtual void insertIntoGrid(Grid *g, Matrix *m){}
    BoundingBox* getBoundingBox() { return boundingBox;}

protected:
    BoundingBox* boundingBox;

};


#endif //UNTITLED_OBJECT3D_H
