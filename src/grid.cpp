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



Grid::Grid(BoundingBox *bb, int nx, int ny, int nz)
{
    show = new bool [nx * ny * nz];
    this->nx = nx;
    this->ny = ny;
    this->nz = nz;

    boundingBox = bb;

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

    direction.Normalize();

    if((origin.x() < min.x() || origin.x() > max.x()) && direction.x() == 0)
        return false;

    if((origin.y() < min.y() || origin.y() > max.y()) && direction.y() == 0)
        return false;

    if((origin.z() < min.z() || origin.z() > max.z()) && direction.z() == 0)
        return false;


    float t1x = (min.x() - origin.x()) / direction.x();
    float t2x = (max.x() - origin.x()) / direction.x();

    if(t1x > t2x)
        swap(t1x,t2x);

    float t1y = (min.y() - origin.y()) / direction.y();
    float t2y = (max.y() - origin.y()) / direction.y();

    if(t1y > t2y)
        swap(t1y,t2y);

    float t1z = (min.z() - origin.z()) / direction.z();
    float t2z = (max.z() - origin.z()) / direction.z();

    if(t1z > t2z)
        swap(t1z,t2z);


    float tnear = max3(t1x,t1y,t1z);
    float tfar = min3(t2x,t2y,t2z);

    if(tnear > tfar || tfar < tmin)
        return false;


    Vec3f startPoint;
    if(tnear > tmin)
        startPoint = origin + tnear * direction;
    else
        startPoint = origin;

}



void Grid::paint()
{

}



void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const
{


}