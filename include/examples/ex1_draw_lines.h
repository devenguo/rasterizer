#ifndef __EX1_DRAW_LINES_H__
#define __EX1_DRAW_LINES_H__
#include <Eigen/Dense>
#include <iostream>
#include <string>
#include <stdio.h>
#include "line.h"
#include "tgaimage.h"
#include "obj_reader.h"
class draw_lines {
public:
    draw_lines(int resX, int resY);
    int resolutionX, resolutionY;
    void draw();
};
#endif