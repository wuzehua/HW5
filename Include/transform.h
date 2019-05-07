//
// Created by RainZhong on 2019/2/5.
//

#ifndef HW2_TRANSFORM_H
#define HW2_TRANSFORM_H


#include <iostream>
#include "object3d.h"
#include "matrix.h"



class Transform:public Object3D
{
public:
    Transform(Matrix matrix,Object3D* object);

    ~Transform()
    {
        if(object != nullptr)
            delete object;
        if(boundingBox != nullptr)
            delete boundingBox;
    }


    bool intersect(const Ray &r, Hit &h, float tmin);
    bool intersectShadow(const Ray &r, Hit &h, float tmin);

    void insertIntoGrid(Grid* g,Matrix* m);

    void paint();

    void addToVector(vector<Object3D *> &v, Matrix *m);


private:
    Matrix matrix;
    Matrix invMatrix;
    Matrix invTMatrix;
    Object3D* object;
};

#endif //HW2_TRANSFORM_H
