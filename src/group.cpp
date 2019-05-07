//
// Created by RainZhong on 2019/2/4.
//

#include "../Include/group.h"

Group::Group(int numOfObjects)
{
    this->numOfObjects = numOfObjects;
    objects = new Object3DPtr[numOfObjects];
    boundingBox = nullptr;
    type = GroupType;
}

Group::~Group()
{
    for(int i = 0;i < numOfObjects;i++)
    {
        if(objects[i] != nullptr)
            delete objects[i];
    }

    delete[] objects;
    if(boundingBox != nullptr)
        delete boundingBox;
}


void Group::reconstruct()
{
    vector<Object3D*> v;
    for(int i = 0;i < numOfObjects;i++)
    {
        if(objects[i]->getType() == GroupType || objects[i]->getType() == TransformType)
        {
            objects[i]->addToVector(v, nullptr);
        } else
        {
            v.push_back(objects[i]);
            objects[i] = nullptr;
        }
    }

    for(int i = 0;i < numOfObjects;i++)
    {
        if(objects[i] != nullptr)
        {
            delete objects[i];
        }
    }

    delete[] objects;

    numOfObjects = v.size();
    objects = new Object3D* [numOfObjects];

    BoundingBox* temp;

    for(int i = 0;i < numOfObjects;i++)
    {
        objects[i] = v[i];
        temp = objects[i]->getBoundingBox();
        if(temp != nullptr) {
            if (boundingBox == nullptr) {
                boundingBox = new BoundingBox(temp->getMin(), temp->getMax());
            } else {
                boundingBox->Extend(temp);
            }
        }
    }


}


void Group::addToVector(vector<Object3D *> &v, Matrix *m)
{
    for(int i = 0;i < numOfObjects;i++)
    {
        if(objects[i]->getType() == GroupType || objects[i]->getType() == TransformType)
        {
            objects[i]->addToVector(v, m);
        } else
        {
            if(m == nullptr)
            {
                v.push_back(objects[i]);
            }
            else
            {
                auto temp = (Object3D*)new Transform(*m,objects[i]);
                v.push_back(temp);
            }
            objects[i] = nullptr;
        }
    }
}



void Group::addObject(int index, Object3D *obj)
{
    objects[index] = obj;
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

