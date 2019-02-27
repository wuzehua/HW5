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