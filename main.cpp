#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <stdio.h>
#include "rasterizer.h"
#include <string>
#include <sstream>
#define resX 500
#define resY 500
int main(int argc, char** argv) {
    // rasterizer r("../data/bunny.obj",
    //             Eigen::Vector3d(25,45,0),   //rotation
    //             Eigen::Vector3d::Zero(),    //translation
    //             Eigen::Vector2i(resX, resY),
    //             "../output/rasterizer.tga");
    // r.rasterize(true, true);
    for(int i=0; i<360; i+=10) {
        std::stringstream ss;
        ss << "../output/rasterizer/";
        ss << i;
        ss << ".tga";
        std::string save_dir;
        ss >> save_dir;
        rasterizer r("../data/bunny.obj",
                     Eigen::Vector3d(0,i,0),   //rotation
                     Eigen::Vector3d::Zero(),    //translation
                     Eigen::Vector2i(resX, resY),
                     save_dir);
        r.rasterize(true, true);
        // cd to "../output/rasterizer/", run the following script to make your own gif
        // ffmpeg -f image2 -i %d0.tga bunny.gif
    }
    return 0;
}