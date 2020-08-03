#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include <Eigen/Dense>
#include "tgaimage.h"
#include "obj_reader.h"
Eigen::Vector3d barycentric(Eigen::Matrix3d vertices, Eigen::Vector3i point);
void triangle(Eigen::Matrix3d vertices, TGAImage &image, TGAColor color);
void triangle(Eigen::Matrix3d vertices, TGAImage &image, TGAColor color, float* zbuffer);
// void triangle(Eigen::Matrix3d vertices, TGAImage &image, TGAColor color, float* zbuffer, objReader& obj);
#endif