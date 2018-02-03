#ifndef _Environment_h
#define _Environment_h

#include "ObjLoader.h"
#include "Camera.h"

class Environment {
public:
	/* attributes */
	char name[100];
	glm::vec3 center;
	float x_span;
	float y_span;
	float z_span;
	
	GLuint wallTextureId_1;
	GLuint wallTextureId_2;
	GLuint wallTextureId_3;
	GLuint wallTextureId_4;
	GLuint floorTextureId;
	GLuint ceilingTextureId;

	char wallImageFilePath_1[100];
	char wallImageFilePath_2[100];
	char wallImageFilePath_3[100];
	char wallImageFilePath_4[100];
	char floorImageFilePath[100];
	char ceilingImageFilePath[100];

	std::vector<ObjLoader> furnitures;

	/* functions */
	// constructors
	Environment();
	Environment(glm::vec3 _center,
				float _x_span, float _y_span, float _z_span,
				char *_wallImageFilePath_1,
				char *_wallImageFilePath_2,
				char *_wallImageFilePath_3,
				char *_wallImageFilePath_4,
				char *_floorImageFilePath,
				char *_ceilingImageFilePath) :
				center(_center), x_span(_x_span), y_span(_y_span), z_span(_z_span) {
		strcpy(wallImageFilePath_1, _wallImageFilePath_1);
		strcpy(wallImageFilePath_2, _wallImageFilePath_2);
		strcpy(wallImageFilePath_3, _wallImageFilePath_3);
		strcpy(wallImageFilePath_4, _wallImageFilePath_4);
		strcpy(floorImageFilePath, _floorImageFilePath);
		strcpy(ceilingImageFilePath, _ceilingImageFilePath);
	};
	~Environment();

	void setup() {
	    loadTexture(&wallTextureId_1, wallImageFilePath_1);
	    loadTexture(&wallTextureId_2, wallImageFilePath_2);
	    loadTexture(&wallTextureId_3, wallImageFilePath_3);
	    loadTexture(&wallTextureId_4, wallImageFilePath_4);
	    loadTexture(&floorTextureId, floorImageFilePath);
	    loadTexture(&ceilingTextureId, ceilingImageFilePath);
	}

	bool loadTexture(GLuint *pTextureId, char *imageFilePath) {
		int width, height, nrComponents;
	    GLenum format;

		glGenTextures(1, pTextureId);
        glBindTexture(GL_TEXTURE_2D, *pTextureId);
		
		// 为当前绑定的纹理对象设置环绕、过滤方式
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 加载并生成纹理
	    stbi_set_flip_vertically_on_load(true);
	    unsigned char *data = stbi_load(imageFilePath, &width, &height, &nrComponents, 0);

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

	void addFurniture(char *name, char *furnitureObjFilePath,
					  glm::vec3 scale,
					  glm::vec3 translate,
					  glm::vec3 rotate) {
		furnitures.push_back(ObjLoader(name, furnitureObjFilePath, scale, translate, rotate));
	}

	void draw() {
		glPushMatrix();
		
		glm::vec3 vertex_xyz(center.x + x_span/2, center.y + y_span/2, center.z + z_span*3/4);
		glm::vec3 vertex_xy_z(center.x + x_span/2, center.y + y_span/2, center.z - z_span/4);
		glm::vec3 vertex_x_yz(center.x + x_span/2, center.y - y_span/2, center.z + z_span*3/4);
		glm::vec3 vertex__xyz(center.x - x_span/2, center.y + y_span/2, center.z + z_span*3/4);
		glm::vec3 vertex_x_y_z(center.x + x_span/2, center.y - y_span/2, center.z - z_span/4);
		glm::vec3 vertex__xy_z(center.x - x_span/2, center.y + y_span/2, center.z - z_span/4);
		glm::vec3 vertex__x_yz(center.x - x_span/2, center.y - y_span/2, center.z + z_span*3/4);
		glm::vec3 vertex__x_y_z(center.x - x_span/2, center.y - y_span/2, center.z - z_span/4);
		drawOneSurface(wallTextureId_1, vertex_xyz, vertex_xy_z, vertex_x_y_z, vertex_x_yz);
		drawOneSurface(floorTextureId, vertex_xy_z, vertex__xy_z, vertex__x_y_z, vertex_x_y_z);
		drawOneSurface(wallTextureId_3, vertex__xy_z, vertex__xyz, vertex__x_yz, vertex__x_y_z);
		drawOneSurface(ceilingTextureId, vertex__xyz, vertex_xyz, vertex_x_yz, vertex__x_yz);
		drawOneSurface(wallTextureId_2, vertex__x_y_z, vertex_x_y_z, vertex_x_yz, vertex__x_yz);
		drawOneSurface(wallTextureId_4, vertex__xy_z, vertex_xy_z, vertex_xyz, vertex__xyz);

		for (int i=0; i<furnitures.size(); i++) {
			glCallList(furnitures[i].listIndex);
		}

		glPopMatrix();
	}

	void drawOneSurface(GLuint textureId,
						glm::vec3 vertex_1,
						glm::vec3 vertex_2,
						glm::vec3 vertex_3,
						glm::vec3 vertex_4) {
		glPushMatrix();

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(vertex_1.x, vertex_1.y, vertex_1.z);


			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(vertex_2.x, vertex_2.y, vertex_2.z);


			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(vertex_3.x, vertex_3.y, vertex_3.z);


			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(vertex_4.x, vertex_4.y, vertex_4.z);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glPopMatrix();
	}
};

#endif // Environment.h
