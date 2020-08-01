#ifndef __OBJ_READER_H__
#define __OBJ_READER_H__
#include <Eigen/Dense>
#include <string>
#include <vector>
class objReader{
public:
    // objReader();
    objReader(std::string filename);
    Eigen::MatrixXd Vertex;
    Eigen::MatrixXi Face;
    int nVertices, nFaces, nEdges;

    std::vector< std::vector<int> > VertexNeighbor;
    std::vector< std::vector<int> > Edge;

    // todo
    Eigen::MatrixXd VertexNormal;
    Eigen::MatrixXd FaceNormal;
    std::vector< std::vector<int> > FaceNeighbor;
};
#endif