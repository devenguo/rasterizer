#include "triangle_shader.h"
#include "transform.h"
#include "triangle.h"
#include <string>
#include <Eigen/Dense>
#include <iostream>
triangle_shader::triangle_shader(std::string objective, Eigen::Vector3d rotation, Eigen::Vector3d translation, Eigen::Vector2i resolution, double intensity, TGAColor color, std::string save_dir, int light) {
    obj_file = objective;
    resolutionX = resolution(0);
    resolutionY = resolution(1);
    dir = save_dir;
    r = rotation;
    t = translation;
    I = intensity;
    kd = color;
    l = light;
}
void triangle_shader::shading(bool perspective, bool anti_aliasing, bool pervertex, bool bg_white) {
    if(anti_aliasing) {
        resolutionX*=2;
        resolutionY*=2;
    }
    objReader obj(obj_file.c_str());
    // display in 60% of the screen size
    obj.fit_screen(0.6, resolutionX, resolutionY);
    Eigen::Vector3d light_dir = 3 * l * (obj.bbox_min + obj.bbox_size);
    int marginX, marginY;
    marginX = (resolutionX - obj.bbox_size[0]) * 0.5;
    marginY = (resolutionY - obj.bbox_size[1]) * 0.5;

    TGAImage frame(resolutionX, resolutionY, TGAImage::RGB);
    float *zbuffer = new float[resolutionX*resolutionY];
    for(int i=resolutionX*resolutionY; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    
    double c = 20*obj.bbox_size(2);

    transform T(r, t);
    obj.transform(T.transformation);

    int n = 0;
    for(int i=0; i<obj.nFaces; ++i) {
        // Face# i
        Eigen::Matrix3d screen_coords(3,3);
        Eigen::Vector3d world_coords;
        for(int j=0; j<3; ++j) {
            world_coords = obj.Vertex.row(obj.Face(i, j));
            // For each vertex in that face
            double z = world_coords(2);
            if(perspective) {
                // formula from tinyrenderer
                screen_coords(j,0) = world_coords(0)/(1. - z/c) + marginX;
                screen_coords(j,1) = world_coords(1)/(1. - z/c) + marginY;
                screen_coords(j,2) = world_coords(2)/(1. - z/c);
            } else {
                screen_coords(j,0) = world_coords(0) + marginX;
                screen_coords(j,1) = world_coords(1) + marginY;
                screen_coords(j,2) = world_coords(2);
            }
        }
        // Lambertian (Diffuse) Shading
        // Ld = kd(I/r^2)max(0,n·l)
        if(pervertex) {
            TGAColor vertices_color[3];
            for(int j=0; j<3; ++j) {
                double r_square = (light_dir - world_coords).squaredNorm();
                light_dir.normalized();
                double m = fmax(0., obj.VertexNormal.row(obj.Face(i,j)).dot(light_dir));
                double L_over_k = I / r_square * m;
                vertices_color[j] = TGAColor(kd[0] * L_over_k,
                                             kd[1] * L_over_k,
                                             kd[2] * L_over_k,
                                             255);
            }
            triangle(screen_coords, frame, vertices_color, zbuffer);
        } else {
            double r_square = (light_dir - world_coords).squaredNorm();
            light_dir.normalized();
            double m = fmax(0., obj.FaceNormal.row(i).dot(light_dir));
            double L_over_k = I / r_square * m;
            triangle(screen_coords, frame,
                     TGAColor(kd[0] * L_over_k,
                              kd[1] * L_over_k,
                              kd[2] * L_over_k,
                              255),
                     zbuffer);
        }
    }
    frame.flip_vertically();

    // white background
    if(bg_white) {
        for(int i=0; i<frame.get_height(); ++i) {
            for(int j=0; j<frame.get_width(); ++j) {
                if(frame.get(i,j).bgra[0]==0
                   && frame.get(i,j).bgra[1]==0
                   && frame.get(i,j).bgra[2]==0) {
                        TGAColor color;
                        for(int k=0; k<4; ++k)
                            color.bgra[k] = 255;
                        frame.set(i, j, color);
                }
            }
        }
    }

    TGAImage frame_aa(resolutionX/2, resolutionY/2, TGAImage::RGB);
    if(anti_aliasing) {
        for(int i=0; i<resolutionX/2; ++i) {
            for(int j=0; j<resolutionY/2; ++j) {
                TGAColor color;
                for(int k=0; k<4; ++k)
                    color.bgra[k] = (frame.get(i*2, j*2).bgra[k]
                                    +frame.get(i*2, j*2+1).bgra[k]
                                    +frame.get(i*2+1, j*2).bgra[k]
                                    +frame.get(i*2+1, j*2+1).bgra[k])*.25;
                frame_aa.set(i,j,color);
            }
        }
    }
    if(anti_aliasing)
        frame_aa.write_tga_file(dir.c_str());
    else
        frame.write_tga_file(dir.c_str());
}
