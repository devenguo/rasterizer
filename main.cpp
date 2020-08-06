#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <stdio.h>
#include <sstream>
#include "rasterizer.h"
#include "triangle.h"
#include "shader.h"
#include "obj_reader.h"
#define resX 1000
#define resY 1000
int main(int argc, char** argv) {
    TGAColor color(255,255,255,255);
    shader s("../data/bunny.obj",
            Eigen::Vector3d(-25,45,0),   //rotation
            Eigen::Vector3d::Zero(),     //translation
            Eigen::Vector2i(resX, resY),
            3000,
            color,
            "../output/shader.tga",
            1);
    s.shading(true,true,false,false);
    return 0;
}