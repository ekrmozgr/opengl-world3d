#ifndef GUIControl_hpp
#define GUIControl_hpp
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <functional>
class GLFWwindow;

enum SHAPETYPE
{
    SHAPE_CUBE = 0,
    SHAPE_PYRAMID = 1,
    SHAPE_CYLINDER = 2
};

typedef std::function<void(SHAPETYPE)> CreateShapeFunction;
typedef std::function<void()> DeleteShapeFunction;
typedef std::function<void()> TextureToColorFunction;
typedef std::function<void()> ColorToTextureFunction;
class GUIControl
{
public:
    void init(GLFWwindow* window);
    void createWindow();
    void createWindowItems();
    void draw();
    void setAngles(glm::vec3* angles);
    void setPos(glm::vec3* vecPos);
    void setScale(glm::vec3* vecScale);
    void setColor(glm::vec4* vecColor);
    CreateShapeFunction  clickCreateShape;
    DeleteShapeFunction  clickDeleteShape;
    TextureToColorFunction clickChangeTextureToColor;
    ColorToTextureFunction clickChangeColorToTexture;
private:
    glm::vec3* vecAngles;
    glm::vec3* vecPos;
    glm::vec3* vecScale;
    glm::vec4* vecColor;
    int selectedShape;
};

#endif