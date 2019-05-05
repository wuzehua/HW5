//
// Created by RainZhong on 2019/2/8.
//

#ifndef HW2_PERSPECTIVECAMERA_H
#define HW2_PERSPECTIVECAMERA_H

#include "camera.h"

class PerspectiveCamera:public Camera
{
public:
    PerspectiveCamera(Vec3f& center, Vec3f &direction, Vec3f &up, float angle_radius);
    Ray generateRay(Vec2f point);
    float getTMin() const;

    void glInit(int w,int h);
    void glPlaceCamera(void);
    void dollyCamera(float dist);
    void truckCamera(float dx, float dy);
    void rotateCamera(float rx, float ry);

private:
    Vec3f center;
    Vec3f direction;
    Vec3f up;
    Vec3f horizion;
    Vec3f rawUp;
    float size;
    float angle_radius;
};


#endif //HW2_PERSPECTIVECAMERA_H
