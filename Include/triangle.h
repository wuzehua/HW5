//
// Created by RainZhong on 2019/2/5.
//

#ifndef HW2_TRIANGLE_H
#define HW2_TRIANGLE_H

#include "object3d.h"
#include "matrix.h"
#include "grid.h"

class Triangle:public Object3D
{
public:
    Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m);
    ~Triangle()
    {
        //delete material;
        delete boundingBox;
    }

    bool intersect(const Ray &r, Hit &h, float tmin);
    bool intersectShadow(const Ray& r, Hit& h, float tmin);

    void insertIntoGrid(Grid *g, Matrix *m);

    BoundingBox* getTransformBoundingBox(Matrix* m);

    void paint();

private:
    Material* material;
    Vec3f a;
    Vec3f b;
    Vec3f c;
    Vec3f normal;
    float offset;
};

#endif //HW2_TRIANGLE_H
