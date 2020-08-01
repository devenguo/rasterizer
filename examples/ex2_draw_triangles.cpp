#include "examples/ex2_draw_triangles.h"
#include "line.h"
draw_triangles::draw_triangles(int resX, int resY): resolutionX(resX), resolutionY(resY){}
void draw_triangles::draw() {
    TGAImage frame(resolutionX, resolutionY, TGAImage::RGB); 
    Eigen::Matrix3i tri;
    tri << 10,  10, 20,
           100, 30, 70,
           190,160,129;
    triangle(tri, frame, TGAColor(255, 0, 0));
    line(Eigen::Vector2i(10,10),Eigen::Vector2i(100,30),frame,TGAColor(0, 0, 255));
    line(Eigen::Vector2i(10,10),Eigen::Vector2i(190,160),frame,TGAColor(0, 0, 255));
    line(Eigen::Vector2i(100,30),Eigen::Vector2i(190,160),frame,TGAColor(0, 0, 255));
    frame.flip_vertically();
    frame.write_tga_file("../output/ex2_draw_triangles.tga");
}