//
// Created by RainZhong on 2019/3/11.
//

#ifndef HW5_GRIDMATERIAL_H
#define HW5_GRIDMATERIAL_H

#include "material.h"
#include <GL/gl.h>

class GridMaterial:public Material
{
public:
    GridMaterial();

    void glSetMaterial(void) const;
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,const Vec3f &lightColor) const;

    void setCurrentIndex(int index){ currentIndex = index;}
    Vec3f getDiffuseColor() const { return colors[currentIndex];}

private:
    Vec3f colors[6];
    int currentIndex;
};


#endif //HW5_GRIDMATERIAL_H
