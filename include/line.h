#ifndef __LINE_H__
#define __LINE_H__
#include <Eigen/Dense>
#include "tgaimage.h"
// Copy from the tiny renderer
// https://github.com/ssloy/tinyrenderer/wiki/Lesson-1-Bresenham’s-Line-Drawing-Algorithm
void line(Eigen::Vector2i p0, Eigen::Vector2i p1, TGAImage &image, TGAColor color);
#endif