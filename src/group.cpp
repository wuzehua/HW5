//
// Created by RainZhong on 2019/2/4.
//

#include "../Include/group.h"

Group::Group(int numOfObjects)
{
    this->numOfObjects = numOfObjects;
    objects = new Object3DPtr[numOfObjects];
    boundingBox = NULL;
}

Group::~Group()
{
    delete[] objects;
    if(boundingBox != NULL)
        delete boundingBox;
}


void Group::addObject(int index, Object3D *obj)
{
    objects[index] = obj;

    BoundingBox* temp;
    temp = obj->getBoundingBox();

    if(temp != NULL)
    {
        if(boundingBox == NULL)
        {
            boundingBox = new BoundingBox(temp->getMin(),temp->getMax());
        } else
        {
            boundingBox->Extend(temp);
        }
    }

}


bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool result = false;
    bool temp;
    //判断在group里面的object是否hit
    for(int i = 0;i < numOfObjects;i++)
    {
        temp = objects[i]->intersect(r,h,tmin);
        if(temp && !result)
        {
            result = temp;
        }
    }

    return result;
}


bool Group::intersectShadow(const Ray &r, Hit &h, float tmin)
{
    bool result = false;
    for(int i = 0;i < numOfObjects;i++)
    {
        result = objects[i]->intersectShadow(r,h,tmin);
        if(result)
            break;
    }

    return result;
}


void Group::paint()
{
    for(int i = 0;i < numOfObjects;i++)
    {
        objects[i]->paint();
    }
}