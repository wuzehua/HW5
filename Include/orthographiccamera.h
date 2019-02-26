//
// Created by RainZhong on 2019/2/4.
//

#ifndef HW1_ORTHOGRAPHICCAMERA_H
#define HW1_ORTHOGRAPHICCAMERA_H

#include <cfloat>
#include "camera.h"


class OrthographicCamera:public Camera
{
public:
    OrthographicCamera(Vec3f center,Vec3f direction,Vec3f up,float size);
    //生成射线
    Ray generateRay(Vec2f point);
    float getTMin() const;

    void glInit(int w,int h);
    void glPlaceCamera(void);
    void dollyCamera(float dist);
    void truckCamera(float dx, float dy);
    void rotateCamera(float rx, float ry);



private:
    float size;
    Vec3f direction;
    Vec3f up;
    Vec3f horizon;
};

#endif //HW1_ORTHOGRAPHICCAMERA_H
