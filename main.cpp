#include <iostream>
#include "Include/imagerender.h"
#include "Include/glCanvas.h"
#include "Include/marchinginfo.h"


ImageRender render;

void renderImage();


void traceRay(float x, float y);

int main(int argc,char** argv) {
    std::cout << "Hello, World!" << std::endl;

    render.parseCommand(argc,argv);


    if(render.getGUI())
    {
        GLCanvas canvas;
        canvas.initialize(render.getScen(),renderImage,traceRay,render.getGrid(),render.getGridVisual());
    }
    else
    {
        render.renderImage();
    }


    return 0;
}

void renderImage()
{
    render.renderImage();
}

void traceRay(float x, float y)
{
    Camera* camera = render.getScen()->getCamera();
    Ray ray = camera->generateRay(Vec2f(x,y));
    Hit hit(FLT_MAX,NULL,Vec3f());

    if(render.getGridVisual())
    {
        Grid* grid = render.getGrid();
        grid->intersect(ray,hit,camera->getTMin());
    }
    else
    {
        int bounces = render.getBounces();
        bool shadeBack = render.getShadeBack();
        bool shadow = render.getShadow();
        float weight = render.getWeight();
        RayTracer rayTracer(render.getScen(),bounces,weight,shadow,shadeBack);
        rayTracer.traceRay(ray,camera->getTMin(),0,1,1,hit);
    }

}
