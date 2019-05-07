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

    boundingBox = object->getTransformBoundingBox(&matrix);
    type = TransformType;

}


void Transform::addToVector(vector<Object3D *> &v, Matrix *m)
{
    if(object->getType() == GroupType || object->getType() == TransformType)
    {
        if(m == nullptr)
        {
            object->addToVector(v,&matrix);
        } else
        {
            auto mat = *m * matrix;
            object->addToVector(v,&mat);
        }
    } else {
        if (m == nullptr) {
            auto temp = (Object3D *) new Transform(matrix, object);
            v.push_back(temp);
        } else {
            auto mat = *m * matrix;
            auto temp = (Object3D *) new Transform(mat, object);
            v.push_back(temp);
        }
        object = nullptr;
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

void Transform::insertIntoGrid(Grid *g, Matrix *m)
{
    Matrix temp;
    if(m != nullptr)
    {
        temp = *m * matrix;
    }
    else
    {
        temp = matrix;
    }
    object->insertIntoGrid(g,&temp);
}
