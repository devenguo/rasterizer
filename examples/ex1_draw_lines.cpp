#include "examples/ex1_draw_lines.h"

draw_lines::draw_lines(int resX, int resY): resolutionX(resX), resolutionY(resY){}
void draw_lines::draw() {
    objReader obj("../data/bunny.obj");
    // printf("nFace = %d, nVertices = %d, nEdges = %d\n",
    //        obj.nFaces, obj.nVertices, obj.nEdges);
    Eigen::VectorXd bbox_min = obj.Vertex.colwise().minCoeff();
    Eigen::VectorXd bbox_max = obj.Vertex.colwise().maxCoeff();
    Eigen::VectorXd bbox_size = bbox_max - bbox_min;
    // std::cout << "bbox_min = " << bbox_min.transpose()
    //           << "\n" << "bbox_max = " << bbox_max.transpose()
    //           << "\n" << "bbox_size = " << bbox_size.transpose() << std::endl;
    float scale = std::min(resolutionX * 1.0 / bbox_size(0), resolutionY * 1.0 / bbox_size(1)) * 0.75;
    // std::cout << "scale = " << scale << std::endl;
    // scaled positions
    const TGAColor white = TGAColor(255, 255, 255, 255);
    TGAImage image(resolutionX, resolutionY, TGAImage::RGB);
    obj.Vertex = obj.Vertex.rowwise() - bbox_min.transpose();
    obj.Vertex *= scale;
    // update bbox
    bbox_min *= scale;
    bbox_max *= scale;
    bbox_size *= scale;
    // draw
    int marginX, marginY;
    marginX = (resolutionX - bbox_size[0]) * 0.5;
    marginY = (resolutionY - bbox_size[1]) * 0.5;
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