//
// Created by RainZhong on 13.02.2019.
//


#include "../Include/transform.h"

Transform::Transform(Matrix matrix, Object3D *object)
{
    this->matrix = matrix;
    invMatrix = matrix;
    invMatrix.Inverse();
    invTMatrix = invMatrix;
    invTMatrix.Transpose();
    this->object = object;
    boundingBox = NULL;

    BoundingBox* temp = object->getBoundingBox();

    if(temp != NULL)
    {
        Vec3f min = temp->getMin();
        Vec3f max = temp->getMax();
        matrix.Transform(min);
    }

}

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f direction  = r.getDirection();
    Vec3f origin = r.getOrigin();
    bool hit;
    direction.Normalize();
    invMatrix.TransformDirection(direction);//先将origin与direction由ws转换到os
    invMatrix.Transform(origin);
    Ray ray(origin,direction);
    hit = object->intersect(ray,h,tmin);
    if(hit)
    {
        float t = h.getT();
        Vec3f normal = h.getNormal();
        Material* material = h.getMaterial();
        direction = r.getDirection();
        direction.Normalize();
        Ray newRay(r.getOrigin(),direction);
        invTMatrix.TransformDirection(normal);//将在os中的法向量转换为ws中的
        normal.Normalize();
        h.set(t,material,normal,newRay);
    }

    return hit;
}


bool Transform::intersectShadow(const Ray &r, Hit &h, float tmin)
{
    return intersect(r,h,tmin);
}


void Transform::paint()
{
    glPushMatrix();
    GLfloat* glMatrix = matrix.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    object->paint();
    glPopMatrix();

}