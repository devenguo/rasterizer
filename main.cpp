#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <stdio.h>
#include "line.h"
#include "tgaimage.h"
#include "obj_reader.h"
#include "triangle.h"
#include "examples/ex1_draw_lines.h"
#include "examples/ex2_draw_triangles.h"
#define resX 1000
#define resY 1000
int main(int argc, char** argv) {
    // ex1
    draw_lines ex1(resX, resY);
    ex1.draw();
    // ex2
    draw_triangles ex2(resX, resY);
    ex2.draw(Eigen::Vector3d(0.,0.,-1.));
    return 0;
}