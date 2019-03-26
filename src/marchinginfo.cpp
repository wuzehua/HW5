//
// Created by RainZhong on 05.03.2019.
//

#include "../Include/marchinginfo.h"

MarchingInfo::MarchingInfo()
{

}



void MarchingInfo::setSign(const Ray &r)
{
    Vec3f direction = r.getDirection();

    if(direction.x() < 0)
        signx = -1;
    else
        signx = 1;

    if(direction.y() < 0)
        signy = -1;
    else 
        signy = 1;

    if(direction.z() < 0)
        signz = -1;
    else
        signz = 1;

}


void MarchingInfo::nextCell()
{
    if(x < nx && x >= 0 && y < ny && y >= 0 && z < nz && z >= 0)
    {
        
    }
}

void MarchingInfo::setup()
{

}


void MarchingInfo::createEnterCell()
{

}


void MarchingInfo::createEnterFace()
{

}