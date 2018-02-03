//
//  Menu.cpp
//  3D_chess
//
//  Created by Sr on 2018/1/4.
//  Copyright © 2018年 Sr. All rights reserved.
//

#include "Menu.h"

MenuBoard::MenuBoard(int x, int y, int height, int width, GLuint name){
    switch (name) {
        case 80:
            loadTexture("./Chess/button/startTitle.png");
            break;
        case 81:
            loadTexture("./Chess/button/singlePlayerBtn.png");
            break;
        case 82:
            loadTexture("./Chess/button/twoPlayerBtn.png");
            break;
        case 83:
            loadTexture("./Chess/button/pauseTitle.png");
            break;
        case 84:
            loadTexture("./Chess/button/backBtn.png");
            break;
        case 85:
            loadTexture("./Chess/button/backMenuBtn.png");
            break;
        case 86:
            loadTexture("./Chess/button/newGameBtn.png");
            break;
        case 87:
            loadTexture("./Chess/button/blackWinsTitle.png");
            break;
        case 88:
            loadTexture("./Chess/button/whiteWinsTitle.png");
            break;
        default:
            break;
    }
    this->x = x;
    this->y = y;
    this->height = height;
    this->width = width;
    this->name = name;
}

GLuint MenuBoard::loadTexture(char *imageFilePath) {
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
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
    
    return textureId;
}

void MenuBoard::drawMenuBoard(GLenum mode){
    GLfloat color = 1;
    
    if(mode == GL_SELECT)
    {
        glLoadName(name);
    }
    
    double p[4][3];
    p[0][0] = x;
    p[0][1] = y;
    p[0][2] = 0.01;
    
    p[1][0] = x+height;
    p[1][1] = y;
    p[1][2] = 0.01;
    
    p[2][0] = x+height;
    p[2][1] = y+width;
    p[2][2] = 0.01;
    
    p[3][0] = x;
    p[3][1] = y+width;
    p[3][2] = 0.01;
    
    glPushMatrix();

    glClear(GL_COLOR_MATRIX);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0.05f, 0.62f);
    glVertex3f( p[0][0], p[0][1], p[0][2]);
    glTexCoord2f(0.05f, 0.38f);
    glVertex3f( p[1][0], p[1][1], p[1][2]);
    glTexCoord2f(0.95f, 0.38f);
    glVertex3f( p[2][0], p[2][1], p[2][2]);
    glTexCoord2f(0.95f, 0.62f);
    glVertex3f( p[3][0], p[3][1], p[3][2]);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    
    glPopMatrix();
    
}

Menu::Menu(){
    startTitle = MenuBoard(-50, -50, 20, 100, 80);
    singlePlayerBtn = MenuBoard(-20, -50, 20, 100, 81);
    twoPlayerBtn = MenuBoard(10, -50, 20, 100, 82);
    
    pauseTitle = MenuBoard(-50, -50, 20, 100, 83);
    backMenuBtn = MenuBoard(-20, -50, 20, 100, 85);
    newGameBtn = MenuBoard(10, -50, 20, 100, 86);
    backBtn = MenuBoard(40, -50, 20, 100, 84);
    
    blackWinTitle = MenuBoard(-50, -50, 20, 100, 87);
    whiteWinTitle = MenuBoard(-50, -50, 20, 100, 88);
}

void Menu::drawStartMenu(GLenum mode, Camera *camera){
    glPushMatrix();
    glRotatef(-camera->rz, 0.0f, 0.0f, 1.0f);
    glRotatef(-camera->ry, 0.0f, 1.0f, 0.0f);
    glRotatef(-camera->rx, 1.0f, 0.0f, 0.0f);
    glTranslatef(-camera->tx, -camera->ty, -camera->tz);
    
    glDisable(GL_DEPTH_TEST);
    startTitle.drawMenuBoard(mode);
    singlePlayerBtn.drawMenuBoard(mode);
    twoPlayerBtn.drawMenuBoard(mode);
    glEnable(GL_DEPTH_TEST);
    
    glPopMatrix();
}

void Menu::drawPauseMenu(GLenum mode, Camera *camera){
    glPushMatrix();
    glRotatef(-camera->rz, 0.0f, 0.0f, 1.0f);
    glRotatef(-camera->ry, 0.0f, 1.0f, 0.0f);
    glRotatef(-camera->rx, 1.0f, 0.0f, 0.0f);
    glTranslatef(-camera->tx, -camera->ty, -camera->tz);
    glDisable(GL_DEPTH_TEST);
    pauseTitle.drawMenuBoard(mode);
    backBtn.drawMenuBoard(mode);
    backMenuBtn.drawMenuBoard(mode);
    newGameBtn.drawMenuBoard(mode);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

void Menu::drawBlackWinsMenu(GLenum mode, Camera *camera){
    glPushMatrix();
    glRotatef(-camera->rz, 0.0f, 0.0f, 1.0f);
    glRotatef(-camera->ry, 0.0f, 1.0f, 0.0f);
    glRotatef(-camera->rx, 1.0f, 0.0f, 0.0f);
    glTranslatef(-camera->tx, -camera->ty, -camera->tz);
    glDisable(GL_DEPTH_TEST);
    blackWinTitle.drawMenuBoard(mode);
    backMenuBtn.drawMenuBoard(mode);
    newGameBtn.drawMenuBoard(mode);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

void Menu::drawWhiteWinsMenu(GLenum mode, Camera *camera){
    glPushMatrix();
    glRotatef(-camera->rz, 0.0f, 0.0f, 1.0f);
    glRotatef(-camera->ry, 0.0f, 1.0f, 0.0f);
    glRotatef(-camera->rx, 1.0f, 0.0f, 0.0f);
    glTranslatef(-camera->tx, -camera->ty, -camera->tz);
    glDisable(GL_DEPTH_TEST);
    whiteWinTitle.drawMenuBoard(mode);
    backMenuBtn.drawMenuBoard(mode);
    newGameBtn.drawMenuBoard(mode);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}

int Menu::mouseClickMenuEvent(int x, int y, Camera *camera, bool isStart){
    const int BUFSIZE = 64;
    GLuint selectBuf[BUFSIZE];
    GLint hits;
    GLint viewport[4];
    
    glGetIntegerv (GL_VIEWPORT, viewport);
    glSelectBuffer (BUFSIZE, selectBuf);
    
    glRenderMode(GL_SELECT);
    glInitNames();
    glPushName(-1);
    
    glPushMatrix();
    glMatrixMode (GL_PROJECTION);  // 1: GL_SELECT下投影变换
    glLoadIdentity ();
    
    gluPickMatrix((GLdouble)x, (GLdouble) (viewport[3]-y), 5.0, 5.0, viewport); // 指定选取窗口
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1f, 800.0f);     // 指定视景体
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 100.0f, camera->centerX, camera->centerY, camera->centerZ, -1.0f, 0.0f, 0.0f);
    glTranslatef(camera->tx, camera->ty, camera->tz);
    glRotatef(camera->rx, 1.0f, 0.0f, 0.0f);
    glRotatef(camera->ry, 0.0f, 1.0f, 0.0f);
    glRotatef(camera->rz, 0.0f, 0.0f, 1.0f);
    if(isStart)
        drawStartMenu(GL_SELECT, camera);
    else
        drawPauseMenu(GL_SELECT, camera);
    glPopMatrix();
    
    hits = glRenderMode(GL_RENDER);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1f, 800.0f);
    
    if(hits){
        return selectBuf[3];
    }
    
    return 0;
}

void Menu::drawBlankBoard(Camera *camera){
    glPushMatrix();
    glRotatef(-camera->rz, 0.0f, 0.0f, 1.0f);
    glRotatef(-camera->ry, 0.0f, 1.0f, 0.0f);
    glRotatef(-camera->rx, 1.0f, 0.0f, 0.0f);
    glTranslatef(-camera->tx, -camera->ty, -camera->tz);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5);
    glBegin(GL_QUADS);  //绘制四边形
    glVertex3f(-70, -70, 0);
    glVertex3f(70, -70, 0);
    glVertex3f(70, 70, 0);
    glVertex3f(-70, 70, 0);
    glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
}


void Menu::reshape(GLint w, GLint h){
    this->w = w;
    this->h = h;
}
