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

}


void MarchingInfo::nextCell()
{

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