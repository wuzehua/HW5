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
        if(object != NULL)
            delete object;
        if(boundingBox != NULL)
            delete boundingBox;
    }


    bool intersect(const Ray &r, Hit &h, float tmin);
    bool intersectShadow(const Ray &r, Hit &h, float tmin);

    void insertIntoGrid(Grid* g,Matrix* m);

    void paint();


private:
    Matrix matrix;
    Matrix invMatrix;
    Matrix invTMatrix;
    Object3D* object;
};

#endif //HW2_TRANSFORM_H
