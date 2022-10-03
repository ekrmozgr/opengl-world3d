#include "GUIControl.hpp"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <iostream>

void GUIControl::init(GLFWwindow* window)
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window,true);
    ImGui_ImplOpenGL3_Init("#version 130");
    ImGui::StyleColorsClassic();
}

void GUIControl::createWindow()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUIControl::createWindowItems()
{
    ImGui::Begin("imgui penceresi");
    ImGui::SliderFloat3("dondurme",(float*)vecAngles,0,360);
    ImGui::SliderFloat3("oteleme",(float*)vecPos, -10.0f, 10.0f);
    ImGui::SliderFloat3("olcek",(float*)vecScale, 0.0f, 2.0f);
    ImGui::ColorEdit3("renk",(float*)vecColor);
    if(ImGui::Button("Kaplama"))
    {
        clickChangeColorToTexture();
    }ImGui::SameLine();
    if(ImGui::Button("Renk"))
    {
        clickChangeTextureToColor();
    }ImGui::SameLine();
    if(ImGui::Button("Sil"))
    {
        clickDeleteShape();
    }
    ImGui::RadioButton("Kup",&selectedShape,0);ImGui::SameLine();
    ImGui::RadioButton("Piramit",&selectedShape,1);ImGui::SameLine();
    ImGui::RadioButton("Silindir",&selectedShape,2);
    if(ImGui::Button("Sekil Olustur"))
    {
        clickCreateShape((SHAPETYPE)selectedShape);
    }

    ImGui::End();
}

void GUIControl::draw()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIControl::setAngles(glm::vec3* vecAngles)
{
    this->vecAngles = vecAngles;
}

void GUIControl::setPos(glm::vec3* vecPos)
{
    this->vecPos = vecPos;
}

void GUIControl::setScale(glm::vec3* vecScale)
{
    this->vecScale = vecScale;
}

void GUIControl::setColor(glm::vec4* vecColor)
{
    this->vecColor = vecColor;
}