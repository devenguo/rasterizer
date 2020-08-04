#ifndef __OBJ_READER_H__
#define __OBJ_READER_H__
#include <Eigen/Dense>
#include <string>
#include <vector>
class objReader{
protected:
    void __update_face_normal();
public:
    // objReader();
    objReader(std::string filename);
    // Sometimes the obj is quite small or large,
    // thus I want to resize the obj in order to fit the screen
    void fit_screen(double ratio, int resX, int resY);
    void transform(Eigen::Matrix4d T);
    Eigen::MatrixXd Vertex;
    Eigen::MatrixXi Face;
    int nVertices, nFaces, nEdges;
    Eigen::VectorXd bbox_min;
    Eigen::VectorXd bbox_max;
    Eigen::VectorXd bbox_size;
    
    std::vector< std::vector<int> > VertexNeighbor;
    std::vector< std::vector<int> > Edge;

    Eigen::MatrixXd FaceNormal;

    // todo
    Eigen::MatrixXd VertexNormal;
    std::vector< std::vector<int> > FaceNeighbor;
};
#endif