//
// Created by RainZhong on 24.02.2019.
//

#ifndef HW5_GRID_H
#define HW5_GRID_H

#include "object3d.h"
#include "marchinginfo.h"
#include "gridmaterial.h"

class Grid:public Object3D
{
public:
    Grid(BoundingBox *bb, int nx, int ny, int nz);
    ~Grid()
    {
        delete[] show;
        delete material;
    }


    bool intersect(const Ray &r, Hit &h, float tmin);

    void paint();

    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;

    void setNumOfColor(int num)
    {
        if(num <= 0)
        {
            numOfColor = 1;
        }else if(num > 6)
        {
            numOfColor = 6;
        } else{
            numOfColor = num;
        }
    }

    void setGridShow(int x,int y,int z);
    const int getnx(){return nx;}
    const int getny(){return ny;}
    const int getnz(){return nz;}
    const float getdx(){return dx;}
    const float getdy(){return dy;}
    const float getdz(){return dz;}
    Vec3f getCenterOfCell(int x,int y,int z);
    GridMaterial* getGridMaterial() {return material;}


private:
    bool* show;
    int nx,ny,nz;
    float dx,dy,dz;
    int numOfColor;
    GridMaterial* material;
};
#endif //HW5_GRID_H
