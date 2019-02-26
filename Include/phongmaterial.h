//
// Created by RainZhong on 18.02.2019.
//

#ifndef HW3_PHONGMATERIAL_H
#define HW3_PHONGMATERIAL_H


#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glut.h>

#include "material.h"
#include "glCanvas.h"

class PhongMaterial:public Material
{

public:
    PhongMaterial(const Vec3f &d_color, const Vec3f& specularColor,float exponent,const Vec3f &reflectiveColor,
                  const Vec3f &transparentColor,
                  float indexOfRefraction):Material(d_color,specularColor,reflectiveColor,transparentColor,indexOfRefraction)
    {
        this->exponent = exponent;
    }

    void glSetMaterial(void) const;


    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,const Vec3f &lightColor) const;


private:
    float exponent;

};




#endif //HW3_PHONGMATERIAL_H
