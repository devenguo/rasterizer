#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include <Eigen/Dense>
#include "tgaimage.h"
Eigen::Vector3d barycentric(Eigen::Matrix3i vertices, Eigen::Vector3i point);
void triangle(Eigen::Matrix3i vertices, TGAImage &image, TGAColor color);
#endif