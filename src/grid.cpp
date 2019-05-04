//
// Created by RainZhong on 27.02.2019.
//

#include "../Include/grid.h"
#include "../Include/usefulfuctions.h"




void getTs(float& t1x, float& t2x, float& t1y, float& t2y, float& t1z, float& t2z
        , const Vec3f& origin, const Vec3f& direction, const Vec3f& min,const Vec3f& max)
{
    t1x = (min.x() - origin.x()) / direction.x();
    t2x = (max.x() - origin.x()) / direction.x();

    if(t1x > t2x)
        swap(t1x,t2x);

    t1y = (min.y() - origin.y()) / direction.y();
    t2y = (max.y() - origin.y()) / direction.y();

    if(t1y > t2y)
        swap(t1y,t2y);

    t1z = (min.z() - origin.z()) / direction.z();
    t2z = (max.z() - origin.z()) / direction.z();

    if(t1z > t2z)
        swap(t1z,t2z);

    if(direction.x() == 0.f)
    {
        t1x = -FLT_MAX;
        t2x = FLT_MAX;
    }

    if(direction.y() == 0.f)
    {
        t1y = -FLT_MAX;
        t2y = FLT_MAX;
    }

    if(direction.z() == 0.f)
    {
        t1z = -FLT_MAX;
        t2z = FLT_MAX;
    }

}


void getNormal(int normalDir, Vec3f& normal)
{
    switch (normalDir) {
        case 1:
            normal.Set(-1, 0, 0);
            break;
        case -1:
            normal.Set(1, 0, 0);
            break;
        case 2:
            normal.Set(0,-1,0);
            break;
        case -2:
            normal.Set(0,1,0);
            break;
        case 4:
            normal.Set(0,0,-1);
            break;
        case -4:
            normal.Set(0,0,1);
            break;
    }
}


void Grid::addEnterFace(int normalDirection,Vec3f center)
{
    if(normalDirection == -1)//Vec朝x轴正方向
    {
        RayTree::AddEnteredFace(center + Vec3f(dx / 2,-dy / 2,-dz / 2),
                                center + Vec3f(dx / 2,-dy / 2,dz / 2),
                                center + Vec3f(dx / 2,dy / 2,dz / 2),
                                center + Vec3f(dx / 2,dy / 2,-dz / 2),
                                Vec3f(1,0,0),
                                material);
    }
    else if(normalDirection == 1)
    {
        RayTree::AddEnteredFace(center + Vec3f(-dx / 2,-dy / 2,-dz / 2),
                                center + Vec3f(-dx / 2,-dy / 2,dz / 2),
                                center + Vec3f(-dx / 2,dy / 2,dz / 2),
                                center + Vec3f(-dx / 2,dy / 2,-dz / 2),
                                Vec3f(-1,0,0),
                                material);
    }
    else if(normalDirection == -2)
    {
        RayTree::AddEnteredFace(center + Vec3f(-dx / 2,dy / 2,-dz / 2),
                                center + Vec3f(-dx / 2,dy / 2,dz / 2),
                                center + Vec3f(dx / 2,dy / 2,dz / 2),
                                center + Vec3f(dx / 2,dy / 2,-dz / 2),
                                Vec3f(0,1,0),
                                material);
    }
    else if(normalDirection == 2)
    {
        RayTree::AddEnteredFace(center + Vec3f(-dx / 2,-dy / 2,-dz / 2),
                                center + Vec3f(-dx / 2,-dy / 2,dz / 2),
                                center + Vec3f(dx / 2,-dy / 2,dz / 2),
                                center + Vec3f(dx / 2,-dy / 2,-dz / 2),
                                Vec3f(0,-1,0),
                                material);
    }
    else if(normalDirection == -4)
    {
        RayTree::AddEnteredFace(center + Vec3f(-dx / 2,-dy / 2,dz / 2),
                                center + Vec3f(-dx / 2,dy / 2,dz / 2),
                                center + Vec3f(dx / 2,dy / 2,dz / 2),
                                center + Vec3f(dx / 2,-dy / 2,dz / 2),
                                Vec3f(0,0,1),
                                material);
    }
    else
    {
        RayTree::AddEnteredFace(center + Vec3f(-dx / 2,-dy / 2,-dz / 2),
                                center + Vec3f(-dx / 2,dy / 2,-dz / 2),
                                center + Vec3f(dx / 2,dy / 2,-dz / 2),
                                center + Vec3f(dx / 2,-dy / 2,-dz / 2),
                                Vec3f(0,0,-1),
                                material);
    }

}


Grid::Grid(BoundingBox *bb, int nx, int ny, int nz)
{
    show = new bool [nx * ny * nz];
    this->nx = nx;
    this->ny = ny;
    this->nz = nz;

    boundingBox = bb;
    numOfColor = 1;

    material = new GridMaterial();

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



bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{

    RayTree::SetMainSegment(r,tmin,FLT_MAX);
    Vec3f origin = r.getOrigin();
    Vec3f direction = r.getDirection();
    Vec3f min = boundingBox->getMin();
    Vec3f max = boundingBox->getMax();
    float t1x,t2x,t1y,t2y,t1z,t2z;

    direction.Normalize();

    //std::cout<<"Direction:"<<direction.x()<<","<<direction.y()<<","<<direction.z()<<std::endl;

    if((origin.x() < min.x() || origin.x() > max.x()) && direction.x() == 0.f)
        return false;

    if((origin.y() < min.y() || origin.y() > max.y()) && direction.y() == 0.f)
        return false;

    if((origin.z() < min.z() || origin.z() > max.z()) && direction.z() == 0.f)
        return false;




    getTs(t1x,t2x,t1y,t2y,t1z,t2z,origin,direction,min,max);


    float tnear = max3(t1x,t1y,t1z);
    float tfar = min3(t2x,t2y,t2z);

    if(tnear > tfar || tfar < tmin)
        return false;


    Vec3f startPoint;
    if(tnear > tmin)
        startPoint = origin + tnear * direction;
    else
        startPoint = origin;

    int x = (int)((startPoint - min).x() / dx);
    int y = (int)((startPoint - min).y() / dy);
    int z = (int)((startPoint - min).z() / dz);

    int signx,signy,signz;

    if(direction.x() > 0)
        signx = 1;
    else
        signx = -1;

    if(direction.y() > 0)
        signy = 1;
    else
        signy = -1;

    if(direction.z() > 0)
        signz = 1;
    else
        signz = -1;




    bool hit = false;

    float tnextx,tnexty,tnextz;
    int normalDirection;

    min = getCenterOfCell(x,y,z) - Vec3f(dx / 2,dy / 2,dz / 2);
    max = getCenterOfCell(x,y,z) + Vec3f(dx / 2,dy / 2,dz / 2);

    if(tnear == t1x)
    {
        normalDirection = (-signx);
        tnextx = tnear;
        getTs(t1x,t2x,t1y,t2y,t1z,t2z,origin,direction,min,max);
        tnexty = t2y;
        tnextz = t2z;
    }
    else if(tnear == t1y)
    {
        normalDirection = (-signy) * 2;
        tnexty = tnear;
        getTs(t1x,t2x,t1y,t2y,t1z,t2z,origin,direction,min,max);
        tnextx = t2x;
        tnextz = t2z;
    }
    else
    {
        normalDirection = (-signz) * 4;
        tnextz = tnear;
        getTs(t1x,t2x,t1y,t2y,t1z,t2z,origin,direction,min,max);
        tnexty = t2y;
        tnextx = t2x;
    }


    float dtx = fabsf(dx / direction.x());
    float dty = fabsf(dy / direction.y());
    float dtz = fabsf(dz / direction.z());
    Vec3f center;

    while (x < nx && y < ny && z < nz && x >= 0 && y >= 0 && z >= 0) {

        //std::cout<<"Grid("<<x<<","<<y<<","<<z<<") ";
        //std::cout<<"tnextx:"<<tnextx;
        //std::cout<<"tnexty:"<<tnexty;
        //std::cout<<"tnextz:"<<tnextz<<std::endl;
        center = getCenterOfCell(x,y,z);
        addEnterFace(normalDirection,center);

        if (show[x * ny * nz + y * nz + z]) {
            if (tmin < tnear) {
                hit = true;
                break;
            } else {
                if (origin == startPoint) {
                    tnear = min3(t2x, t2y, t2z);
                    hit = true;
                    break;
                }
            }
        }

        if (tnextx < tnexty && tnextx < tnextz) {
            normalDirection = (-signx);
            x += signx;
            tnextx += dtx;
            tnear = tnextx;
        } else {
            if (tnexty < tnextz) {
                normalDirection = (-signy) * 2;
                y += signy;
                tnexty += dty;
                tnear = tnexty;
            } else {
                normalDirection = (-signz) * 4;
                z += signz;
                tnextz += dtz;
                tnear = tnextz;
            }
        }
    }

    if(hit)
    {
        Vec3f normal;
        material->setCurrentIndex((x + y + z) % numOfColor);

        switch (normalDirection) {
            case 1:
                normal.Set(-1, 0, 0);
                break;
            case -1:
                normal.Set(1, 0, 0);
                break;
            case 2:
                normal.Set(0,-1,0);
                break;
            case -2:
                normal.Set(0,1,0);
                break;
            case 4:
                normal.Set(0,0,-1);
                break;
            case -4:
                normal.Set(0,0,1);
                break;
        }

        h.set(tnear,material,normal,r);
    }

    return hit;
}



void Grid::paint()
{
    Vec3f center;
    Vec3f min,max;
    Vec3f delta(dx/2,dy/2,dz/2);

    for(int x = 0;x < nx;x++)
    {
        for(int y = 0;y < ny;y++)
        {
            for (int z = 0; z < nz; z++)
            {
                if(!show[x * ny * nz + y * nz + z])
                    continue;

                center = getCenterOfCell(x,y,z);
                min = center - delta;
                max = center + delta;

                material->setCurrentIndex((x + y + z) % numOfColor);
                material->glSetMaterial();
                glBegin(GL_QUADS);

                //先对垂直于x轴上的面进行处理
                if(x == 0)
                {
                    glNormal3f(-1.0f,0.0f,0.0f);
                    glVertex3f(min.x(),min.y(),min.z());
                    glVertex3f(min.x(),min.y(),max.z());
                    glVertex3f(min.x(),max.y(),max.z());
                    glVertex3f(min.x(),max.y(),min.z());

                    if(x == nx - 1 || (x < nx - 1 && !show[(x + 1) * ny * nz + y * nz + z]))
                    {
                        glNormal3f(1.0f,0.0f,0.0f);
                        glVertex3f(max.x(),min.y(),min.z());
                        glVertex3f(max.x(),min.y(),max.z());
                        glVertex3f(max.x(),max.y(),max.z());
                        glVertex3f(max.x(),max.y(),min.z());
                    }
                }
                else if(x == nx - 1)
                {
                    glNormal3f(1.0f,0.0f,0.0f);
                    glVertex3f(max.x(),min.y(),min.z());
                    glVertex3f(max.x(),min.y(),max.z());
                    glVertex3f(max.x(),max.y(),max.z());
                    glVertex3f(max.x(),max.y(),min.z());

                    if(x > 0 && !show[(x - 1) * ny * nz + y * nz + z])
                    {
                        glNormal3f(-1.0f,0.0f,0.0f);
                        glVertex3f(min.x(),min.y(),min.z());
                        glVertex3f(min.x(),min.y(),max.z());
                        glVertex3f(min.x(),max.y(),max.z());
                        glVertex3f(min.x(),max.y(),min.z());
                    }
                }
                else
                {
                    if(!show[(x - 1) * ny * nz + y * nz + z])
                    {
                        glNormal3f(-1.0f,0.0,0.0);
                        glVertex3f(min.x(),min.y(),min.z());
                        glVertex3f(min.x(),min.y(),max.z());
                        glVertex3f(min.x(),max.y(),max.z());
                        glVertex3f(min.x(),max.y(),min.z());
                    }

                    if(!show[(x + 1) * ny * nz + y * nz + z])
                    {
                        glNormal3f(1.0f,0.0f,0.0f);
                        glVertex3f(max.x(),min.y(),min.z());
                        glVertex3f(max.x(),min.y(),max.z());
                        glVertex3f(max.x(),max.y(),max.z());
                        glVertex3f(max.x(),max.y(),min.z());
                    }
                }

                if(y == 0)
                {
                    glNormal3f(0.0f,-1.0f,0.0f);
                    glVertex3f(min.x(),min.y(),min.z());
                    glVertex3f(max.x(),min.y(),min.z());
                    glVertex3f(max.x(),min.y(),max.z());
                    glVertex3f(min.x(),min.y(),max.z());

                    if(y == ny - 1 || (y < ny - 1 && !show[x * ny * nz + (y + 1) * nz + z]))
                    {
                        glNormal3f(0.0f,1.0f,0.0f);
                        glVertex3f(min.x(),max.y(),min.z());
                        glVertex3f(max.x(),max.y(),min.z());
                        glVertex3f(max.x(),max.y(),max.z());
                        glVertex3f(min.x(),max.y(),max.z());
                    }

                }
                else if(y == ny - 1)
                {
                    glNormal3f(0.0f,1.0f,0.0f);
                    glVertex3f(min.x(),max.y(),min.z());
                    glVertex3f(max.x(),max.y(),min.z());
                    glVertex3f(max.x(),max.y(),max.z());
                    glVertex3f(min.x(),max.y(),max.z());

                    if(y > 0 && !show[x * ny * nz + (y - 1) * nz + z])
                    {
                        glNormal3f(0.0f,-1.0f,0.0f);
                        glVertex3f(min.x(),min.y(),min.z());
                        glVertex3f(max.x(),min.y(),min.z());
                        glVertex3f(max.x(),min.y(),max.z());
                        glVertex3f(min.x(),min.y(),max.z());
                    }
                }
                else
                {
                    if(!show[x * ny * nz + (y + 1) * nz + z])
                    {
                        glNormal3f(0.0f,1.0f,0.0f);
                        glVertex3f(min.x(),max.y(),min.z());
                        glVertex3f(max.x(),max.y(),min.z());
                        glVertex3f(max.x(),max.y(),max.z());
                        glVertex3f(min.x(),max.y(),max.z());
                    }

                    if(!show[x * ny * nz + (y - 1) * nz + z])
                    {
                        glNormal3f(0.0f,-1.0f,0.0f);
                        glVertex3f(min.x(),min.y(),min.z());
                        glVertex3f(max.x(),min.y(),min.z());
                        glVertex3f(max.x(),min.y(),max.z());
                        glVertex3f(min.x(),min.y(),max.z());
                    }
                }


                if(z == 0)
                {
                    glNormal3f(0.0f,0.0f,-1.0f);
                    glVertex3f(min.x(),min.y(),min.z());
                    glVertex3f(max.x(),min.y(),min.z());
                    glVertex3f(max.x(),max.y(),min.z());
                    glVertex3f(min.x(),max.y(),min.z());

                    if(z == nz - 1 || (z < nz - 1 && !show[x * ny * nz + y * nz + (z + 1)]))
                    {
                        glNormal3f(0.0f,0.0f,1.0f);
                        glVertex3f(min.x(),min.y(),max.z());
                        glVertex3f(max.x(),min.y(),max.z());
                        glVertex3f(max.x(),max.y(),max.z());
                        glVertex3f(min.x(),max.y(),max.z());
                    }
                }
                else if(z == nz - 1)
                {
                    glNormal3f(0.0f,0.0f,1.0f);
                    glVertex3f(min.x(),min.y(),max.z());
                    glVertex3f(max.x(),min.y(),max.z());
                    glVertex3f(max.x(),max.y(),max.z());
                    glVertex3f(min.x(),max.y(),max.z());

                    if(z > 0 && !show[x * ny * nz + y * nz + (z - 1)])
                    {
                        glNormal3f(0.0f,0.0f,-1.0f);
                        glVertex3f(min.x(),min.y(),min.z());
                        glVertex3f(max.x(),min.y(),min.z());
                        glVertex3f(max.x(),max.y(),min.z());
                        glVertex3f(min.x(),max.y(),min.z());
                    }
                }
                else
                {
                    if(!show[x * ny * nz + y * nz + (z + 1)])
                    {
                        glNormal3f(0.0f,0.0f,1.0f);
                        glVertex3f(min.x(),min.y(),max.z());
                        glVertex3f(max.x(),min.y(),max.z());
                        glVertex3f(max.x(),max.y(),max.z());
                        glVertex3f(min.x(),max.y(),max.z());
                    }

                    if(!show[x * ny * nz + y * nz + (z - 1)])
                    {
                        glNormal3f(0.0f,0.0f,-1.0f);
                        glVertex3f(min.x(),min.y(),min.z());
                        glVertex3f(max.x(),min.y(),min.z());
                        glVertex3f(max.x(),max.y(),min.z());
                        glVertex3f(min.x(),max.y(),min.z());
                    }
                }

                glEnd();
            }
        }
    }

}



void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const
{
    RayTree::SetMainSegment(r,tmin,100);

}