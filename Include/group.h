//
// Created by RainZhong on 2019/2/1.
//

#ifndef HW1_GROUP_H
#define HW1_GROUP_H

#include "object3d.h"

typedef Object3D* Object3DPtr;
typedef Object3DPtr* Object3DPtrArray;

class Group:public Object3D
{
public:

    Group(int numOfObjects);

    ~Group();

    void addObject(int index,Object3D* obj);


    bool intersect(const Ray &r, Hit &h, float tmin);
    bool intersectShadow(const Ray& r,Hit& h,float tmin);

    void paint();



private:
    int numOfObjects;
    Object3DPtrArray objects;
};

#endif //HW1_GROUP_H
