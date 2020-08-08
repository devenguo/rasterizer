#include "shader.h"

shader::shader(std::string objective,
               Eigen::Vector3d rotation,
               Eigen::Vector3d translation,
               Eigen::Vector2i resolution,
               double intensity,
               TGAColor color,
               std::string save_dir,
               int light) {
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
void shader::shading(bool anti_aliasing) {
    if(anti_aliasing) {
        resolutionX*=2;
        resolutionY*=2;
    }
    objReader obj(obj_file.c_str());
    // display in 60% of the screen size
    obj.fit_screen(0.6, resolutionX, resolutionY);
    Eigen::Vector3d light_pos = 4 * l * (obj.bbox_min + obj.bbox_size);
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
    std::vector< std::vector< std::vector<double> > >
        frame_buffer_msaa(
            resolutionX,
            std::vector< std::vector<double> >(
                resolutionY,
                std::vector<double>(3, 0.)
            )
        );
    
    for(int i=0; i<obj.nFaces; ++i) {
        // Face# i
        Eigen::Matrix3d screen_coords(3,3);
        Eigen::Matrix3d world_coords(3,3);
        Eigen::Matrix3d vertices_normal(3,3);
        for(int j=0; j<3; ++j) {
            world_coords.row(j) = obj.Vertex.row(obj.Face(i, j));
            vertices_normal.row(j) = obj.VertexNormal.row(obj.Face(i, j));
            // vertices_normal.row(j) = obj.FaceNormal.row(i);
        }

        triangle(
            world_coords,
            frame,
            vertices_normal,
            l,
            light_pos,
            I,
            kd,
            zbuffer,
            c,
            marginX,
            marginY,
            frame_buffer_msaa);
    }
    frame.flip_vertically();
    frame.write_tga_file(dir.c_str());

    TGAImage frame_aa(resolutionX/2, resolutionY/2, TGAImage::RGB);
    if(anti_aliasing) {
        for(int i=0; i<resolutionX/2; ++i) {
            for(int j=0; j<resolutionY/2; ++j) {
                TGAColor color;
                for(int k=0; k<3; ++k) {
                    color.bgra[k] = (frame_buffer_msaa[i*2][j*2][k]
                                    +frame_buffer_msaa[i*2][j*2+1][k]
                                    +frame_buffer_msaa[i*2+1][j*2][k]
                                    +frame_buffer_msaa[i*2+1][j*2+1][k])*.25;
                }
                color.bgra[3] = 255;
                frame_aa.set(i, j, color);
            }
        }
        frame_aa.flip_vertically();
        frame_aa.write_tga_file(dir.c_str());
        return;
    }
    frame.write_tga_file(dir.c_str());
}
