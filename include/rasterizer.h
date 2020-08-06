#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__
#include <Eigen/Dense>
#include "obj_reader.h"
#include "tgaimage.h"
#include <string>
class rasterizer {
protected:
    // file to be rendered
    std::string obj_file;
    int resolutionX, resolutionY;
    // save directory
    std::string dir;
    // rotation, translation
    Eigen::Vector3d r, t;
public:
    rasterizer(std::string objective, Eigen::Vector3d rotation, Eigen::Vector3d translation, Eigen::Vector2i resolution, std::string save_dir);
    void rasterize(bool perspective, bool anti_aliasing, bool pervertex, bool bg_white);
};
#endif