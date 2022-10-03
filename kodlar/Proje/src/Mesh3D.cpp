#include "Mesh3D.hpp"
#include "ShaderProgram.hpp"
#include<glm/gtc/matrix_transform.hpp>


Mesh3D::Mesh3D()
{
    position=glm::vec3(0,0,0);
    scale = glm::vec3(1.0f,1.0f,1.0f);
    angles = glm::vec3(0,0,0);
    color = glm::vec4(1.0f,0.0f,1.0f,1.0f);
    colorActive = false;
    update();
}
bool Mesh3D::getColorActive()
{
    return colorActive;
}
void Mesh3D::setColorActive(bool active)
{
    colorActive = active;
}
void Mesh3D::setColor(const glm::vec4& color)
{
    this->color = color;
 
}
void Mesh3D::setPosition(const glm::vec3& pos)
{
    this->position = pos;
    update();
}
void Mesh3D::setScale(const glm::vec3& scale)
{
    this->scale = scale;
    update();
}
void Mesh3D::setAngles(const glm::vec3& angles)
{
    this->angles = angles;
    update();
}
void Mesh3D::update()
{

    glm::mat4 mtxRotateX = glm::rotate(glm::mat4(1),glm::radians(angles.x),glm::vec3(1.0,0.0f,0.0));
    glm::mat4 mtxRotateY = glm::rotate(glm::mat4(1),glm::radians(angles.y),glm::vec3(0.0,1.0f,0.0));
    glm::mat4 mtxRotateZ = glm::rotate(glm::mat4(1),glm::radians(angles.z),glm::vec3(0.0,0.0f,1.0));        
    glm::mat4 mtxRotate = mtxRotateX*mtxRotateY*mtxRotateZ;
    glm::mat4 mtxTranslate = glm::translate(glm::mat4(1),position);
    glm::mat4 mtxScale = glm::scale(glm::mat4(1),scale);
    mtxWorld = mtxTranslate*mtxRotate*mtxScale;

}
void Mesh3D::draw(ShaderProgram& program)
{
    program.setMat4("uMtxTransform",mtxWorld);
    if(colorActive)
    {
        program.setVec4("uColor",color);
        program.setInt("uTextureFlag",0);
    }
    else
    {
        program.setVec4("uColor",glm::vec4(0.0,0.0,0.0,0.0));
        program.setInt("uTextureFlag",1);
    }
    vertexArrayObject.use();
    vertexArrayObject.draw();
    vertexArrayObject.unuse();
}

glm::vec3* Mesh3D::getPosition()
{
    return &this->position;
}

glm::vec3* Mesh3D::getScale()
{
    return &this->scale;
}

glm::vec4* Mesh3D::getColor()
{
    return &this->color;
}

glm::vec3* Mesh3D::getAngles()
{
    return &this->angles;
}

void Mesh3D::setRandomPosition()
{
    int x = rand()%11;
    int y = rand()%11;
    int z = rand()%11;
    int _rand = rand()%2;
    if(_rand == 0)
        x = -x;
    _rand = rand()%2;
    if(_rand == 0)
        y = -y;
    _rand = rand()%2;
    if(_rand == 0)
        z = -z;
    position = glm::vec3(x,y,z);
}