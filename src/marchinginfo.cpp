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
        sign[0] = -1;
    else
        sign[0] = 1;

    if(direction.y() < 0)
        sign[1] = -1;
    else 
        sign[1] = 1;

    if(direction.z() < 0)
        sign[2] = -1;
    else
        sign[2] = 1;

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