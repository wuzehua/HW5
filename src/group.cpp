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


void Group::insertIntoGrid(Grid *g, Matrix *m)
{
    if(boundingBox == NULL)
    {
        if(numOfObjects == 0)
        {
            boundingBox = new BoundingBox(Vec3f(),Vec3f());
        } else
        {
            BoundingBox* temp;
            for(int i = 0;i < numOfObjects;i++)
            {
                temp = objects[i]->getBoundingBox();
                if(i == 0)
                {
                    boundingBox = new BoundingBox(temp->getMin(),temp->getMax());
                } else
                {
                    boundingBox->Extend(temp);
                }
            }
        }

    }
    for(int i = 0;i < numOfObjects;i++)
    {
        objects[i]->insertIntoGrid(g,m);
    }
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

/*
BoundingBox* Group::getTransformBoundingBox(Matrix *m)
{
    BoundingBox* b = NULL;
    BoundingBox* temp;
    for(int i = 0;i < numOfObjects;i++)
    {
        temp = objects[i]->getTransformBoundingBox(m);
        if(b == NULL)
        {
            b = temp;
        } else
        {
            b->Extend(temp);
            delete temp;
        }
    }

    return b;
}
 */