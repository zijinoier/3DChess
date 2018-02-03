#include "ObjLoader.h"

ObjLoader::ObjLoader() {}

ObjLoader::ObjLoader(const char *name, const char *objFilePath) : 
    translate(glm::vec3(0.0f, 0.0f, 0.0f)), 
    rotate(glm::vec3(0.0f, 0.0f, 0.0f)), 
    scale(glm::vec3(1.0f, 1.0f, 1.0f)) {
    strcpy(this->objFilePath, objFilePath);
    getDirPath();
    
    strcpy(this->name, name);
    
    loadObjFile(this->objFilePath);
    
    createList();   
}

ObjLoader::ObjLoader(const char *name, const char *objFilePath, glm::vec3 _scale, glm::vec3 _translate, glm::vec3 _rotate) : 
    scale(_scale), translate(_translate), rotate(_rotate) {
    strcpy(this->objFilePath, objFilePath);
    getDirPath();
    
    strcpy(this->name, name);
    
    loadObjFile(this->objFilePath);
    
    createList();
}

ObjLoader::~ObjLoader() {}

bool ObjLoader::loadObjFile(const char *objFilePath)
{
    FILE *fp = fopen(objFilePath, "r");
    
    int materialIndex = 0;
    
    if (fp == NULL)
        return false;
    
    while (1) {
        char lineHeader[128];
        
        // read the first word of the line
        int res = fscanf(fp, "%s", lineHeader);
        
        // the end of the files
        if (res == EOF) {
            break;
        }
        // parse lineHeader
        else {
            // import mtllib
            if (strcmp(lineHeader, "mtllib") == 0) {
                // get the mtl file name
                char mtlFileName[100];
                fscanf(fp, "%s", mtlFileName);
                
                // set the mtl file path
                strcpy(mtlFilePath, dirPath);
                strcat(mtlFilePath, mtlFileName);

                loadMtlFile(mtlFilePath);
            }
            // vertex position info
            else if (strcmp(lineHeader, "v") == 0) {
                glm::vec3 position;
                fscanf(fp, "%f %f %f\n", &position.x, &position.y, &position.z);
                Positions.push_back(position);
            }
            // vertex texture coordinates info
            else if (strcmp(lineHeader, "vt") == 0) {
                glm::vec2 uv;
                fscanf(fp, "%f %f\n", &uv.x, &uv.y);
                TexCoords.push_back(uv);
            }
            // vertex normal coordinates info
            else if (strcmp(lineHeader, "vn") == 0) {
                glm::vec3 normal;
                fscanf(fp, "%f %f %f", &normal.x, &normal.y, &normal.z);
                Normals.push_back(normal);
            }
            // if use mtl
            else if (strcmp(lineHeader, "usemtl") == 0) {
                // get the mtl name
                char materialName[100];
                fscanf(fp, "%s", materialName);
                
                bool isMtlExist = false;
                for (int i = 0; i < Materials.size(); i++) {
                    if (strcmp(Materials[i].name, materialName) == 0) {
                        isMtlExist = true;
                        materialIndex = i;
                        break;
                    }
                }
            }
            // face info
            else if (strcmp(lineHeader, "f") == 0) {
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(fp, "%d/%d/%d%d/%d/%d%d/%d/%d",
                                     &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                     &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                                     &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                // if doesn't match
                if (matches != 9) {
                    return false;
                }
                // if match
                else {
                    Vertex vertex_1(Positions.at(vertexIndex[0]-1), TexCoords.at(uvIndex[0]-1), Normals.at(normalIndex[0]-1));
                    Vertex vertex_2(Positions.at(vertexIndex[1]-1), TexCoords.at(uvIndex[1]-1), Normals.at(normalIndex[1]-1));
                    Vertex vertex_3(Positions.at(vertexIndex[2]-1), TexCoords.at(uvIndex[2]-1), Normals.at(normalIndex[2]-1));
                    Mesh mesh(vertex_1, vertex_2, vertex_3);
                    Materials[materialIndex].Meshes.push_back(mesh);
                }
            }
        }
    }
    
    return true;
}

bool ObjLoader::loadMtlFile(const char *mtlFilePath)
{
    FILE *fp = fopen(this->mtlFilePath, "r");
    
    Material material;
    
    material.init();
    
    if (!fp) {
        return false;
    }
    
    while (1) {
        char lineHeader[100];
        // read the first word of the line
        int res = fscanf(fp, "%s", lineHeader);
        if (res == EOF) {
            if (material.isValid) {
                Materials.push_back(material);
                material.init();
            }
            break;
        }
        // parse lineHeader
        else {
            if (strcmp(lineHeader, "newmtl") == 0) {
                if (material.isValid) {
                    Materials.push_back(material);
                    material.init();
                }
                
                fscanf(fp, "%s", &material.name);
                material.isValid = true;
            }
            else if (strcmp(lineHeader, "Ns") == 0) {
                fscanf(fp, "%f", &material.Ns);
            }
            else if (strcmp(lineHeader, "Ka") == 0) {
                fscanf(fp, "%f%f%f", &material.Ka.x, &material.Ka.y, &material.Ka.z);
            }
            else if (strcmp(lineHeader, "Kd") == 0) {
                fscanf(fp, "%f%f%f", &material.Kd.x, &material.Kd.y, &material.Kd.z);
            }
            else if (strcmp(lineHeader, "Ks") == 0) {
                fscanf(fp, "%f%f%f", &material.Ks.x, &material.Ks.y, &material.Ks.z);
            }
            else if (strcmp(lineHeader, "Ni") == 0) {
                fscanf(fp, "%f", &material.Ni);
            }
            else if (strcmp(lineHeader, "d") == 0) {
                fscanf(fp, "%f", &material.d);;
            }
            else if (strcmp(lineHeader, "illum") == 0) {
                fscanf(fp, "%d", &material.illum);
            }
            else if (strcmp(lineHeader, "map_Kd") == 0) {
                fscanf(fp, "%s", &material.map_Kd);
                
                char mapKdPath[100];
                strcpy(mapKdPath, dirPath);
                strcat(mapKdPath, material.map_Kd);

                material.loadTextureFromFile(mapKdPath, 1);
            }
            else if (strcmp(lineHeader, "map_Bump") == 0) {
                fscanf(fp, "%s", &material.map_Bump);
                
                char mapBumpPath[100];
                strcpy(mapBumpPath, dirPath);
                strcat(mapBumpPath, material.map_Bump);

                material.loadTextureFromFile(mapBumpPath, 2);
            }
            else if (strcmp(lineHeader, "map_Ks") == 0) {
                fscanf(fp, "%s", &material.map_Ks);
                
                char mapKsPath[100];
                strcpy(mapKsPath, dirPath);
                strcat(mapKsPath, material.map_Ks);

                material.loadTextureFromFile(mapKsPath, 3);
            }
        }
    }
    
    return true;
}

GLuint ObjLoader::createList() {
    listIndex = glGenLists(1);
    
    glNewList(listIndex, GL_COMPILE);
    {
        glPushMatrix();
        
        glScaled(0.25, 0.25, 0.25);

        // set position and rotation
        glScalef(scale.x, scale.y, scale.z);
        glTranslatef(translate.x, translate.y, translate.z);
        glRotatef(rotate.x, 1.0f, 0.0f, 0.0f);
        glRotatef(rotate.y, 0.0f, 1.0f, 0.0f);
        glRotatef(rotate.z, 0.0f, 0.0f, 1.0f);
        
        for (int j = 0; j < this->Materials.size(); j++) {
            // set material
            float Ka[3] = { this->Materials[j].Ka.x,
                this->Materials[j].Ka.y,
                this->Materials[j].Ka.z };
            float Kd[3] = { this->Materials[j].Kd.x,
                this->Materials[j].Kd.y,
                this->Materials[j].Kd.z };
            float Ks[3] = { this->Materials[j].Ks.x,
                this->Materials[j].Ks.y,
                this->Materials[j].Ks.z };
            float Ns = this->Materials[j].Ns;

            glEdgeFlag(GL_TRUE);
            glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);          // 使环境材质颜色追踪当前颜色
            glEnable(GL_COLOR_MATERIAL);                    // 启用 材质颜色追踪当前颜色
            
            glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, Kd);
            glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);
            glMaterialf(GL_FRONT, GL_SHININESS, Ns);
            
            glColor3f(1.0f, 1.0f, 1.0f);
            
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            glEnable(GL_TEXTURE_2D);
            
            if (this->Materials[j].textureKdId != -1) { 
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, this->Materials[j].textureKdId);
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            }
            if (this->Materials[j].textureBumpId != -1) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, this->Materials[j].textureBumpId);
            }
            if (this->Materials[j].textureKsId != -1) {
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, this->Materials[j].textureKsId);
            }

            for (int i = 0; i < this->Materials[j].Meshes.size(); i++) {
                Vertex *vertex_1 = &this->Materials[j].Meshes[i].Vertices[0];
                Vertex *vertex_2 = &this->Materials[j].Meshes[i].Vertices[1];
                Vertex *vertex_3 = &this->Materials[j].Meshes[i].Vertices[2];                

                glBegin(GL_TRIANGLES);
                {
                    glNormal3f(vertex_1->Normal.x, vertex_1->Normal.y, vertex_1->Normal.z);               
                    if (this->Materials[j].textureKdId != -1)
                        glMultiTexCoord2f(GL_TEXTURE0, vertex_1->TexCoords.x, vertex_1->TexCoords.y);
                    if (this->Materials[j].textureBumpId != -1)
                        glMultiTexCoord2f(GL_TEXTURE1, vertex_1->TexCoords.x, vertex_1->TexCoords.y);
                    if (this->Materials[j].textureKsId != -1)
                        glMultiTexCoord2f(GL_TEXTURE2, vertex_1->TexCoords.x, vertex_1->TexCoords.y);
                    glVertex3f(vertex_1->Position.x, vertex_1->Position.y, vertex_1->Position.z);
                    
                    glNormal3f(vertex_2->Normal.x, vertex_2->Normal.y, vertex_2->Normal.z);               
                    if (this->Materials[j].textureKdId != -1)
                        glMultiTexCoord2f(GL_TEXTURE0, vertex_2->TexCoords.x, vertex_2->TexCoords.y);
                    if (this->Materials[j].textureBumpId != -1)
                        glMultiTexCoord2f(GL_TEXTURE1, vertex_2->TexCoords.x, vertex_2->TexCoords.y);
                    if (this->Materials[j].textureKsId != -1)
                        glMultiTexCoord2f(GL_TEXTURE2, vertex_2->TexCoords.x, vertex_2->TexCoords.y);
                    glVertex3f(vertex_2->Position.x, vertex_2->Position.y, vertex_2->Position.z);

                    glNormal3f(vertex_3->Normal.x, vertex_3->Normal.y, vertex_3->Normal.z);
                    if (this->Materials[j].textureKdId != -1)
                        glMultiTexCoord2f(GL_TEXTURE0, vertex_3->TexCoords.x, vertex_3->TexCoords.y);
                    if (this->Materials[j].textureBumpId != -1)
                        glMultiTexCoord2f(GL_TEXTURE1, vertex_3->TexCoords.x, vertex_3->TexCoords.y);
                    if (this->Materials[j].textureKsId != -1)
                        glMultiTexCoord2f(GL_TEXTURE2, vertex_3->TexCoords.x, vertex_3->TexCoords.y);
                        
                    glVertex3f(vertex_3->Position.x, vertex_3->Position.y, vertex_3->Position.z);
                }
                glEnd();
            }
            if (this->Materials[j].textureKdId != -1) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            if (this->Materials[j].textureBumpId != -1) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }
            if (this->Materials[j].textureKsId != -1) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, 0);
            }

            glDisable(GL_TEXTURE_2D);
            glDisable(GL_COLOR_MATERIAL);
        }
        
        glPopMatrix();
    }
    glEndList();
    
    return listIndex;
}

void ObjLoader::getDirPath()
{
    int tmp;
    for (int i = strlen(objFilePath); i > 0; i--) {
        if (objFilePath[i] == '/') {
            tmp = i + 1;
            break;
        }
    }
    strncpy(dirPath, objFilePath, tmp);
    dirPath[tmp] = '\0';
}

bool Material::loadTextureFromFile(const char *imageFilePath, const int texType)
{
    switch (texType) {
        case 1:
            glGenTextures(1, &textureKdId);
            glBindTexture(GL_TEXTURE_2D, textureKdId);                     
            break;
        case 2:
            glGenTextures(1, &textureBumpId);
            glBindTexture(GL_TEXTURE_2D, textureBumpId);
            break;
        case 3:
            glGenTextures(1, &textureKsId);
            glBindTexture(GL_TEXTURE_2D, textureKsId);
            break;
        default:
            return false;
    }
    
    // 为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // 加载并生成纹理
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(imageFilePath, &width, &height, &nrComponents, 0);
    
    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;
    
    if (data)
        gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);       
    else
        return false;
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    /* free the image data */
    stbi_image_free(data);
    
    return true;
}

void ObjLoader::printInfo() {
    for (int i = 0; i < Materials.size(); i++) {
        Materials[i].printInfo();
    }
}

void Material::printInfo() {
    //printf("[PRINT INFO::Material] mtl %s\n", name);
    //printf("[PRINT INFO::Material] Mesh count = %d\n", Meshes.size());
}
