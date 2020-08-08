/************************************************************
Phong + Blinn Phong

a completed shading pipeline / graphics pipeline
a software-level shading pipeline

I think the difference between the shading pipeline and the
previous triangle shader is mainly about the structure

1. previous: triangle as a basic shading unit;
2. shading pipeline: pixel (fragment) as a basic shading unit
(which means there is no triangle() function here, we need to
calculate the per-pixel normal here for better visualization
(phong shading: per-pixel shading));
Phong shading != Blinn-Phong shading
    Phong shading: shading frequency, per-pixel normal instead of per-vertex normal
    Blinn-Phong shading: shading style, ambient + diffuse + specular
Here in our application we use phong + blinn-phong shading here.

Application:
(Thank to the on-line CG course provided by Lingqi Yan from UCSB.)
1. vertex processing
    vertex stream
2. triangle processing
    triangle stream
3. rasterization
    fragment stream
4. fragment processing
    shaded fragments
5. framebuffer operations
    display
*************************************************************/

#ifndef __SHADER_H__
#define __SHADER_H__
#include "obj_reader.h"
#include "tgaimage.h"
#include "transform.h"
#include "triangle.h"
#include <string>
#include <Eigen/Dense>
#include <iostream>
// illumination with light
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
    void shading(bool anti_aliasing);
};
#endif