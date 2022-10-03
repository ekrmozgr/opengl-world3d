#include "MeshManager.hpp"
#include "Mesh3D.hpp"
#include "VertexTypes.hpp"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <iostream>


VertexArrayObject MeshManager::createTextureBuffers(const TexturedVertexList& vertices, const IndexList& indices)
{
    unsigned int vertexBufferId;
    unsigned int vertexArrayObjectId;
    unsigned int indexBufferId;

    glGenVertexArrays(1,&vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);

    glGenBuffers(1,&vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER,vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER,sizeof(TexturedVertex)*vertices.size(),&vertices[0],GL_STATIC_DRAW);

    glGenBuffers(1,&indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),&indices[0],GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(TexturedVertex),0);
    glVertexAttribPointer(1,2,GL_FLOAT,false,sizeof(TexturedVertex),(const void*)(sizeof(float)*3));
    glEnableVertexAttribArray(0);  
    glEnableVertexAttribArray(1); 


    VertexArrayObject vao;
    vao.indexCount = indices.size();
    vao.id= vertexArrayObjectId;
    return vao;
}
VertexArrayObject MeshManager::createBuffers(VertexList& vertices,IndexList& indices)
{
    unsigned int vertexBufferId;
    unsigned int vertexArrayObjectId;
    unsigned int indexBufferId;

    glGenVertexArrays(1,&vertexArrayObjectId);
    glBindVertexArray(vertexArrayObjectId);

    glGenBuffers(1,&vertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER,vertexBufferId);
    glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*vertices.size(),&vertices[0],GL_STATIC_DRAW);

    glGenBuffers(1,&indexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(unsigned int)*indices.size(),&indices[0],GL_STATIC_DRAW);


    glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(glm::vec3),0);
    glEnableVertexAttribArray(0);  

    VertexArrayObject vao;
    vao.indexCount = indices.size();
    vao.id= vertexArrayObjectId;
    return vao;
}
Mesh3D* MeshManager::createGrid() 
{
    VertexArrayObject vao;

    if(!vaoMap.count("grid"))
    { 
        unsigned int vertexArrayObjectId;
        
        float length = 10;
        int hVertexCount = length*4+1;
        int vVertexCount = length*4+1;

        int vertexCount = vVertexCount*hVertexCount;
        float distance= length/hVertexCount;

        VertexList vertices;
 
        IndexList   indices;    

        for(int i=0;i<hVertexCount*2+1;i++)
        {
            float x1 = -length;
            float y1 = 0.0f;
            float z1 = -length+i*distance;

            float x2 = length;
            float y2 = 0.0f;
            float z2 = -length+i*distance;
            vertices.emplace_back(x1,y1,z1);
            vertices.emplace_back(x2,y2,z2);

            vertices.emplace_back(z1,y1,x1);
            vertices.emplace_back(z2,y2,x2);   

            indices.push_back(i*4);
            indices.push_back(i*4+1);
            indices.push_back(i*4+2);
            indices.push_back(i*4+3);            
        

        }
        
        vao = createBuffers(vertices,indices);
		vao.primitiveType = GL_LINES;     
        vaoMap["grid"] = vao;    
    }
    else
    {
        vao = vaoMap["grid"];
    }
    Mesh3D *mesh= new Mesh3D;
    mesh->vertexArrayObject = vao;

    return mesh;        
}
Mesh3D* MeshManager::createCube()
{
    VertexArrayObject vao;

    if(!vaoMap.count("cube"))
    {
        unsigned int vertexArrayObjectId;
        int vertexCount = 24;
    
        TexturedVertexList vertices(vertexCount);
        int faceCount= 6;
        IndexList   indices(faceCount*6); 
        glm::vec3 v[8];
        v[0] = glm::vec3(-0.5,0.5,0.5);   
        v[1] = glm::vec3(0.5,0.5,0.5);
        v[2] = glm::vec3(0.5,-0.5,0.5);
        v[3] = glm::vec3(-0.5,-0.5,0.5);
        v[4] = glm::vec3(-0.5,0.5,-0.5);   
        v[5] = glm::vec3(0.5,0.5,-0.5);
        v[6] = glm::vec3(0.5,-0.5,-0.5);
        v[7] = glm::vec3(-0.5,-0.5,-0.5);
        //ön yüz
        vertices[0].pos = v[0]; vertices[0].tex= glm::vec2(0.0,1.0);
        vertices[1].pos = v[1]; vertices[1].tex= glm::vec2(1.0,1.0);
        vertices[2].pos = v[2]; vertices[2].tex= glm::vec2(1.0,0.0);
        vertices[3].pos = v[3]; vertices[3].tex= glm::vec2(0.0,0.0);
        //sağ
        vertices[4].pos = v[1]; vertices[4].tex= glm::vec2(0.0,1.0);
        vertices[5].pos = v[5]; vertices[5].tex= glm::vec2(1.0,1.0);
        vertices[6].pos = v[6]; vertices[6].tex= glm::vec2(1.0,0.0);
        vertices[7].pos = v[2]; vertices[7].tex= glm::vec2(0.0,0.0);  

        //ust
        vertices[8].pos  = v[4]; vertices[8].tex= glm::vec2(0.0,1.0);
        vertices[9].pos  = v[5]; vertices[9].tex= glm::vec2(1.0,1.0);
        vertices[10].pos = v[1]; vertices[10].tex= glm::vec2(1.0,0.0);
        vertices[11].pos = v[0]; vertices[11].tex= glm::vec2(0.0,0.0);   

        //sol
        vertices[12].pos = v[4]; vertices[12].tex= glm::vec2(0.0,1.0);
        vertices[13].pos = v[0]; vertices[13].tex= glm::vec2(1.0,1.0);
        vertices[14].pos = v[3];  vertices[14].tex= glm::vec2(1.0,0.0);
        vertices[15].pos = v[7];  vertices[15].tex= glm::vec2(0.0,0.0);  

        //alt
        vertices[16].pos = v[7];  vertices[16].tex= glm::vec2(0.0,1.0);
        vertices[17].pos = v[6];  vertices[17].tex= glm::vec2(1.0,1.0);
        vertices[18].pos = v[2];  vertices[18].tex= glm::vec2(1.0,0.0);
        vertices[19].pos = v[3];  vertices[19].tex= glm::vec2(0.0,0.0);   
        //arka
        vertices[20].pos = v[4];  vertices[20].tex= glm::vec2(0.0,1.0);
        vertices[21].pos = v[5];  vertices[21].tex= glm::vec2(1.0,1.0);
        vertices[22].pos = v[6];  vertices[22].tex= glm::vec2(1.0,0.0);
        vertices[23].pos = v[7];  vertices[23].tex= glm::vec2(0.0,0.0);   

        for(int i=0;i<faceCount;i++)
        {
            int index= i;
            indices[i*6]   =index*4; indices[i*6+1] =index*4+1; indices[i*6+2] =index*4+2;
            indices[i*6+3] =index*4; indices[i*6+4] =index*4+2; indices[i*6+5] =index*4+3;
        }
        vao = createTextureBuffers(vertices,indices);
		vao.primitiveType = GL_TRIANGLES;
        vaoMap["cube"] = vao;                  
    }
    else
    {
        vao = vaoMap["cube"];
    }
    Mesh3D *mesh= new Mesh3D;
    mesh->vertexArrayObject = vao;
    mesh->setRandomPosition();
    return mesh;
}
Mesh3D* MeshManager::createPyramid() 
{
    VertexArrayObject vao;

    if(!vaoMap.count("pyramid"))
    { 
        unsigned int vertexArrayObjectId;
        int vertexCount = 16;
        int triangleCount = 6;
        int indexCount = triangleCount * 3;
        IndexList   indices(indexCount); 
        TexturedVertexList vertices(vertexCount);

        glm::vec3 v[5];
        v[0] = glm::vec3(-0.5,-0.5,0.5);
        v[1] = glm::vec3(-0.5,-0.5,-0.5);
        v[2] = glm::vec3(0.5,-0.5,-0.5);
        v[3] = glm::vec3(0.5,-0.5,0.5);
        v[4] = glm::vec3(0,0.5,0);

        //taban
        vertices[0].pos = v[0]; vertices[0].tex = glm::vec2(0,0);
        vertices[1].pos = v[1]; vertices[1].tex = glm::vec2(0,1);
        vertices[2].pos = v[2]; vertices[2].tex = glm::vec2(1,1);
        vertices[3].pos = v[3]; vertices[3].tex = glm::vec2(1,0);

        indices[0] = 0;     indices[3] = 0;
        indices[1] = 1;     indices[4] = 2;
        indices[2] = 2;     indices[5] = 3;

        //on yuz
        vertices[4].pos = v[0]; vertices[4].tex = glm::vec2(0,0);
        vertices[5].pos = v[3]; vertices[5].tex = glm::vec2(1,0);
        vertices[6].pos = v[4]; vertices[6].tex = glm::vec2(0.5,1);

        indices[6] = 4;
        indices[7] = 5;
        indices[8] = 6;

        //sag yuz
        vertices[7].pos = v[3]; vertices[7].tex = glm::vec2(0,0);
        vertices[8].pos = v[2]; vertices[8].tex = glm::vec2(1,0);
        vertices[9].pos = v[4]; vertices[9].tex = glm::vec2(0.5,1);

        indices[9] = 7; 
        indices[10] = 8;
        indices[11] = 9;

        //arka uz
        vertices[10].pos = v[2];    vertices[10].tex = glm::vec2(0,0);
        vertices[11].pos = v[1];    vertices[11].tex = glm::vec2(1,0);
        vertices[12].pos = v[4];    vertices[12].tex = glm::vec2(0.5,1);

        indices[12] = 10;
        indices[13] = 11;
        indices[14] = 12;

        //sol yuz
        vertices[13].pos = v[1];    vertices[13].tex = glm::vec2(0,0);
        vertices[14].pos = v[0];    vertices[14].tex = glm::vec2(1,0);
        vertices[15].pos = v[4];    vertices[15].tex = glm::vec2(0.5,1);

        indices[15] = 13;
        indices[16] = 14;
        indices[17] = 15;
        
        vao = createTextureBuffers(vertices,indices);
		vao.primitiveType = GL_TRIANGLES;     
        vaoMap["pyramid"] = vao;    
    }
    else
    {
        vao = vaoMap["pyramid"];
    }
    Mesh3D *mesh= new Mesh3D;
    mesh->vertexArrayObject = vao;
    mesh->setRandomPosition();
    return mesh;        
}

Mesh3D* MeshManager::createCylinder() 
{
VertexArrayObject vao;

    if(!vaoMap.count("cylinder"))
    { 
        unsigned int vertexArrayObjectId;
        IndexList   indices; 
        
        float length = 1.5f;
        float radius = 1.5f;
        //parametre çap alıyor yariçapa çeviriyoruz.
        float halfRadius = radius/2.0f;
        //Dairenin noktaları arasındaki açı miktarı hesaplanıyor
        int angleIncrese = 5;

        //Bir daire için gerekli olan nokta sayısı hesaplanıyor
        int circleVertexCount = 360/angleIncrese;

        //vertex dizisinin boyutu başlangıçta bütün noktaları alacak kadar genişlitiliyor
        //bu sayede noktaları sıra ile eklemek zorunda kalmıyoruz.
        TexturedVertexList vertices(circleVertexCount*4);
        // noktalar hesaplanıyor
        for(int i = 0;i<circleVertexCount;i++)
        {
            TexturedVertex vertex;

            //Önceclikle ön yüzdedki dairenin nokta koordinatları hesaplanıyor
            float angle =(float) i*angleIncrese;
            vertex.pos.x = glm::cos(glm::radians(angle))*halfRadius;
            vertex.pos.y = glm::sin(glm::radians(angle))*halfRadius;

            vertex.pos.z = length/2.0f;
            
            vertices[i] = vertex;
            vertices[i].tex.x = glm::cos(glm::radians(angle))*0.5+0.5;
            vertices[i].tex.y = glm::sin(glm::radians(angle))*0.5+0.5;

            //ön dairenin nokyaları ayrıca ara yüzeydede  kullanacağı için
            //vertex listesi içerisine aynı bir noktaya yerleştiriliyor
            //tabi renk değeri farklı veriliyor
            vertices[i+circleVertexCount*2] =vertex;

            //Alt yüzeyin koordinatındaki tek fark z eksenindeki konumudur. 
            //buna göre ayarlanıyor ve diziye yerleştiriyor
            vertex.pos.z = -length/2.0f;

            vertices[i+circleVertexCount] = vertex;
            vertices[i+circleVertexCount].tex.x = glm::cos(glm::radians(angle))*0.5+0.5;
            vertices[i+circleVertexCount].tex.y = glm::sin(glm::radians(angle))*0.5+0.5;

            //bir öncekinde olduğu gibi ara yüzeylerin alt koordinatları da diziye yerleştiriliyor
            vertices[i+circleVertexCount*3] = vertex;
        }

        //dairelerin indeks değerleri ayarlanıyor 
        for(int i =0;i<circleVertexCount-2;i++)
        {
            //Ön Yüz 
            indices.push_back(0);
            indices.push_back(i+1);
            indices.push_back(i+2);
            //Arka yüz
            indices.push_back(circleVertexCount);
            indices.push_back(circleVertexCount+i+1);
            indices.push_back(circleVertexCount+i+2);
        }
   
    
        int startIndex = circleVertexCount*2;
        int currentIndex = startIndex;

        //ara yüzler
        for(int i =0;i<circleVertexCount-1;i++)
        {
            indices.push_back(currentIndex);
            indices.push_back(currentIndex+circleVertexCount);
            indices.push_back(currentIndex+circleVertexCount+1);

            indices.push_back(currentIndex);
            indices.push_back(currentIndex+circleVertexCount+1);
            indices.push_back(currentIndex+1);

            currentIndex++;
        }
        //Son yüzeyde indeks tekrardan başa sardığı için döngü dışına aldık
        indices.push_back(currentIndex);
        indices.push_back(currentIndex+circleVertexCount);
        indices.push_back(currentIndex+1);

        indices.push_back(currentIndex);
        indices.push_back(currentIndex+1);
        indices.push_back(startIndex);    
        
        vao = createTextureBuffers(vertices,indices);
		vao.primitiveType = GL_TRIANGLES;    
        vaoMap["cylinder"] = vao;    
    }
    else
    {
        vao = vaoMap["cylinder"];
    }
    Mesh3D *mesh= new Mesh3D;
    mesh->vertexArrayObject = vao;
    mesh->setRandomPosition();
    return mesh;        
}

