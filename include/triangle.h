#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__
#include <Eigen/Dense>
#include <vector>
#include "tgaimage.h"
#include "obj_reader.h"
Eigen::Vector3d barycentric(Eigen::Matrix3d vertices, Eigen::Vector3i point);
void triangle(Eigen::Matrix3d vertices, TGAImage &image, TGAColor color);
void triangle(Eigen::Matrix3d vertices, TGAImage &image, TGAColor color, float* zbuffer);
void triangle(Eigen::Matrix3d vertices, TGAImage &image, TGAColor* vertex_color, float* zbuffer);
void triangle(Eigen::Matrix3d world_coords,
              TGAImage &image,
              Eigen::Matrix3d vertices_normal,
              int light,
              Eigen::Vector3d light_dir,
              double I,
              TGAColor kd,
              float* zbuffer,
              double c,
              int marginX,
              int marginY,
              std::vector< std::vector< std::vector<double> > >& buf_msaa);
#endif