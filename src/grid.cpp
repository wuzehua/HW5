//
// Created by RainZhong on 27.02.2019.
//

#include "../Include/grid.h"

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


