#include "examples/ex2_draw_triangles.h"
#include "line.h"
#include "obj_reader.h"
#include <iostream>
draw_triangles::draw_triangles(int resX, int resY): resolutionX(resX), resolutionY(resY){}
void draw_triangles::draw() {
    
    // // first try: draw a triangle
    // TGAImage frame(resolutionX, resolutionY, TGAImage::RGB); 
    // Eigen::Matrix3i tri;
    // tri << 10,  10, 20,
    //        100, 30, 70,
    //        190,160,129;
    // triangle(tri, frame, TGAColor(255, 0, 0));
    // line(Eigen::Vector2i(10,10),Eigen::Vector2i(100,30),frame,TGAColor(0, 0, 255));
    // line(Eigen::Vector2i(10,10),Eigen::Vector2i(190,160),frame,TGAColor(0, 0, 255));
    // line(Eigen::Vector2i(100,30),Eigen::Vector2i(190,160),frame,TGAColor(0, 0, 255));
    // frame.flip_vertically();
    // frame.write_tga_file("../output/ex2_draw_triangles.tga");
    
    // // second try: draw a bunny with random triangle color
    objReader obj("../data/bunny.obj");
    obj.fit_screen(0.75, resolutionX, resolutionY);
    int marginX, marginY;
    marginX = (resolutionX - obj.bbox_size[0]) * 0.5;
    marginY = (resolutionY - obj.bbox_size[1]) * 0.5;

    TGAImage frame(resolutionX, resolutionY, TGAImage::RGB);
    for(int i=0; i<obj.nFaces; ++i) {
        // Face# i
        Eigen::Matrix3d screen_coords(3,3);
        for(int j=0; j<3; ++j) {
            // For each vertex in that face
            Eigen::Vector3d world_coords = obj.Vertex.row(obj.Face(i, j));
            screen_coords(j,0) = world_coords(0) + marginX;
            screen_coords(j,1) = world_coords(1) + marginY;
            screen_coords(j,2) = 1;
        }
        triangle(screen_coords, frame, TGAColor(rand()%255, rand()%255, rand()%255, 255));
    }
    frame.flip_vertically();
    frame.write_tga_file("../output/ex2_draw_triangles_wo_zbuffer.tga");
}

void draw_triangles::draw(Eigen::Vector3d light_dir) {
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
            // orthographic projection
            // screen_coords(j,0) = world_coords(0) + marginX;
            // screen_coords(j,1) = world_coords(1) + marginY;
            // screen_coords(j,2) = world_coords(2);
            // perspective projection
            screen_coords(j,0) = world_coords(0)/(1. - z/c) + marginX;
            screen_coords(j,1) = world_coords(1)/(1. - z/c) + marginY;
            screen_coords(j,2) = world_coords(2)/(1. - z/c);
        }

        float intensity = obj.FaceNormal.row(i) * light_dir;
        // triangle(screen_coords, frame,
        //          TGAColor(intensity*255,
        //                   intensity*255,
        //                   intensity*255,
        //                   255),
        //          zbuffer);
        triangle(screen_coords, frame,
                 TGAColor((obj.FaceNormal(i,2)+1.)*.5*255,
                          (obj.FaceNormal(i,1)+1.)*.5*255,
                          (obj.FaceNormal(i,0)+1.)*.5*255,
                          255),
                 zbuffer);
    }
    frame.flip_vertically();
    frame.write_tga_file("../output/ex2_draw_triangles_w_zbuffer.tga");
}