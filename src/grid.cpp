//
// Created by RainZhong on 27.02.2019.
//

#include "../Include/grid.h"

float min3(float x, float y, float z)
{
    float result = x < y ? x : y;
    result = result < z ? result : z;
    return result;
}


float max3(float x1,float x2,float x3)
{
    float result = x1 > x2 ? x1 : x2;
    result = result > x3 ? result : x3;
    return result;
}



void getTs(float& t1x, float& t2x, float& t1y, float& t2y, float& t1z, float& t2z
        , const Vec3f& origin, const Vec3f& direction, const Vec3f& min,const Vec3f& max)
{
    t1x = (min.x() - origin.x()) / direction.x();
    t2x = (max.x() - origin.x()) / direction.x();

    if(t1x > t2x)
        swap(t1x,t2x);

    t1y = (min.y() - origin.y()) / direction.y();
    t2y = (max.y() - origin.y()) / direction.y();

    if(t1y > t2y)
        swap(t1y,t2y);

    t1z = (min.z() - origin.z()) / direction.z();
    t2z = (max.z() - origin.z()) / direction.z();

    if(t1z > t2z)
        swap(t1z,t2z);
}


Grid::Grid(BoundingBox *bb, int nx, int ny, int nz)
{
    show = new bool [nx * ny * nz];
    this->nx = nx;
    this->ny = ny;
    this->nz = nz;

    boundingBox = bb;
    numOfColor = 1;

    material = new GridMaterial();

    float x = bb->getMax().x() - bb->getMin().x();
    float y = bb->getMax().y() - bb->getMin().y();
    float z = bb->getMax().z() - bb->getMin().z();

    dx = x / (float)nx;
    dy = y / (float)ny;
    dz = z / (float)nz;


    for(int i = 0;i < nx;i++)
    {
        for(int j = 0;j < ny;j++)
        {
            for(int k = 0;k < nz;k++)
            {
                show[i * ny * nz + j * nz + k] = false;
            }
        }
    }

}


void Grid::setGridShow(int x, int y, int z)
{
    show[x * ny * nz + y * nz + z] = true;
}


Vec3f Grid::getCenterOfCell(int x, int y, int z)
{
    float cx = ((float)x * 2 + 1) / 2 * dx;
    float cy = ((float)y * 2 + 1) / 2 * dy;
    float cz = ((float)z * 2 + 1) / 2 * dz;

    Vec3f center(cx,cy,cz);//计算以bb min为原点的相对中心位置

    center += boundingBox->getMin();

    return center;
}



bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f origin = r.getOrigin();
    Vec3f direction = r.getDirection();
    Vec3f min = boundingBox->getMin();
    Vec3f max = boundingBox->getMax();
    float t1x,t2x,t1y,t2y,t1z,t2z;

    direction.Normalize();

    if((origin.x() < min.x() || origin.x() > max.x()) && direction.x() == 0)
        return false;

    if((origin.y() < min.y() || origin.y() > max.y()) && direction.y() == 0)
        return false;

    if((origin.z() < min.z() || origin.z() > max.z()) && direction.z() == 0)
        return false;




    getTs(t1x,t2x,t1y,t2y,t1z,t2z,origin,direction,min,max);


    float tnear = max3(t1x,t1y,t1z);
    float tfar = min3(t2x,t2y,t2z);

    if(tnear > tfar || tfar < tmin)
        return false;


    Vec3f startPoint;
    if(tnear > tmin)
        startPoint = origin + tnear * direction;
    else
        startPoint = origin;

    int x = (int)((startPoint - min).x() / dx);
    int y = (int)((startPoint - min).y() / dy);
    int z = (int)((startPoint - min).z() / dz);

    int signx,signy,signz;

    if(direction.x() > 0)
        signx = 1;
    else
        signx = -1;

    if(direction.y() > 0)
        signy = 1;
    else
        signy = -1;

    if(direction.z() > 0)
        signz = 1;
    else
        signz = -1;


    float dtx = dx / direction.x();
    float dty = dy / direction.y();
    float dtz = dz / direction.z();

    bool hit = false;

    float tnextx,tnexty,tnextz;
    int normalDirection;

    min = getCenterOfCell(x,y,z) - Vec3f(dx / 2,dy / 2,dz / 2);
    max = getCenterOfCell(x,y,z) + Vec3f(dx / 2,dy / 2,dz / 2);

    if(tnear == t1x)
    {
        normalDirection = (-signx);
        tnextx = tnear;
        getTs(t1x,t2x,t1y,t2y,t1z,t2z,origin,direction,min,max);
        tnexty = t2y;
        tnextz = t2z;
    }
    else if(tnear == t1y)
    {
        normalDirection = (-signy) << 1;
        tnexty = tnear;
        getTs(t1x,t2x,t1y,t2y,t1z,t2z,origin,direction,min,max);
        tnextx = t2x;
        tnextz = t2z;
    }
    else
    {
        normalDirection = (-signz) << 2;
        tnextz = tnear;
        getTs(t1x,t2x,t1y,t2y,t1z,t2z,origin,direction,min,max);
        tnexty = t2y;
        tnextx = t2x;
    }




    while (x < nx && y < ny && z < nz && x >= 0 && y >= 0 && z >= 0)
    {
        if(show[x * ny * nz + y * nz + z] )
        {
            if(tmin < tnear) {
                hit = true;
                break;
            } else{
                if(origin == startPoint)
                {
                    tnear = min3(t2x,t2y,t2z);
                    hit = true;
                    break;
                }
            }
        }

        if(tnextx < tnexty && tnextx < tnextz)
        {
            normalDirection = (-signx);
            x += signx;
            tnextx += dtx;
            tnear = tnextx;
        }
        else if(tnexty < tnextx && tnexty < tnextz)
        {
            normalDirection = (-signy) << 1;
            y += signy;
            tnexty += dty;
            tnear = tnexty;
        }
        else
        {
            normalDirection = (-signz) << 2;
            z += signz;
            tnextz += dtz;
            tnear = tnextz;
        }
    }

    if(hit)
    {
        Vec3f normal;
        material->setCurrentIndex((x + y + z) % numOfColor);

        switch (normalDirection) {
            case 1:
                normal.Set(-1, 0, 0);
                break;
            case -1:
                normal.Set(1, 0, 0);
                break;
            case 2:
                normal.Set(0,-1,0);
                break;
            case -2:
                normal.Set(0,1,0);
                break;
            case 4:
                normal.Set(0,0,-1);
                break;
            case -4:
                normal.Set(0,0,1);
                break;
        }

        h.set(tnear,material,normal,r);
    }

    return hit;
}



void Grid::paint()
{

}



void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const
{


}