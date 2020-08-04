#include "rasterizer.h"
#include "transform.h"
#include "triangle.h"
rasterizer::rasterizer(std::string objective,
                       Eigen::Vector3d rotation,
                       Eigen::Vector3d translation,
                       Eigen::Vector2i resolution,
                       std::string save_dir) {
    obj_file = objective;
    resolutionX = resolution(0);
    resolutionY = resolution(1);
    dir = save_dir;
    r = rotation;
    t = translation;
}

void rasterizer::rasterize(bool perspective, bool anti_aliasing) {
    if(anti_aliasing) {
        // std::cout << "anti-aliasing" <<std::endl;
        resolutionX*=2;
        resolutionY*=2;
    }
    objReader obj(obj_file.c_str());
    obj.fit_screen(0.6, resolutionX, resolutionY);
    int marginX, marginY;
    marginX = (resolutionX - obj.bbox_size[0]) * 0.5;
    marginY = (resolutionY - obj.bbox_size[1]) * 0.5;

    TGAImage frame(resolutionX, resolutionY, TGAImage::RGB);
    float *zbuffer = new float[resolutionX*resolutionY];
    for(int i=resolutionX*resolutionY; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    
    double c = 20*obj.bbox_size(2);
    // std::cout << c << std::endl << obj.bbox_max << obj.bbox_min << std::endl;

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
        triangle(screen_coords, frame,
                 TGAColor((obj.FaceNormal(i,2)+1.)*.5*255,
                          (obj.FaceNormal(i,1)+1.)*.5*255,
                          (obj.FaceNormal(i,0)+1.)*.5*255,
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
    if(perspective) {
        if(anti_aliasing)
            frame_aa.write_tga_file(dir.c_str());
        else
            frame.write_tga_file(dir.c_str());
    }
    else {
        if(anti_aliasing)
            frame_aa.write_tga_file(dir.c_str());
        else
            frame.write_tga_file(dir.c_str());
    }
}