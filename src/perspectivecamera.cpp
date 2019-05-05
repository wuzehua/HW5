//
// Created by RainZhong on 2019/2/8.
//

#include "../Include/perspectivecamera.h"

PerspectiveCamera::PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle_radius)
{
    this->center = center;
    this->direction = direction;
    this->rawUp = up;
    this->rawUp.Normalize();
    Vec3f::Cross3(horizion,direction,this->rawUp);
    Vec3f::Cross3(this->up,horizion,direction);
    this->up.Normalize();
    horizion.Normalize();
    this->direction.Normalize();
    this->angle_radius = angle_radius;
    size = 2 * tanf(angle_radius / 2); //在距离center为1个单位长度的位置虚拟屏幕的size

}

float PerspectiveCamera::getTMin() const
{
    return 0;
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    Vec3f dir;
    dir = direction + (point.x() - 0.5) * size * horizion + (point.y() - 0.5) * size * up;
    dir.Normalize();
    Ray ray(center,dir);
    return ray;

}


void PerspectiveCamera::glInit(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle_radius * 180.0/3.14159, (float)w/float(h), 0.5, 40.0);
}


void PerspectiveCamera::glPlaceCamera(void)
{
    gluLookAt(center.x(), center.y(), center.z(),
            center.x()+direction.x(), center.y()+direction.y(), center.z()+direction.z(),
            up.x(), up.y(), up.z());
}


void PerspectiveCamera::dollyCamera(float dist)
{
    center += direction * dist;
}

void PerspectiveCamera::truckCamera(float dx, float dy)
{
    /*
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, direction, up);
    horizontal.Normalize(); Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, direction);
    center += horizontal*dx + screenUp*dy;
     */

    center += horizion * dx + up * dy;
}


void PerspectiveCamera::rotateCamera(float rx, float ry)
{

    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)

    float tiltAngle = acos(rawUp.Dot3(direction));
    if (tiltAngle - ry > 3.13) ry = tiltAngle - 3.13;
    else if (tiltAngle - ry < 0.01) ry = tiltAngle - 0.01;
    Matrix rotMat = Matrix::MakeAxisRotation(rawUp, rx);
    rotMat *= Matrix::MakeAxisRotation(horizion, ry);
    rotMat.Transform(center);
    rotMat.TransformDirection(direction);
    direction.Normalize();

    Vec3f::Cross3(horizion,direction,rawUp);
    Vec3f::Cross3(up,horizion,direction);
    horizion.Normalize();
    up.Normalize();
}