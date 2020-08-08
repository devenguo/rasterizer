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

// per-vertex shading / Gouraud Shading
void triangle(Eigen::Matrix3d vertices, TGAImage &image, TGAColor* vertex_color, float* zbuffer) {
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
            TGAColor color(0,0,0,255);
            for(int i=0; i<3; ++i)
                for(int j=0; j<3; ++j)
                    color[i] += P_bc(j) * vertex_color[j][i];
            
            // color[0] = P_bc(0)* vertex_color[0][0] + P_bc(1)* vertex_color[1][0] + P_bc(2)* vertex_color[2][0];
            // color[1] = P_bc(0)* vertex_color[0][1] + P_bc(1)* vertex_color[1][1] + P_bc(2)* vertex_color[2][1];
            // color[2] = P_bc(0)* vertex_color[0][2] + P_bc(1)* vertex_color[1][2] + P_bc(2)* vertex_color[2][2];
            if (zbuffer[x+y*image.get_width()] < _Pz) {
                zbuffer[x+y*image.get_width()] = _Pz;
                image.set(x, y, color);
            }
            // image.set(x, y, color);
            // std::cout << x << " " << y << std::endl;
        }
    }
}

// per-pixel shading / Phong shading
void triangle(Eigen::Matrix3d world_coords,
              TGAImage &image,
              Eigen::Matrix3d vertices_normal,
              int light,
              Eigen::Vector3d light_pos,
              double I,
              TGAColor kd,
              float* zbuffer,
              double c,
              int marginX,
              int marginY,
              std::vector< std::vector< std::vector<double> > >& buf_msaa) {

    Eigen::Matrix3d screen_coords;
    for(int i=0; i<3; ++i) {
        double z = world_coords(i, 2);
        screen_coords(i, 0) = world_coords(i, 0)/(1. - z/c) + marginX;
        screen_coords(i, 1) = world_coords(i, 1)/(1. - z/c) + marginY;
        screen_coords(i, 2) = world_coords(i, 2)/(1. - z/c);
        // image.set(screen_coords(i, 0), screen_coords(i, 1), TGAColor(
        //     (vertices_normal(i,2)+1.)*.5*255,
        //     (vertices_normal(i,1)+1.)*.5*255,
        //     (vertices_normal(i,0)+1.)*.5*255,
        //     255
        // ));
    }
    // return;
    
    Eigen::Vector3d bbox_min = screen_coords.colwise().minCoeff();
    Eigen::Vector3d bbox_max = screen_coords.colwise().maxCoeff();
    
    double x_min = fmax(bbox_min(0),0), x_max = fmin(bbox_max(0),image.get_width()-1),
           y_min = fmax(bbox_min(1),0), y_max = fmin(bbox_max(1),image.get_height()-1);

    for(int x=x_min; x<=x_max; ++x) {
        for(int y=y_min; y<=y_max; ++y) {
            Eigen::Vector3d P_bc = barycentric(screen_coords, Eigen::Vector3d(x, y, 1.));
            if(P_bc(0)<0. || P_bc(1)<0. || P_bc(2)<0.) continue;
            
            double world_x, world_y, world_z;
            world_z = P_bc(0)*screen_coords(0,2) + P_bc(1)*screen_coords(1,2) + P_bc(2)*screen_coords(2,2);
            world_x = (x - marginX) * (1. - world_z/c);
            world_y = (y - marginY) * (1. - world_z/c);
            // Eigen::Vector3d world_P_bc = barycentric(world_coords, Eigen::Vector3d(world_x, world_y, 1.));
            Eigen::Vector3d vertex_world_coord(world_x, world_y, world_z);

            double _Pz = 0.;
            for (int i=0; i<3; i++)
                _Pz += screen_coords(i,2) * P_bc(i);
            
            // std::cout << P_bc << std::endl << world_P_bc << std::endl << std::endl;

            Eigen::Vector3d vertex_normal = Eigen::Vector3d::Zero();
            for(int i=0; i<3; ++i)
                // vertex_normal += world_P_bc(i) * vertices_normal.row(i);
                vertex_normal += P_bc(i) * vertices_normal.row(i);
            vertex_normal.normalize();
            // std::cout << vertex_normal.norm() << std::endl;

            // if (zbuffer[x+y*image.get_width()] < _Pz) {
            //     zbuffer[x+y*image.get_width()] = _Pz;
            //     image.set(x, y,
            //               TGAColor((vertex_normal(2)+1.)*.5*255,
            //                        (vertex_normal(1)+1.)*.5*255,
            //                        (vertex_normal(0)+1.)*.5*255,
            //                        255));
            // }


            // Lambertian (Diffuse) Shading
            // Ld = kd(I/r^2)max(0,n·l)
            Eigen::Vector3d l = (vertex_world_coord - light_pos).normalized();
            double r_square = (light_pos - vertex_world_coord).squaredNorm();
            // std::cout << r_square << std::endl;
            double m1 = fmax(0., vertex_normal.dot(l));
            double Ld_over_kd = I / r_square * m1;
            
            // Ls = ks(I/r^2)max(0,n·h)
            TGAColor ks(255,255,255,255);
            Eigen::Vector3d v(0., 0., -c); v.normalize();
            Eigen::Vector3d h = (l+v)/(l+v).norm();
            // std::cout << h << std::endl;
            double m2 = fmax(0., vertex_normal.dot(h));
            // std::cout << m2 << std::endl;
            double Ls_over_ks = I / r_square * pow(m2, 8);
            // std::cout << m1 << " " << m2 << std::endl;
            // std::cout << Ld_over_kd << " " << Ls_over_ks << std::endl;
            if (zbuffer[x+y*image.get_width()] < _Pz) {
                zbuffer[x+y*image.get_width()] = _Pz;
                image.set(x, y,
                          TGAColor(kd[2] * Ld_over_kd + ks[2] * Ls_over_ks,
                                   kd[1] * Ld_over_kd + ks[1] * Ls_over_ks,
                                   kd[0] * Ld_over_kd + ks[0] * Ls_over_ks,
                                   255));
                for(int i=0; i<3; ++i) {
                    buf_msaa[x][y][i] = double(kd[i]) * Ld_over_kd + double(ks[i]) * Ls_over_ks + 50.;
                    // std::cout << buf_msaa[x][y][i] << std::endl;
                }
            }
        }
    }
}