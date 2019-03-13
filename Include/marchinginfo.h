//
// Created by RainZhong on 05.03.2019.
//

#ifndef HW5_MARCHINGINFO_H
#define HW5_MARCHINGINFO_H

#include "boundingbox.h"
#include "ray.h"

class MarchingInfo
{
public:
    MarchingInfo();
    void nextCell();
    void createEnterCell();
    void createEnterFace();
    bool isInTheBox(){ return isInBox;}

    void setup();

    void setBoundingBox(BoundingBox* b){ boundingBox = b;}
    void setNxyz(int _nx,int _ny,int _nz)
    {
        nx = _nx;
        ny = _ny;
        nz = _nz;
    }
    void setTmin(float t){ tmin = t;}
    void setSign(const Ray& r);


private:
    int nx,ny,nz;
    int signx,signy,signz;
    int x,y,z;
    float dx,dy,dz;
    float tmin;
    float tnear;
    bool isInBox;
    BoundingBox* boundingBox;
    Vec3f origin;


};

#endif //HW5_MARCHINGINFO_H
