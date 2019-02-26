//
// Created by RainZhong on 20.02.2019.
//

#include "../Include/raytracer.h"

RayTracer::RayTracer(SceneParser *s, int max_bounce, float cutoff_weight, bool shadow, bool shadeBack)
{
    parser = s;
    maxBounce = max_bounce;
    cutOffWeight = cutoff_weight;
    this->shadow = shadow;
    this->shadeBack = shadeBack;
    if(s != NULL)
    {
        group = s->getGroup();
        numOfLights = s->getNumLights();
        lights = new Light* [numOfLights];
        for(int i = 0;i < numOfLights;i++)
        {
            lights[i] = s->getLight(i);
        }

        ambient = s->getAmbientLight();

    }

}



Vec3f RayTracer::traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit) const
{
    if(bounces > maxBounce) return Vec3f(0,0,0);
    if(weight < cutOffWeight) return Vec3f(0,0,0);
    bool ishit = group->intersect(ray,hit,tmin);
    Vec3f color(0,0,0);
    if(ishit)
    {
        Material* material = hit.getMaterial();
        color = ambient * material->getDiffuseColor();
        Vec3f directionToLight;
        Vec3f hitPoint;
        Vec3f lightColor;
        float distanceToLight;
        float epsilon = 5e-4;
        hitPoint = hit.getIntersectionPoint();



        float dot = ray.getDirection().Dot3(hit.getNormal());



        if(dot > 0)
        {
            if(shadeBack) {
                //若渲染背面则翻转法向量
                Vec3f normal = hit.getNormal();
                normal.Negate();
                hit.set(hit.getT(),hit.getMaterial(),normal,ray);
            } else
            {
                return Vec3f(0,0,0);
            }
        }




        RayTree::SetMainSegment(ray,tmin,hit.getT());

        //渲染阴影

        for(int i = 0;i < numOfLights;i++)
        {
            lights[i]->getIllumination(hitPoint,directionToLight,lightColor,distanceToLight);
            //directionToLight.Normalize();

            if(shadow)
            {
                //在交点处发出虚拟的阴影光线
                Ray shadowRay(hitPoint, directionToLight);
                Hit h(distanceToLight, NULL, Vec3f(0, 0, 0));

                //如果该虚拟光线与物体有交，则不叠加光照颜色
                if (group->intersectShadow(shadowRay, h, epsilon) && h.getT() < distanceToLight)
                {
                    continue;
                }


            }
            color += material->Shade(ray, hit, directionToLight, lightColor);

        }


        //渲染反射
        if(material->getReflectiveColor().Length() > epsilon)
        {
            Vec3f outDirection;
            outDirection = mirrorDirection(hit.getNormal(),ray.getDirection());
            Ray reflectRay(hitPoint,outDirection);
            Hit h(FLT_MAX,NULL,Vec3f(0,0,0));
            //迭代
            color += material->getReflectiveColor() *
                    traceRay(reflectRay,epsilon,bounces + 1,weight * material->getReflectiveColor().Length() / sqrtf(3),indexOfRefraction,h);

            if(bounces < maxBounce)
                RayTree::AddReflectedSegment(reflectRay,0,h.getT());
        }


        //渲染折射
        if(material->getTransparentColor().Length() > epsilon)
        {
            Vec3f transmitted;
            bool trans;
            float t_index;

            //若点积小于0，则光线在真空中，否则在物体内部
            t_index = dot < 0 ? material->getIndexOfRefraction() : 1;

            //判断是否会有全反射
            trans = transmittedDirection(hit.getNormal(),ray.getDirection(),indexOfRefraction,t_index,transmitted);

            if(trans)
            {
                Ray transRay(hitPoint,transmitted);
                Hit transHit(FLT_MAX,NULL,Vec3f(0,0,0));
                //迭代
                color += material->getTransparentColor() * traceRay(transRay,epsilon,bounces + 1,weight * material->getTransparentColor().Length() / sqrtf(3),t_index,transHit);
                RayTree::AddTransmittedSegment(transRay,0,transHit.getT());
            }

        }


    } else
    {
        //若ray没有交则渲染背景色
        color = parser->getBackgroundColor();
    }

    return color;
}


Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &income) const
{
    Vec3f out;
    Vec3f newNormal = normal;
    Vec3f newIncome = income;
    newNormal.Normalize();
    newIncome.Normalize();

    out = newIncome - 2 * newNormal.Dot3(newIncome) * newNormal;

    out.Normalize();

    return out;
}


bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t,
                                     Vec3f &transmitted) const
{
    Vec3f newNormal = normal;
    Vec3f inLight = incoming;
    inLight.Negate();
    inLight.Normalize();
    newNormal.Normalize();
    if(inLight.Dot3(newNormal) < 0)
        newNormal.Negate();
    float ita = index_i / index_t;
    float dotin = newNormal.Dot3(inLight);
    float costht2 = 1 - ita * ita * (1 - dotin * dotin);

    if(costht2 < 0)
    {
        return false;
    }

    transmitted = (ita * dotin - sqrtf(costht2)) * normal - ita * inLight;
    transmitted.Normalize();
    return true;

}