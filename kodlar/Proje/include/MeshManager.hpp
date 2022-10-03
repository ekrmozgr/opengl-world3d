#ifndef MeshManager_hpp
#define MeshManager_hpp
#include<map>
#include<string>
#include "VertexTypes.hpp"
class Mesh3D;
class VertexArrayObject;

class MeshManager
{
public:
    Mesh3D* createCube();
    Mesh3D* createPyramid();
    Mesh3D* createCylinder();
    Mesh3D* createGrid();
private:
    VertexArrayObject createTextureBuffers(const TexturedVertexList& vertices, const IndexList& indices);
    VertexArrayObject createBuffers(VertexList&  vertices,IndexList& indices);
    std::map<std::string,VertexArrayObject> vaoMap;
};


#endif