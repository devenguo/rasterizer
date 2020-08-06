#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <stdio.h>
#include "rasterizer.h"
#include "examples/ex3_rasterizer.h"
#define resX 500
#define resY 500
void rasterizer_example() {
    rasterizer r("../data/bunny.obj",
                Eigen::Vector3d(-25,45,0),   //rotation
                Eigen::Vector3d::Zero(),     //translation
                Eigen::Vector2i(resX, resY),
                "../output/rasterizer.tga");
    r.rasterize(true, true, true, true);

    for(int i=0; i<720; i+=10) {
        std::stringstream ss;
        ss << "../output/rasterizer_white/";
        ss << i;
        ss << ".tga";
        std::string save_dir;
        ss >> save_dir;
        rasterizer r("../data/bunny.obj",
                     Eigen::Vector3d(0,i,0),   //rotation
                     Eigen::Vector3d::Zero(),    //translation
                     Eigen::Vector2i(resX, resY),
                     save_dir);
        if(i<=360) r.rasterize(true, true, false, true);
        else r.rasterize(true, true, true, true);
        // cd to "../output/rasterizer/", run the following script to make your own gif
        // ffmpeg -f image2 -i %d0.tga bunny.gif
        // ffmpeg -f image2 -i ../output/rasterizer_white/%d0.tga ../output/rasterizer_white/bunny.gif
    }
}