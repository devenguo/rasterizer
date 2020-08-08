#include "shader.h"
#include "tgaimage.h"
#include <Eigen/Dense>
#include <sstream>
#include <examples/ex2_draw_triangles.h>
#include <examples/ex3_rasterizer.h>
#define resX 1000
#define resY 1000
int main(int argc, char** argv) {
    draw_triangles d(resX, resY);
    d.draw(false,false);
    d.draw(true,false);
    d.draw(true,true);
    return 0;
    rasterizer_example();
    return 0;
    TGAColor color(255,255,255,255);
    for(int i=0; i<360; i+=10) {
        std::stringstream ss;
        ss << "../output/blinn-phong/";
        ss << i;
        ss << ".tga";
        std::string save_dir;
        ss >> save_dir;
        shader s(
            "../data/bunny.obj",
            Eigen::Vector3d(0,i,0),
            Eigen::Vector3d::Zero(),
            Eigen::Vector2i(resX, resY),
            5e6,
            color,
            save_dir,
            1
        );
        s.shading(false);
    }
    // cd to "../output/blinn-phong/", run the following script to make your own gif
    // ffmpeg -f image2 -i %d0.tga bunny.gif
    // ffmpeg -f image2 -i ../output/blinn-phong/%d0.tga ../output/blinn-phong/blinn-phong.gif
    return 0;
}