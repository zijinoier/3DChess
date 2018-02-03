#pragma once
#pragma warning(disable:4996)

// new
#define GLEW_STATIC

#define FREEGLUT_STATIC
#include <GL/freeglut.h>

#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <vector>
#include <string>

#ifndef GL_Lib
#ifdef _DEBUG
#define GL_Lib(name) name "d.lib"
#else
#define GL_Lib(name) name ".lib"
#endif
#endif

#pragma comment(lib, GL_Lib("freeglut_static"))

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
    glm::vec3 Normal;
    
    Vertex(glm::vec3 pos, glm::vec2 uv, glm::vec3 norm)
    : Position(pos), TexCoords(uv), Normal(norm) {};
};


class Mesh {
public:
    std::vector<Vertex> Vertices;
    
    Mesh(Vertex vertex1, Vertex vertex2, Vertex vertex3) {
        Vertices.push_back(vertex1);
        Vertices.push_back(vertex2);
        Vertices.push_back(vertex3);
    };
};

class Material {
public:
    /* attributes */
    bool isValid;
    char name[100];
    
    /* data */
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    float Ns;
    float Ni;
    float d;
    int illum;
    char map_Kd[100];
    char map_Bump[100];
    char map_Ks[100];
    
    GLuint textureKdId = -1;
    GLuint textureBumpId = -1;
    GLuint textureKsId = -1;
    std::vector<Mesh> Meshes;
    
    /* functions */
    void init() {
        Ka.x = Ka.y = Ka.z = 0.0f;
        Kd.x = Kd.y = Kd.z = 0.0f;
        Ks.x = Ks.y = Ks.z = 0.0f;
        Ns = 0.0f;
        Ni = 0.0f;
        d = 0.0f;
        illum = 0;
        map_Kd[0] = map_Bump[0] = map_Ks[0] = '\0';
        textureKdId = -1;
        textureBumpId = -1;
        textureKsId = -1;
        Meshes.clear();
        isValid = false;
    };
    
    bool loadTextureFromFile(const char *imageFilePath, const int texType);
    void printInfo();
};

class ObjLoader
{
public:
    /* attributes */
    char name[100];
    char objFilePath[100];
    char mtlFilePath[100];
    char dirPath[100];
    GLuint listIndex;
    
    glm::vec3 translate;
    glm::vec3 rotate;
    glm::vec3 scale;

    /* data */
    std::vector<Material> Materials;
    
    /* functions */
    // constructors
    ObjLoader();
    ObjLoader(const char *name, const char* objFilePath);
    ObjLoader(const char *name, const char *objFilePath, glm::vec3 _scale, glm::vec3 _translate, glm::vec3 _rotate);
    ~ObjLoader();
    
    bool loadObjFile(const char* objFilePath = NULL);
    bool loadMtlFile(const char* mtlFilePath = NULL);
    GLuint createList();
    void getDirPath();
    void printInfo();
    
private:
    /* data */
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec2> TexCoords;
    std::vector<glm::vec3> Normals;
};

