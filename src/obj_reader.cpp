#include "obj_reader.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
// objReader::objReader(): nVertices(0), nFaces(0), nEdges(0) {}

objReader::objReader(std::string filename) {
    nVertices = 0;
    nFaces = 0;
    nEdges = 0;

    std::ifstream infile;
    infile.open(filename);

    std::vector< std::vector<float> > _Vertex;
    std::vector< std::vector<int> > _Face;

    std::string line;
    while(std::getline(infile, line)) {
        std::istringstream iss(line);
        char _;
        if (line[0]=='v') {
            float x, y, z;
            iss >> _ >> x >> y >> z;
            std::vector<float> tmp(3);
            tmp[0] = x; tmp[1] = y; tmp[2] = z;
            _Vertex.push_back(tmp);
            ++nVertices;
        } else if (line[0]=='f') {
            int v0, v1, v2;
            iss >> _ >> v0 >> v1 >> v2;
            std::vector<int> tmp(3);
            tmp[0] = v0-1; tmp[1] = v1-1; tmp[2] = v2-1;
            _Face.push_back(tmp);
            ++nFaces;
        }
    }

    Vertex.resize(nVertices, 3);
    Face.resize(nFaces, 3);
    VertexNeighbor.resize(nVertices);
    for(int i=0; i<nVertices; ++i) {
        for(int j=0; j<3; ++j) {
            Vertex(i,j) = _Vertex[i][j];
        }
    }

    for(int i=0; i<nFaces; ++i) {
        for(int j=0; j<3; ++j) {
            Face(i,j) = _Face[i][j];
            int v0 = _Face[i][j],
                v1 = _Face[i][(j+1)%3],
                v2 = _Face[i][(j+2)%3];

            bool visited_v1=false, visited_v2=false;
            for(int k=0; k<VertexNeighbor[v0].size(); ++k) {
                if(VertexNeighbor[v0][k]==v1) visited_v1 = true;
                else if(VertexNeighbor[v0][k]==v2) visited_v2 = true;
            }
            if(!visited_v1) VertexNeighbor[v0].push_back(v1);
            if(!visited_v2) VertexNeighbor[v0].push_back(v2);

        }
    }

    for(int i=0; i<nVertices; ++i) {
        for(int j=0; j<VertexNeighbor[i].size(); ++j) {
            if(i<VertexNeighbor[i][j]) {
                std::vector<int> e(2);
                e[0] = i; e[1] = VertexNeighbor[i][j];
                Edge.push_back(e);
            }
        }
    }
    nEdges = Edge.size();

    // bbox setup
    bbox_min = Vertex.colwise().minCoeff();
    bbox_max = Vertex.colwise().maxCoeff();
    bbox_size = bbox_max - bbox_min;

    // face normal setup
    __update_face_normal();

    // std::ofstream vertex_neighbor("../output/vertex_neighbor.txt");
    // for(int i=0; i<nVertices; ++i) {
    //     vertex_neighbor << " " << i << " ";
    //     for(int j=0; j<VertexNeighbor[i].size(); ++j) {
    //         vertex_neighbor << VertexNeighbor[i][j] << " ";
    //     }
    //     vertex_neighbor << std::endl;
    // }
    // std::ofstream edge("../output/edge.txt");
    // for(int i=0; i<nEdges; ++i) {
    //     edge << " " << Edge[i][0] << " " << Edge[i][1] << " " << std::endl;
    // }

}

void objReader::fit_screen(double ratio, int resX, int resY) {
    float scale = std::min(resX * 1.0 / bbox_size(0), resY * 1.0 / bbox_size(1)) * ratio;
    Vertex = Vertex.rowwise() - bbox_min.transpose();
    // now the left-lower bound is (0, 0, 0)

    // update
    Vertex *= scale;
    bbox_min *= scale;
    bbox_max *= scale;
    bbox_size *= scale;
    // __update_face_normal();
}

void objReader::__update_face_normal() {
    FaceNormal.resize(nFaces, 3);
    for(int i=0; i<nFaces; ++i) {
        Eigen::Vector3d v0, v1, normal;
        v0 = Vertex.row(Face(i,2)) - Vertex.row(Face(i,0));
        v1 = Vertex.row(Face(i,1)) - Vertex.row(Face(i,0));
        normal = v0.cross(v1).normalized();
        for(int j=0; j<3; ++j) FaceNormal(i,j) = normal(j);
    }
}