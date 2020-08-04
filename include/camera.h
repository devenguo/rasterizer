#ifndef __CAMERA_H__
#define __CAMERA_H__
#include <Eigen/Dense>
class camera{
public:
    camera(Eigen::Vector3d e,
           Eigen::Vector3d g,
           Eigen::Vector3d t);
    Eigen::Matrix4d M;
    // M: (also known as M_view)
    // translate e to origin
    // rotate g to -z = (0, 0, -1)
    // rotate t to y = (0, 1, 0)
    // rotate (g x t) to x = (1, 0, 0)

    // e: position
    // g: look-at / gaze direction
    // t: up direction
    // transform the camera to (0., 0., 0.)
};
#endif