#include "examples/ex4_shader.h"
#include "tgaimage.h"
#include "triangle_shader.h"
#include <Eigen/Dense>
#define resX 1000
#define resY 1000
void triangle_shader_example() {
    TGAColor color(255,255,255,255);
    triangle_shader s("../data/bunny.obj",
                      Eigen::Vector3d::Zero(),   //rotation
                      Eigen::Vector3d::Zero(),   //translation
                      Eigen::Vector2i(resX, resY),
                      3000,
                      color,
                      "../output/shader.tga",
                      -1);
    s.shading(true,true,true,false);
}