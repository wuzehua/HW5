//
// Created by RainZhong on 2019/3/11.
//

#include "../Include/gridmaterial.h"

GridMaterial::GridMaterial():Material(Vec3f(),Vec3f(),Vec3f(),Vec3f(),1)
{
    currentIndex = 0;
    colors[0].Set(0.8,0.8,0.8);
    colors[1].Set(0.0,0.8,0.5);
    colors[2].Set(0.0,0.9,0.0);
    colors[3].Set(0.5,0.5,0.1);
    colors[4].Set(0.0,0.0,0.9);
    colors[5].Set(0.9,0.0,0.0);
}

Vec3f GridMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    Vec3f v = ray.getDirection();
    v.Negate();
    v.Normalize();

    Vec3f normal = hit.getNormal();
    normal.Normalize();

    float d = dirToLight.Dot3(normal);
    d = d < 0 ? 0 : d;

    return d * lightColor * colors[currentIndex];
}


void GridMaterial::glSetMaterial() const
{
    GLfloat color[4] =
            {
                colors[currentIndex].r(),
                colors[currentIndex].g(),
                colors[currentIndex].b(),
                1
            };

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, color);
}