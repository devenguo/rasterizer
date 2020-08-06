#ifndef __SHADER_H__
#define __SHADER_H__
#include <Eigen/Dense>
#include "obj_reader.h"
#include "tgaimage.h"
#include <string>
class shader {
protected:
    // file to be rendered
    std::string obj_file;
    int resolutionX, resolutionY;
    // save directory
    std::string dir;
    // rotation, translation
    Eigen::Vector3d r, t;
    // light position -1: bottom-left, 1: top-right
    int l;
    // light intensity
    double I;
    // light color
    TGAColor kd;
public:
    shader(std::string objective,
           Eigen::Vector3d rotation,
           Eigen::Vector3d translation,
           Eigen::Vector2i resolution,
           double intensity,
           TGAColor color,
           std::string save_dir,
           int light);
    void shading(bool perspective, bool anti_aliasing, bool pervertex, bool bg_white);
};
#endif