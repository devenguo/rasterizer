#include "triangle.h"
#include <iostream>
#include <vector>
Eigen::Vector3d barycentric(Eigen::Matrix3d vertices,
                            Eigen::Vector3d point) {
    Eigen::Vector3d AB = vertices.row(1) - vertices.row(0);
    Eigen::Vector3d AC = vertices.row(2) - vertices.row(0);
    Eigen::Vector3d PA = vertices.row(0) - point.transpose();
    // Vx = (ABx, ACx, PAx)
    // Vy = (ABy, ACy, PAy)
    // (u, v, 1)Vx^T = 0
    // (u, v, 1)Vy^T = 0
    // (!!!) CAUTION: Vector3d (√) Vector3i (×)
    Eigen::Vector3d Vx(AB(0), AC(0), PA(0));
    Eigen::Vector3d Vy(AB(1), AC(1), PA(1));
    Eigen::Vector3d Vx_cross_Vy = Vx.cross(Vy);
    if (std::abs(Vx_cross_Vy[2]) < 1)
        // parallel to the screen, thus the (u,v,1) should be vertical to the screen
        return Eigen::Vector3d(-1., 1., 1.);
    // return (1-u-v, u, v)
    float u = Vx_cross_Vy(0) / Vx_cross_Vy(2);
    float v = Vx_cross_Vy(1) / Vx_cross_Vy(2);
    return Eigen::Vector3d(1.-u-v, u, v);
}

void triangle(Eigen::Matrix3d vertices, TGAImage &image, TGAColor color) {
    Eigen::Vector3d bbox_min = vertices.colwise().minCoeff();
    Eigen::Vector3d bbox_max = vertices.colwise().maxCoeff();
    // std::cout << bbox_min << "\n" << bbox_max << std::endl;
    // clip to the screen boundary
    double x_min = fmax(bbox_min(0),0), x_max = fmin(bbox_max(0),image.get_width()-1),
           y_min = fmax(bbox_min(1),0), y_max = fmin(bbox_max(1),image.get_height()-1);
    for(int x=x_min; x<x_max; ++x) {
        for(int y=y_min; y<y_max; ++y) {
            Eigen::Vector3d P_bc = barycentric(vertices, Eigen::Vector3d(x, y, 1.));
            if(P_bc(0)<0. || P_bc(1)<0. || P_bc(2)<0.) continue;
            image.set(x, y, color);
            // std::cout << x << " " << y << std::endl;
        }
    }
}

void triangle(Eigen::Matrix3d vertices, TGAImage &image, TGAColor color, float* zbuffer) {
    Eigen::Vector3d bbox_min = vertices.colwise().minCoeff();
    Eigen::Vector3d bbox_max = vertices.colwise().maxCoeff();
    // std::cout << bbox_min << "\n" << bbox_max << std::endl;
    // clip to the screen boundary
    double x_min = fmax(bbox_min(0),0), x_max = fmin(bbox_max(0),image.get_width()-1),
           y_min = fmax(bbox_min(1),0), y_max = fmin(bbox_max(1),image.get_height()-1);
    // x, y have to be integer
    // or double x = int(x_min); x <= int(x_max); ++x
    for(int x=x_min; x<=x_max; ++x) {
        for(int y=y_min; y<=y_max; ++y) {
            Eigen::Vector3d P_bc = barycentric(vertices, Eigen::Vector3d(x, y, 1.));
            if(P_bc(0)<0. || P_bc(1)<0. || P_bc(2)<0.) continue;
            // std::cout << P_bc(0) << " " << P_bc(1) << " " << P_bc(2) << std::endl;
            double _Pz = 0.;
            for (int i=0; i<3; i++) {
                _Pz += vertices(i,2)*P_bc(i);
                // std::cout << vertices.row(i) << std::endl;
                // std::cout << vertices(i,2) << " " << P_bc(i) << " " << _Pz << std::endl;
            }
            if (zbuffer[x+y*image.get_width()] < _Pz) {
                zbuffer[x+y*image.get_width()] = _Pz;
                image.set(x, y, color);
            }
            // image.set(x, y, color);
            // std::cout << x << " " << y << std::endl;
        }
    }
}
