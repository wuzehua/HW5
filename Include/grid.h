//
// Created by RainZhong on 24.02.2019.
//

#ifndef HW5_GRID_H
#define HW5_GRID_H

#include "object3d.h"

class Grid:public Object3D
{
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz);
};
#endif //HW5_GRID_H
