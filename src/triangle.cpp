//
// Created by RainZhong on 2019/2/5.
//

#include "../Include/triangle.h"
#include "../Include/usefulfuctions.h"



Triangle::Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m)
{
    Vec3f ab = b - a;
    Vec3f bc = c - b;
    Vec3f::Cross3(normal,ab,bc);
    normal.Normalize();
    offset = a.Dot3(normal);

    if(offset < 0)
    {
        normal.Negate();
    }

    this->a = a;
    this->b = b;
    this->c = c;
    material = m;

    float xmin = min3(a.x(),b.x(),c.x());
    float ymin = min3(a.y(),b.y(),c.y());
    float zmin = min3(a.z(),b.z(),c.z());

    float xmax = max3(a.x(),b.x(),c.x());
    float ymax = max3(a.y(),b.y(),c.y());
    float zmax = max3(a.z(),b.z(),c.z());

    Vec3f min(xmin,ymin,zmin);
    Vec3f max(xmax,ymax,zmax);

    boundingBox = new BoundingBox(min,max);
}




bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f direction = r.getDirection();
    Vec3f origin = r.getOrigin();
    Vec3f hit;
    float t,Adet;
    float beta,gama;




    Vec3f amb = a - b;
    Vec3f amc = a - c;
    Vec3f amo = a - origin;
    Matrix A;

    A.Set3x3Col(0,amb);
    A.Set3x3Col(1,amc);
    A.Set3x3Col(2,direction);

    Adet = A.Get3x3Det();

    if(Adet == 0)
    {
        return false;
    }


    A.Set3x3Col(0,amo);
    beta = A.Get3x3Det() / Adet;

    A.Set3x3Col(0,amb);
    A.Set3x3Col(1,amo);
    gama = A.Get3x3Det() / Adet;

    A.Set3x3Col(1,amc);
    A.Set3x3Col(2,amo);
    t = A.Get3x3Det() / Adet;

    //交点在三角形内部的条件
    if(beta >= 0 && gama >= 0 && beta + gama <= 1 && t > tmin && t < h.getT())
    {
        Ray ray(origin,direction);
        h.set(t,material,normal,ray);
        return true;
    }

    return false;
}


bool Triangle::intersectShadow(const Ray &r, Hit &h, float tmin)
{
    return intersect(r,h,tmin);
}


void Triangle::paint()
{
    material->glSetMaterial();
    glBegin(GL_TRIANGLES);
    glNormal3f(normal.x(),normal.y(),normal.z());
    glVertex3f(a.x(),a.y(),a.z());
    glVertex3f(b.x(),b.y(),b.z());
    glVertex3f(c.x(),c.y(),c.z());
    glEnd();
}


void Triangle::insertIntoGrid(Grid *g, Matrix *m)
{

    BoundingBox* b = getTransformBoundingBox(m);


    float xmin,ymin,zmin;
    float xmax,ymax,zmax;

    xmin = b->getMin().x();
    ymin = b->getMin().y();
    zmin = b->getMin().z();

    xmax = b->getMax().x();
    ymax = b->getMax().y();
    zmax = b->getMax().z();

    delete b;

    Vec3f min = g->getBoundingBox()->getMin();

    int xstart,xstop,ystart,ystop,zstart,zstop;

    xstart = (int)((xmin - min.x()) / g->getdx());
    ystart = (int)((ymin - min.y()) / g->getdy());
    zstart = (int)((zmin - min.z()) / g->getdz());

    xstop = (int)((xmax - min.x()) / g->getdx());
    ystop = (int)((ymax - min.y()) / g->getdy());
    zstop = (int)((zmax - min.z()) / g->getdz());


    for(int x = xstart;x < xstop;x++)
    {
        for(int y = ystart;y < ystop;y++)
        {
            for(int z = zstart;z < zstop;z++)
            {
                g->setGridShow(x,y,z);
            }
        }
    }

}


BoundingBox* Triangle::getTransformBoundingBox (Matrix* m)
{
    if(m == NULL)
    {
        BoundingBox* b = new BoundingBox(*boundingBox);
        return b;
    }

    Vec3f a1 = a;
    Vec3f b1 = b;
    Vec3f c1 = c;

    m->Transform(a1);
    m->Transform(b1);
    m->Transform(c1);

    float xmin,ymin,zmin;
    float xmax,ymax,zmax;

    xmin = min3(a1.x(),b1.x(),c1.x());
    ymin = min3(a1.y(),b1.y(),c1.y());
    zmin = min3(a1.z(),b1.z(),c1.z());

    xmax = max3(a1.x(),b1.x(),c1.x());
    ymax = max3(a1.y(),b1.y(),c1.y());
    zmax = max3(a1.z(),b1.z(),c1.z());


    BoundingBox* b = new BoundingBox(Vec3f(xmin,ymin,zmin),Vec3f(xmax,ymax,zmax));

    return b;
}