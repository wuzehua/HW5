//
// Created by RainZhong on 2019/2/12.
//

#ifndef HW2_RAYTRACER_H
#define HW2_RAYTRACER_H

#include <iostream>
#include <cstring>
#include <cfloat>
#include "scene_parser.h"
#include "camera.h"
#include "group.h"
#include "image.h"
#include "hit.h"
#include "light.h"
#include "sphere.h"
#include "raytracer.h"

class ImageRender
{

public:

    ImageRender();
    ~ImageRender();

    void parseCommand(int argc,char** argv);
    void renderImage();
    SceneParser* getScen(){ return parser;}
    bool getGUI(){ return gui;}
    bool getShadeBack(){ return shadeBack;}
    bool getShadow(){ return shadow;}
    int getBounces(){ return bounces;}
    float getWeight(){ return weight;}


private:

    void render();
    void renderDepth();
    void renderNormals();


    char* inputFile;
    char* outputFile;
    char* outputFile_depth;
    char* outputFile_normal;

    int width;
    int height;
    int bounces;

    float depth_min;
    float depth_max;
    float weight;

    bool shadeBack;
    bool gui;
    bool shadow;

    Image* image;
    SceneParser* parser;
};


#endif //HW2_RAYTRACER_H
