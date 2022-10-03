#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "FileLoader.hpp"
#include "ShaderProgram.hpp"
#include "GUIControl.hpp"
#include "VertexTypes.hpp"
#include "Timer.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "MeshManager.hpp"
#include "Mesh3D.hpp"
#include "Camera.hpp"
#include <algorithm>
#define WIDTH 1500
#define HEIGHT 1500

GUIControl gui;
ShaderProgram programTexture;
ShaderProgram programColor;
Timer timer;
Mesh3D* selected;
Mesh3D* grid;
Mesh3DList meshList;
MeshManager meshManager;
Camera* camera;

glm::vec3 vecPos;                   
glm::vec3 vecScale;                 
glm::vec4 vecColor;
glm::vec3 angles;                   

unsigned int textureId;

float randColorAdder = 0.0000000001;

void changeTextureToColor()
{
    if(selected!=nullptr)
    {
        selected->setColorActive(true);
    }
}

void changeColorToTexture()
{
    if(selected!=nullptr)
    {
        selected->setColorActive(false);
    }
}

void selectedShapeChange()
{
    if(selected!=nullptr)
    {
        gui.setColor(selected->getColor());
        gui.setAngles(selected->getAngles());
        gui.setPos(selected->getPosition());
        gui.setScale(selected->getScale());
    }
    else
    {
        angles = glm::vec3(0,0,0); vecPos = glm::vec3(0,0,0); vecScale = glm::vec3(0,0,0); vecColor = glm::vec4(1.0,0.0,1.0,1.0);
        gui.setAngles(&angles);
        gui.setPos(&vecPos);
        gui.setScale(&vecScale);
        gui.setColor(&vecColor);
    }
}


void createShape(SHAPETYPE type)
{
    if(type==SHAPE_CUBE)
    {
        meshList.push_back(meshManager.createCube());
    }
    if(type==SHAPE_PYRAMID)
    {
        meshList.push_back(meshManager.createPyramid());
    }
    if(type==SHAPE_CYLINDER)
    {
        meshList.push_back(meshManager.createCylinder());
    }   
    selected = meshList.back();
    selectedShapeChange();
}

void deleteShape()
{
    if(selected != nullptr)
    {
        std::vector<Mesh3D*>::iterator toDelete;
        toDelete = std::find(meshList.begin(), meshList.end(), selected);
        delete *toDelete;
        meshList.erase(toDelete);
        if(!meshList.empty())
        {
            selected = meshList.back();
        }
        else
        {
            selected = nullptr;
        }
        selectedShapeChange();
    }
}

void draw()
{
    double elapsedTime = timer.elapsedSeconds();
    if(elapsedTime<0.016)
    {
        unsigned int miliseconds = (0.016-elapsedTime)*1000;
        timer.sleep(miliseconds);
    }
    glClearColor(0.3, 0.4, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    programTexture.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);   
    camera->setCamera(programTexture);
    if(meshList.size())
    {
        for(auto mesh : meshList)
        {
            if(mesh==selected)
            {
                mesh->update();
            }
            mesh->draw(programTexture);
        }
    }

    programColor.use();
    camera->setCamera(programColor);
    grid->draw(programColor);

    gui.createWindow();
    gui.createWindowItems();
    gui.draw();
}

void checkCollisionDraw()
{
    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    programTexture.use();
    for(auto mesh : meshList)
    {
        mesh->draw(programTexture);
    }
    gui.createWindow();
    gui.createWindowItems();
    gui.draw();
}


void keyboard_callback(GLFWwindow* window,int key,int scanCode,int action,int mods)
{
    if(key==GLFW_KEY_ESCAPE)
        glfwTerminate();
    if(key==GLFW_KEY_W)
        camera->moveForward();
    if(key==GLFW_KEY_S)
        camera->moveBackWard();        
    if(key==GLFW_KEY_A)
        camera->moveLeft();
    if(key==GLFW_KEY_D)
        camera->moveRight();           
    if(key==GLFW_KEY_LEFT)
        camera->rotateLeft();
    if(key==GLFW_KEY_RIGHT)
        camera->rotateRight();
    if(key==GLFW_KEY_UP)
        camera->rotateUp();
    if(key==GLFW_KEY_DOWN)
        camera->rotateDown();
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{   
    if(meshList.size()==0)
        return;
    if ((button == GLFW_MOUSE_BUTTON_RIGHT || button == GLFW_MOUSE_BUTTON_LEFT) && action == GLFW_PRESS)
        {
            float pixel[3];
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            glm::vec4 colors[meshList.size()];
            bool colorActive[meshList.size()];

            for(int i = 0;i < meshList.size();i++)
            {
                if(i % 255 == 0)
                {
                    randColorAdder*=10;
                }
                float color = fmod(((float)((i+1) % 255) + randColorAdder),255.0f) / 255.0f;
                colors[i] = *meshList.at(i)->getColor();
                colorActive[i] = meshList.at(i)->getColorActive();
                meshList.at(i)->setColor(glm::vec4(color,color,color,1.0));
                meshList.at(i)->setColorActive(true);
            }
            randColorAdder = 0.0000000001;

            checkCollisionDraw();
            glReadPixels(xpos,HEIGHT-ypos,1,1,GL_RGB,GL_FLOAT,&pixel);

            for(int i = 0;i < meshList.size();i++)
            {
                Mesh3D* current = meshList.at(i);
                glm::vec4 currentColor = *current->getColor();
                if(currentColor.r == pixel[0] && currentColor.g == pixel[1] && currentColor.b == pixel[2])
                {
                    selected = current;
                    selectedShapeChange();
                }
                meshList.at(i)->setColor(colors[i]);
                meshList.at(i)->setColorActive(colorActive[i]);
            }

            bool selectedFlag = false;
            for(int i = 0;i < meshList.size();i++)
            {
                if(meshList.at(i) == selected)
                {
                    selectedFlag = true;
                }
                if(selectedFlag)
                {
                    if(i == meshList.size()-1)
                    {
                        meshList.at(i) = selected;
                    }
                    else
                    {
                        meshList.at(i) = meshList.at(i+1);
                    }
                }
            }
        }
}

unsigned int loadTexture(const std::string& fileName)
{
    unsigned int id;
    int width, height, channels;
    auto data = stbi_load(fileName.c_str(), &width, &height, &channels, 0);

    glGenTextures(1,&id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    return id;
}

void init()
{
    programTexture.attachShader("./shaders/vertex.glsl",GL_VERTEX_SHADER);
    programTexture.attachShader("./shaders/fragment.glsl",GL_FRAGMENT_SHADER);
    programTexture.link();
    programTexture.addUniform("uMtxTransform");
    programTexture.addUniform("uMtxView");
    programTexture.addUniform("uMtxProj");
    programTexture.addUniform("uTextureFlag");
    programTexture.addUniform("uColor");

    programColor.attachShader("./shaders/colorVertex.glsl",GL_VERTEX_SHADER);
    programColor.attachShader("./shaders/colorFragment.glsl",GL_FRAGMENT_SHADER);
    programColor.link();
    programColor.addUniform("uMtxTransform");
    programColor.addUniform("uMtxView");
    programColor.addUniform("uMtxProj");
    programColor.addUniform("uColor");

    textureId = loadTexture("./images/brick.jpg");

    selectedShapeChange();

    gui.clickCreateShape = createShape;
    gui.clickDeleteShape = deleteShape;
    gui.clickChangeTextureToColor = changeTextureToColor;
    gui.clickChangeColorToTexture = changeColorToTexture;
    grid = meshManager.createGrid();
    grid->setColorActive(true);
    grid->setColor({1.0,0.0,0.0,1.0});
    camera = new Camera();

    glEnable(GL_DEPTH_TEST);
    timer.start();
}


int main()
{
    if(!glfwInit())
    return -1;
    
    GLFWwindow* window = glfwCreateWindow(WIDTH,HEIGHT,"2.Odev",0,0);
    if(window == nullptr)
    {
        std::cout << "pencere olusmadi" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window,(GLFWkeyfun)keyboard_callback);
    glfwSetMouseButtonCallback(window,(GLFWmousebuttonfun)mouse_button_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<"glad yukleme hatasi" << std::endl;
    }

    init();

    gui.init(window);

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        draw();

        glfwSwapBuffers(window);
    }
}