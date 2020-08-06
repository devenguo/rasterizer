#include "examples/ex2_draw_triangles.h"
#include "line.h"
#include "obj_reader.h"
#include "transform.h"
#include <iostream>
#include <Eigen/Dense>
draw_triangles::draw_triangles(int resX, int resY): resolutionX(resX), resolutionY(resY){}
void draw_triangles::draw(bool perspective, bool anti_aliasing) {
    if(anti_aliasing) {
        // std::cout << "anti-aliasing" <<std::endl;
        resolutionX*=2;
        resolutionY*=2;
    }
    objReader obj("../data/bunny.obj");
    obj.fit_screen(0.6, resolutionX, resolutionY);
    int marginX, marginY;
    marginX = (resolutionX - obj.bbox_size[0]) * 0.5;
    marginY = (resolutionY - obj.bbox_size[1]) * 0.5;

    TGAImage frame(resolutionX, resolutionY, TGAImage::RGB);
    float *zbuffer = new float[resolutionX*resolutionY];
    for(int i=resolutionX*resolutionY; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    
    double c = 20*obj.bbox_size(2);
    // std::cout << c << std::endl << obj.bbox_max << obj.bbox_min << std::endl;

    Eigen::Vector3d r, t;
    r << -25, 45, 0; t << 0, 0, 0;
    transform T(r, t);
    obj.transform(T.transformation);

    for(int i=0; i<obj.nFaces; ++i) {
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
        TGAColor vertices_color[3];
        for(int j=0; j<3; ++j) {
            vertices_color[j] = TGAColor(
                (obj.VertexNormal(obj.Face(i,j),2)+1.)*.5*255,
                (obj.VertexNormal(obj.Face(i,j),1)+1.)*.5*255,
                (obj.VertexNormal(obj.Face(i,j),0)+1.)*.5*255,
                255
            );
        }
        triangle(screen_coords, frame, vertices_color, zbuffer);
        // triangle(screen_coords, frame,
        //          TGAColor((obj.FaceNormal(i,2)+1.)*.5*255, // #2 of i-th facenormal
        //                   (obj.FaceNormal(i,1)+1.)*.5*255, // #1 of i-th facenormal
        //                   (obj.FaceNormal(i,0)+1.)*.5*255, // #0 of i-th facenormal
        //                   255),
        //          zbuffer);
    }
    frame.flip_vertically();
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
        // frame_aa.flip_vertically();
    }
    if(perspective) {
        if(anti_aliasing)
            frame_aa.write_tga_file("../output/ex2_draw_triangles_normal_perspective_aa.tga");
        else
            frame.write_tga_file("../output/ex2_draw_triangles_normal_perspective.tga");
    }
    else {
        if(anti_aliasing)
            frame_aa.write_tga_file("../output/ex2_draw_triangles_normal_orthographic_aa.tga");
        else
            frame.write_tga_file("../output/ex2_draw_triangles_normal_orthographic.tga");
    }
}



void draw_triangles::draw(Eigen::Vector3d light_dir, bool perspective, bool anti_aliasing) {
    if(anti_aliasing)
        resolutionX*=2, resolutionY*=2;
    objReader obj("../data/bunny.obj");
    obj.fit_screen(0.6, resolutionX, resolutionY);
    // std::cout << obj.bbox_min << " " << obj.bbox_max << std::endl;
    int marginX, marginY;
    marginX = (resolutionX - obj.bbox_size[0]) * 0.5;
    marginY = (resolutionY - obj.bbox_size[1]) * 0.5;

    TGAImage frame(resolutionX, resolutionY, TGAImage::RGB);
    float *zbuffer = new float[resolutionX*resolutionY];
    for(int i=resolutionX*resolutionY; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    double c = 10.0*(obj.bbox_max(2)-obj.bbox_min(2));
    for(int i=0; i<obj.nFaces; ++i) {
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

        float intensity = obj.FaceNormal.row(i) * light_dir;
        triangle(screen_coords, frame,
                 TGAColor((intensity+1.)*0.5*255,
                          (intensity+1.)*0.5*255,
                          (intensity+1.)*0.5*255,
                          255),
                 zbuffer);
    }
    frame.flip_vertically();
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
        // frame_aa.flip_vertically();
    }
    if (perspective) {
        if(anti_aliasing)
            frame_aa.write_tga_file("../output/ex2_draw_triangles_intensity_perspective_aa.tga");
        else
            frame.write_tga_file("../output/ex2_draw_triangles_intensity_perspective.tga");
    }
    else {
        if(anti_aliasing)
            frame_aa.write_tga_file("../output/ex2_draw_triangles_intensity_orthographic_aa.tga");
        else
            frame.write_tga_file("../output/ex2_draw_triangles_intensity_orthographic.tga");
    }
}