#ifndef VertexArrayObject_hpp
#define VertexArrayObject_hpp
#include<vector>
class MeshManager;
class Mesh3D;
class VertexArrayObject
{
public:
    VertexArrayObject();        
    void use();
    void draw();
    void unuse();
private:
    unsigned int indexCount;
    unsigned int id;
    friend MeshManager;         
    unsigned int primitiveType;
    friend Mesh3D;
};
#endif