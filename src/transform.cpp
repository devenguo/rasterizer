#include "transform.h"
#define PI 3.1415926
transform::transform(Eigen::Vector3d rotate, Eigen::Vector3d translate) {
    double x = rotate(0), y = rotate(1), z = rotate(2);
    x *= PI/180.; y *= PI/180.; z *= PI/180.;
    R_x << 1, 0, 0, 0,
           0, cos(x), -sin(x), 0,
           0, sin(x), cos(x), 0,
           0, 0, 0, 1;
    R_y << cos(y), 0, sin(y), 0,
           0, 1, 0, 0,
           -sin(y), 0, cos(y), 0,
           0, 0, 0, 1;
    R_z << cos(z), -sin(z), 0, 0,
           sin(z), cos(z), 0, 0,
           0, 0, 1, 0,
           0, 0, 0, 1;
    translation << 0, 0, 0, translate(0),
                   0, 0, 0, translate(1),
                   0, 0, 0, translate(2),
                   0, 0, 0, 0;
    rotation = R_z * R_y * R_x;
    transformation = rotation + translation;
}

transform::transform(Eigen::Vector3d rotate, Eigen::Vector3d translate, double scale){
    double x = rotate(0), y = rotate(1), z = rotate(2);
    R_x << 1, 0, 0, 0,
           0, cos(x), -sin(x), 0,
           0, sin(x), cos(x), 0,
           0, 0, 0, 1;
    R_y << cos(y), 0, sin(y), 0,
           0, 1, 0, 0,
           -sin(y), 0, cos(y), 0,
           0, 0, 0, 1;
    R_z << cos(z), -sin(z), 0, 0,
           sin(z), cos(z), 0, 0,
           0, 0, 1, 0,
           0, 0, 0, 1;
    translation << 1, 0, 0, translate(0),
                   0, 1, 0, translate(1),
                   0, 0, 1, translate(2),
                   0, 0, 0, 1;
    rotation = R_z * R_y * R_x;
    transformation = (rotation + translation) * scale;
}