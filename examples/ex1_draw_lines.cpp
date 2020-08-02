#include "examples/ex1_draw_lines.h"

draw_lines::draw_lines(int resX, int resY): resolutionX(resX), resolutionY(resY){}
void draw_lines::draw() {
    objReader obj("../data/bunny.obj");
    obj.fit_screen(0.75, resolutionX, resolutionY);
    const TGAColor white = TGAColor(255, 255, 255, 255);
    TGAImage image(resolutionX, resolutionY, TGAImage::RGB);
    // draw
    int marginX, marginY;
    marginX = (resolutionX - obj.bbox_size[0]) * 0.5;
    marginY = (resolutionY - obj.bbox_size[1]) * 0.5;
    for(int i=0; i<obj.nEdges; ++i) {
        float v00 = obj.Vertex(obj.Edge[i][0], 0) + marginX,
              v01 = obj.Vertex(obj.Edge[i][0], 1) + marginY,
              v10 = obj.Vertex(obj.Edge[i][1], 0) + marginX,
              v11 = obj.Vertex(obj.Edge[i][1], 1) + marginY;
        Eigen::Vector2i p0, p1;
        p0 << v00, v01; p1 << v10, v11;
        line(p0, p1, image, white);
    }
    image.flip_vertically();
    image.write_tga_file("../output/ex1_draw_lines.tga");
}