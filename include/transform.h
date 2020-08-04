#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__
#include <Eigen/Dense>
class transform {
public:
    transform(Eigen::Vector3d rotate, Eigen::Vector3d translate);
    transform(Eigen::Vector3d rotate, Eigen::Vector3d translate, double scale);
    Eigen::Matrix4d R_x, R_y, R_z, T, rotation, translation, transformation;
};
#endif