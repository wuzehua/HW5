//
// Created by RainZhong on 2019/2/12.
//

#include "../Include/imagerender.h"

ImageRender::ImageRender()
{
    inputFile = NULL;
    outputFile = NULL;
    outputFile_depth = NULL;
    outputFile_normal = NULL;

    width = 100;
    height = 100;
    bounces = 0;

    depth_min = 0;
    depth_max = 0;
    weight = 1;

    shadeBack = false;
    gui = false;
    shadow = false;
    showGrid = false;

    image = NULL;
    parser = NULL;
}


ImageRender::~ImageRender()
{
    if(image != NULL)
    {
        delete image;
    }

    if(parser != NULL)
    {
        delete parser;
    }

    if(grid != NULL)
    {
        delete grid;
    }
}



//解析附加参数
void ImageRender::parseCommand(int argc, char **argv)
{
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc);
            inputFile = argv[i];
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc);
            width = atoi(argv[i]);
            i++; assert (i < argc);
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc);
            outputFile = argv[i];
        } else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc);
            depth_min = atof(argv[i]);
            i++; assert (i < argc);
            depth_max = atof(argv[i]);
            i++; assert (i < argc);
            outputFile_depth = argv[i];
        }else if(!strcmp(argv[i],"-normals")){
            i++;assert(i < argc);
            outputFile_normal = argv[i];
        } else if(!strcmp(argv[i],"-shade_back"))
        {
            shadeBack = true;
        }else if(!strcmp(argv[i],"-gui"))
        {
            gui = true;
        } else if(!strcmp(argv[i],"-tessellation"))
        {
            i++;assert(i < argc);
            int th = atoi(argv[i]);
            i++;assert(i < argc);
            int ph = atoi(argv[i]);
            Sphere::setStep(th,ph);
        }else if(!strcmp(argv[i],"-gouraud"))
        {
            Sphere::setGouraud(true);
        }
        else if(!strcmp(argv[i],"-shadows"))
        {
            shadow = true;
        }
        else if(!strcmp(argv[i],"-bounces"))
        {
            i++;assert(i < argc);
            bounces = atoi(argv[i]);
        }
        else if(!strcmp(argv[i],"-weight"))
        {
            i++;assert(i < argc);
            weight = atof(argv[i]);
        }
        else if(!strcmp(argv[i],"-grid"))
        {
            i++;assert(i < argc);
            nx = atoi(argv[i]);
            i++;assert(i < argc);
            ny = atoi(argv[i]);
            i++;assert(i < argc);
            nz = atoi(argv[i]);
        }
        else if(!strcmp(argv[i],"-visualize_grid"))
        {
            showGrid = true;
        }
        else {
            printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }

    assert(width > 0);
    assert(height > 0);
    assert(inputFile != NULL);

    image = new Image(width,height);
    parser = new SceneParser(inputFile);
    grid = new Grid(parser->getGroup()->getBoundingBox(),nx,ny,nz);
    grid->setNumOfColor(parser->getGroup()->getNumOfObj());
    parser->getGroup()->insertIntoGrid(grid,NULL);


}


//根据用户要求来渲染图片
void ImageRender::renderImage()
{
    if(outputFile != NULL)
    {
        if(!showGrid)
        {
            render();
        }
        else
        {
            renderGrid();
        }
    }

    if(outputFile_depth != NULL)
    {
        renderDepth();
    }

    if(outputFile_normal != NULL)
    {
        renderNormals();
    }
}



//普通渲染
void ImageRender::render()
{
    Vec2f point;
    int x,y;
    float x_float,y_float;
    RayTracer rayTracer(parser,bounces,weight,shadow,shadeBack);
    Camera* camera = parser->getCamera();

    for(x = 0; x < width;x++)
    {
        for(y = 0; y < height;y++)
        {
            Hit h(FLT_MAX,NULL,Vec3f());
            x_float = (float)x / width;
            y_float = (float)y / height;
            point.Set(x_float,y_float);
            Ray ray = camera->generateRay(point);
            Vec3f color = rayTracer.traceRay(ray,camera->getTMin(),0,1,1,h);
            image->SetPixel(x,y,color);
        }
    }



    assert(outputFile != NULL);
    image->SaveTGA(outputFile);
}



//深度渲染
void ImageRender::renderDepth()
{
    float range;
    Vec3f background;
    Vec3f color;
    Camera* camera;
    Group* group;
    Vec2f point;
    int x,y;
    float x_float,y_float;
    bool hit;
    float t;
    float grey;
    camera = parser->getCamera();
    group = parser->getGroup();
    background.Set(0,0,0);
    image->SetAllPixels(background);

    if(depth_max < depth_min)
    {
        float temp = depth_min;
        depth_min = depth_max;
        depth_max = temp;
    }

    range = depth_max - depth_min;
    if(!(range > 0))
    {
        range = 1;
    }

    for(x = 0; x < width;x++)
    {
        for(y = 0; y < height;y++)
        {
            Hit h(FLT_MAX,NULL,Vec3f());
            x_float = (float)x / width;
            y_float = (float)y / height;
            point.Set(x_float,y_float);
            Ray ray = camera->generateRay(point);
            hit = group->intersect(ray,h,camera->getTMin());
            if(hit)
            {
                t = h.getT();
                if(t < depth_min)
                {
                    t = depth_min;
                }
                if(t > depth_max)
                {
                    t = depth_max;
                }
                grey = (depth_max - t) / range;
                color.Set(grey,grey,grey);
                image->SetPixel(x,y,color);

            }
        }
    }


    assert(outputFile_depth != NULL);
    image->SaveTGA(outputFile_depth);
}


//单位法向量作rgb值进行渲染
void ImageRender::renderNormals()
{
    Vec3f black(0,0,0);
    Vec3f color;
    Vec2f point;
    Camera* camera;
    Group* group;
    bool hit;
    int x,y;

    camera = parser->getCamera();
    group = parser->getGroup();
    image->SetAllPixels(black);

    for(x = 0;x < width;x++)
    {
        for(y = 0;y < height;y++)
        {
            point.Set((float) x / width,(float)y / height);
            Ray r = camera->generateRay(point);
            Hit h(FLT_MAX,NULL,Vec3f());
            hit = group->intersect(r,h,camera->getTMin());

            if(hit)
            {
                Vec3f normal = h.getNormal();
                float r = fabsf(normal.r());
                float g = fabsf(normal.g());
                float b = fabsf(normal.b());
                Vec3f color(r,g,b);
                image->SetPixel(x,y,color);
            }
        }
    }


    assert(outputFile_normal != NULL);
    image->SaveTGA(outputFile_normal);

}

void ImageRender::renderGrid()
{
    Vec2f point;
    int x,y;
    int numOfLight;
    int i;
    float x_float,y_float;
    Camera* camera = parser->getCamera();
    numOfLight = parser->getNumLights();
    Light* lights[numOfLight];
    Vec3f background = parser->getBackgroundColor();
    Vec3f color;
    Vec3f ambient;
    Vec3f clight,L;
    Material* material;
    float distance;


    ambient = parser->getAmbientLight();

    for(i = 0;i < numOfLight;i++)
    {
        lights[i] = parser->getLight(i);
    }



    for(x = 0;x < width;x++)
    {
        for(y = 0;y < height;y++)
        {
            Hit h(FLT_MAX,NULL,Vec3f());
            x_float = (float)x / width;
            y_float = (float)y / height;
            point.Set(x_float,y_float);
            Ray ray = camera->generateRay(point);

            if(grid->intersect(ray,h,camera->getTMin()))
            {
                material = h.getMaterial();
                color = ambient * material->getDiffuseColor();

                for(i = 0;i < numOfLight;i++)
                {
                    lights[i]->getIllumination(h.getIntersectionPoint(),L,clight,distance);
                    color += material->Shade(ray,h,L,clight);
                }

                image->SetPixel(x,y,color);

            }
            else
            {
                image->SetPixel(x,y,background);
            }

        }
    }

    assert(outputFile != NULL);
    image->SaveTGA(outputFile);

}