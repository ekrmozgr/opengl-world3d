#ifndef Mesh3D_hpp
#define Mesh3D_hpp

#include<glm/mat4x4.hpp>
#include<glm/vec3.hpp>
#include<glm/vec4.hpp>
#include<vector>
#include "VertexArrayObject.hpp"
class ShaderProgram;
class MeshManager;


class Mesh3D
{
public:
    void setPosition(const glm::vec3& pos);
    void setRoll(float angle);
    void setYaw(float angle);
    void setPitch(float pitch);
    void setColor(const glm::vec4& color);
    void setScale(const glm::vec3& scale);
    void setAngles(const glm::vec3& angles);
    void draw(ShaderProgram& shader);
    void update();
    bool getColorActive();
    void setColorActive(bool active);
    glm::vec3* getPosition();
    glm::vec3* getAngles();
    glm::vec4* getColor();
    glm::vec3* getScale();
    void setRandomPosition();
private:
    Mesh3D();
    bool                colorActive;
    glm::mat4           mtxWorld;
    glm::vec3           position;
    glm::vec3           scale;
    glm::vec4           color;
    glm::vec3           angles;
    VertexArrayObject   vertexArrayObject;  
    friend MeshManager;
};
typedef std::vector<Mesh3D*> Mesh3DList;
#endif