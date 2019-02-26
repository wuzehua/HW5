//
// Created by RainZhong on 2019/2/4.
//

#include "../Include/orthographiccamera.h"

OrthographicCamera::OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size)
{
    this->center = center;
    this->direction = direction;
    this->direction.Normalize();
    Vec3f::Cross3(horizon,direction,up);
    horizon.Normalize();
    Vec3f::Cross3(this->up,horizon,direction); //将up与horizon及direction构成的平面正交化
    this->up.Normalize();
    this->size = size;
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
    Vec3f horizonMap;
    Vec3f upMap;
    Vec3f origin;
    horizonMap = (point.x() - 0.5) * size * horizon;
    upMap = (point.y() - 0.5) * size * up;
    origin = center + horizonMap + upMap;

    Ray ray(origin,direction);

    return ray;
}

float OrthographicCamera::getTMin() const
{
    return -FLT_MAX; //射线的起点位无穷远
}


void OrthographicCamera::glInit(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-size/2.0, size/2.0, -size*(float)h/(float)w/2.0, size*(float)h/(float)w/2.0, 0.5, 40.0);
    else
        glOrtho(-size*(float)w/(float)h/2.0, size*(float)w/(float)h/2.0, -size/2.0, size/2.0, 0.5, 40.0);
}


void OrthographicCamera::glPlaceCamera(void)
{
    gluLookAt(center.x(), center.y(), center.z(),
            center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
            up.x(), up.y(), up.z());
}

void OrthographicCamera::dollyCamera(float dist)
{
    center += dist * direction;
}


void OrthographicCamera::truckCamera(float dx, float dy)
{

    //Vec3f horizontal;
    //Vec3f::Cross3(horizontal, direction, up);
    //horizontal.Normalize();
    //Vec3f screenUp;
    //Vec3f::Cross3(screenUp, horizontal, direction);
    //center += horizontal*dx + screenUp*dy;


    center += horizon * dx + up * dy;
}


void OrthographicCamera::rotateCamera(float rx, float ry)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize();
    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)

    float tiltAngle = acos(up.Dot3(direction));
    if (tiltAngle-ry > 3.13) ry = tiltAngle - 3.13;
    else if (tiltAngle-ry < 0.01) ry = tiltAngle - 0.01;
    Matrix rotMat = Matrix::MakeAxisRotation(up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);
    rotMat.Transform(center);
    rotMat.TransformDirection(direction);

    Vec3f::Cross3(horizon,direction,up);
    Vec3f::Cross3(up,horizon,direction);
    horizon.Normalize();
    up.Normalize();
}