//
// Created by RainZhong on 2019/2/1.
//

#ifndef HW1_CAMERA_H
#define HW1_CAMERA_H

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


#include "ray.h"
#include "matrix.h"

class Camera
{
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;


    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;


protected:
    Vec3f center;
};

#endif //HW1_CAMERA_H
