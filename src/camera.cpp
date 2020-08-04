#include "camera.h"

camera::camera(Eigen::Vector3d e,
               Eigen::Vector3d g,
               Eigen::Vector3d t) {
    // M:
    // translate e to origin
    Eigen::Matrix4d T, R; // M = R * T
    T << 1, 0, 0, -e(0),
         0, 1, 0, -e(1),
         0, 0, 1, -e(2),
         0, 0, 0, 1;
    // rotate (g x t) to x
    // rotate t to y
    // rotate g to -z
    // first step: R_inv =
    //      x to (g x t)
    //      y to t
    //      -z to g
    //      which is easy to compute
    // R_inv = R^T =
    //      x_(g x t)   x_t     x_(-g)      0
    //      y_(g x t)   y_t     y_(-g)      0
    //      z_(g x t)   z_t     z_(-g)      0
    Eigen::Vector3d gxt = g.cross(t);
    R << gxt(0),    gxt(1),     gxt(2),     0,
         t(0),      t(1),       t(2),       0,
         -g(0),     -g(1),      -g(2),      0,
         0,         0,          0,          1;
    M = R * T;
}