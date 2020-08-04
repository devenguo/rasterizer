#ifndef __EX2_DRAW_TRIANGLES__
#define __EX2_DRAW_TRIANGLES__
#include "triangle.h"
class draw_triangles {
public:
    draw_triangles(int resX, int resY);
    int resolutionX, resolutionY;
    void draw(bool perspective, bool anti_aliasing);
    void draw(Eigen::Vector3d light_dir, bool perspective, bool anti_aliasing);
};
#endif