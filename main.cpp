#include <iostream>
#include "Include/imagerender.h"
#include "Include/glCanvas.h"


ImageRender render;

void renderImage()
{
    render.renderImage();
}

void traceRay(float x, float y)
{}

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