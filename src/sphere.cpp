//
// Created by RainZhong on 2019/2/1.
//

#include "../Include/sphere.h"
#include "../Include/usefulfuctions.h"



int Sphere::theta_Step = 10;
int Sphere::phi_Step = 10;
bool Sphere::gouraud = false;



bool Sphere::intersectGeo(const Ray &r, Hit &h, float tmin)
{
    Vec3f direction = r.getDirection();
    Vec3f hitPoint;
    Vec3f normal;
    Ray newRay(r.getOrigin() - center,direction);
    Ray ray(r.getOrigin(),direction);
    float tp = - newRay.getOrigin().Dot3(direction);
    float ro2 = newRay.getOrigin().Dot3(newRay.getOrigin());
    float r2 = radius * radius;

    float d2 = ro2 - tp * tp;
    if(d2 > r2)
    {
        return false;
    }

    float tdot = sqrt(r2 - d2);
    float t;



    //t = ro2 >= r2 ? tp - tdot : tp + tdot;
    t = tp - tdot;

    t = t <= tmin ? tp + tdot : t;


    if(t > tmin && t < h.getT())
    {
        hitPoint = newRay.getOrigin() + t * newRay.getDirection();
        normal = hitPoint - center;
        normal.Normalize();
        h.set(t,material,normal,ray);
        return true;
    }


    return false;
}


bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f origin = r.getOrigin();
    Vec3f direction = r.getDirection();

    Ray ray(origin - center,direction);

    float a = ray.getDirection().Dot3(ray.getDirection());
    float b = 2 * ray.getDirection().Dot3(ray.getOrigin());
    float c = ray.getOrigin().Dot3(ray.getOrigin()) - radius * radius;

    float delta = b * b - 4 * a * c;

    if(delta < 0)
    {
        return false;
    }

    float d = sqrt(delta);

    float t = (-b - d) / (2 * a);


    if(t <= tmin)
    {
        t = (d - b) / (2 * a);
    }

    if(t > tmin && t < h.getT())
    {
        Vec3f normal = ray.getOrigin() + t * ray.getDirection();
        normal.Normalize();
        h.set(t,material,normal,r);
        return true;
    }

    return false;
}


bool Sphere::intersectShadow(const Ray &r, Hit &h, float tmin)
{
    return intersect(r,h,tmin);
}


void Sphere::insertIntoGrid(Grid *g, Matrix *m)
{
    Matrix matrix;
    if(m != NULL)
    {
        matrix = *m;
        matrix.Inverse();
    }
    else
    {
        matrix.SetToIdentity();
    }

    Vec3f cellCenter;
    float dmin = Vec3f(g->getdx(),g->getdy(),g->getdz()).Length() / 2;

    int xmin = 0,xmax = g->getnx();
    int ymin = 0,ymax = g->getny();
    int zmin = 0,zmax = g->getnz();



    for(int x = xmin;x < xmax;x++)
    {
        for(int y = ymin;y < ymax;y++)
        {
            for(int z = zmin;z < zmax;z++)
            {
                cellCenter = g->getCenterOfCell(x,y,z);
                matrix.Transform(cellCenter);
                if((cellCenter - center).Length() < radius + dmin)
                {
                    g->setGridShow(x,y,z);
                }
            }
        }
    }

}

void Sphere::paint()
{
    if(theta_Step == 0 || phi_Step == 0) return;
    float dtheta = 2 *  M_PI / (float)theta_Step;
    float dphi = M_PI / (float)phi_Step;

    float theta,phi;

    Vec3f x(radius,0,0);
    Vec3f y(0,radius,0);
    Vec3f z(0,0,radius);

    material->glSetMaterial();

    glBegin(GL_QUADS);
    for (int itheta = 0;itheta < theta_Step;itheta++)
    {
        for(int iphi = 0;iphi < phi_Step;iphi++)
        {
            theta = (float)itheta * dtheta;
            phi = (float)iphi * dphi - M_PI / 2;
            float costh = cosf(theta);
            float sinth = sinf(theta);
            float cosph = cosf(phi);
            float sinph = sinf(phi);
            float cosdth = cosf(theta + dtheta);
            float sindth = sinf(theta + dtheta);
            float cosdph = cosf(phi + dphi);
            float sindph = sinf(phi + dphi);


            Vec3f v1 = center + x * cosdth * cosdph
                    + y * sindth * cosdph + z * sindph;
            Vec3f v2 = center + x * costh * cosdph
                    + y * sinth * cosdph + z * sindph;
            Vec3f v3 = center + x * costh * cosph
                    + y * sinth * cosph + z * sinph;
            Vec3f v4 = center + x * cosdth * cosph
                    + y * sindth * cosph + z * sinph;

            Vec3f newNormal;

            if(!gouraud)
            {
                Vec3f::Cross3(newNormal, v4 - v3, v2 - v3);
                newNormal.Normalize();

                if (newNormal.Dot3(v1 - center) < 0) {
                    newNormal.Negate();
                }

                glNormal3f(newNormal.x(), newNormal.y(), newNormal.z());
            }

            if(gouraud)
            {
                newNormal = v1 - center;
                glNormal3f(newNormal.x(), newNormal.y(), newNormal.z());
            }

            glVertex3f(v1.x(),v1.y(),v1.z());


            if(gouraud)
            {
                newNormal = v2 - center;
                glNormal3f(newNormal.x(), newNormal.y(), newNormal.z());
            }

            glVertex3f(v2.x(),v2.y(),v2.z());


            if(gouraud)
            {
                newNormal = v3 - center;
                glNormal3f(newNormal.x(), newNormal.y(), newNormal.z());
            }

            glVertex3f(v3.x(),v3.y(),v3.z());


            if(gouraud)
            {
                newNormal = v4 - center;
                glNormal3f(newNormal.x(), newNormal.y(), newNormal.z());
            }

            glVertex3f(v4.x(),v4.y(),v4.z());

        }
    }
    glEnd();

}

void Sphere::setStep(int theta, int phi)
{
    theta_Step = theta;
    phi_Step = phi;
}


void Sphere::setGouraud(bool gouraud)
{
    Sphere::gouraud = gouraud;
}