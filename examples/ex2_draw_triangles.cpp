#include "examples/ex2_draw_triangles.h"
#include "line.h"
#include "obj_reader.h"
#include <iostream>
draw_triangles::draw_triangles(int resX, int resY): resolutionX(resX), resolutionY(resY){}
void draw_triangles::draw(bool perspective) {
    objReader obj("../data/bunny.obj");
    obj.fit_screen(0.6, resolutionX, resolutionY);
    int marginX, marginY;
    marginX = (resolutionX - obj.bbox_size[0]) * 0.5;
    marginY = (resolutionY - obj.bbox_size[1]) * 0.5;

    TGAImage frame(resolutionX, resolutionY, TGAImage::RGB);
    float *zbuffer = new float[resolutionX*resolutionY];
    for(int i=resolutionX*resolutionY; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    for(int i=0; i<obj.nFaces; ++i) {
        double c = 10.0*(obj.bbox_max(2)-obj.bbox_min(2));
        // Face# i
        Eigen::Matrix3d screen_coords(3,3);
        for(int j=0; j<3; ++j) {
            // For each vertex in that face
            Eigen::Vector3d world_coords = obj.Vertex.row(obj.Face(i, j));
            double z = world_coords(2);
            if(perspective) {
                screen_coords(j,0) = world_coords(0)/(1. - z/c) + marginX;
                screen_coords(j,1) = world_coords(1)/(1. - z/c) + marginY;
                screen_coords(j,2) = world_coords(2)/(1. - z/c);
            } else {
                screen_coords(j,0) = world_coords(0) + marginX;
                screen_coords(j,1) = world_coords(1) + marginY;
                screen_coords(j,2) = world_coords(2);
            }
        }
        // triangle(screen_coords, frame, TGAColor(rand()%255, rand()%255, rand()%255, 255));
        triangle(screen_coords, frame,
                 TGAColor((obj.FaceNormal(i,2)+1.)*.5*255,
                          (obj.FaceNormal(i,1)+1.)*.5*255,
                          (obj.FaceNormal(i,0)+1.)*.5*255,
                          255),
                 zbuffer);
    }
    frame.flip_vertically();
    if(perspective)
        frame.write_tga_file("../output/ex2_draw_triangles_normal_perspective.tga");
    else
        frame.write_tga_file("../output/ex2_draw_triangles_normal_orthographic.tga");
}

void draw_triangles::draw(Eigen::Vector3d light_dir, bool perspective) {
    objReader obj("../data/bunny.obj");
    obj.fit_screen(0.6, resolutionX, resolutionY);
    // std::cout << obj.bbox_min << " " << obj.bbox_max << std::endl;
    int marginX, marginY;
    marginX = (resolutionX - obj.bbox_size[0]) * 0.5;
    marginY = (resolutionY - obj.bbox_size[1]) * 0.5;

    TGAImage frame(resolutionX, resolutionY, TGAImage::RGB);
    float *zbuffer = new float[resolutionX*resolutionY];
    for(int i=resolutionX*resolutionY; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    for(int i=0; i<obj.nFaces; ++i) {
        // Face# i
        Eigen::Matrix3d screen_coords(3,3);

        double c = 10.0*(obj.bbox_max(2)-obj.bbox_min(2));
        for(int j=0; j<3; ++j) {
            // For each vertex in that face
            Eigen::Vector3d world_coords = obj.Vertex.row(obj.Face(i, j));
            double z = world_coords(2);
            if(perspective) {
                screen_coords(j,0) = world_coords(0)/(1. - z/c) + marginX;
                screen_coords(j,1) = world_coords(1)/(1. - z/c) + marginY;
                screen_coords(j,2) = world_coords(2)/(1. - z/c);
            } else {
                screen_coords(j,0) = world_coords(0) + marginX;
                screen_coords(j,1) = world_coords(1) + marginY;
                screen_coords(j,2) = world_coords(2);
            }
        }

        float intensity = obj.FaceNormal.row(i) * light_dir;
        triangle(screen_coords, frame,
                 TGAColor(intensity*255,
                          intensity*255,
                          intensity*255,
                          255),
                 zbuffer);
    }
    frame.flip_vertically();
    if (perspective)
        frame.write_tga_file("../output/ex2_draw_triangles_intensity_perspective.tga");
    else
        frame.write_tga_file("../output/ex2_draw_triangles_intensity_orthographic.tga");
}