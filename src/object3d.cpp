#include "../Include/matrix.h"
#include "../Include/object3d.h"


BoundingBox* Object3D::getTransformBoundingBox(Matrix* m)
{

    if(boundingBox == NULL)
        return NULL;

    if(m == NULL)
    {
        BoundingBox* b = new BoundingBox(boundingBox->getMin(),boundingBox->getMax());
        return b;
    }

    Vec3f min = boundingBox->getMin();
    Vec3f max = boundingBox->getMax();

    Vec3f v[8];

    v[0].Set(min.x(),min.y(),min.z());
    v[1].Set(min.x(),min.y(),max.z());
    v[2].Set(min.x(),max.y(),max.z());
    v[3].Set(min.x(),max.y(),min.z());

    v[4].Set(max.x(),min.y(),min.z());
    v[5].Set(max.x(),min.y(),max.z());
    v[6].Set(max.x(),max.y(),max.z());
    v[7].Set(max.x(),max.y(),min.z());


    float minx,miny,minz;
    float maxx,maxy,maxz;

    for(int i = 0;i < 8;i++)
    {
        m->Transform(v[i]);
    }

    for(int i = 0;i < 8;i++)
    {
        if(i == 0)
        {
            maxx = minx = v[i].x();
            maxy = miny = v[i].y();
            maxz = minz = v[i].z();
        }
        else
        {
            if(minx > v[i].x())
                minx = v[i].x();
            
            if(miny > v[i].y())
                miny = v[i].y();
            
            if(minz > v[i].z())
                minz = v[i].z();
            
            if(maxx < v[i].x())
                maxx = v[i].x();
            
            if(maxy < v[i].y())
                maxy = v[i].y();

            if(maxz < v[i].z())
                maxz = v[i].z();
        }
    }

    min.Set(minx,miny,minz);
    max.Set(maxx,maxy,maxz);

    BoundingBox* b = new BoundingBox(min,max);
    return b;

}