//
//  Menu.hpp
//  3D_chess
//
//  Created by Sr on 2018/1/4.
//  Copyright © 2018年 Sr. All rights reserved.
//

#ifndef Menu_hpp
#define Menu_hpp

#include <stdio.h>
#include "Camera.h"

class MenuBoard{
private:
    int x;
    int y;
    int height;
    int width;
    GLuint name;
    GLuint textureId;
    GLuint loadTexture(char *imageFilePath);
public:
    MenuBoard(){};
    MenuBoard(int x, int y, int height, int width, GLuint name);
    
    void drawMenuBoard(GLenum mode);
};

class Menu {
private:
    MenuBoard startTitle;
    MenuBoard singlePlayerBtn;
    MenuBoard twoPlayerBtn;
    
    MenuBoard pauseTitle;
    MenuBoard backBtn;
    MenuBoard backMenuBtn;
    MenuBoard newGameBtn;
    
    MenuBoard blackWinTitle;
    MenuBoard whiteWinTitle;
    
    GLint w;
    GLint h;
    
public:
    Menu();
    void drawStartMenu(GLenum mode, Camera *camera);
    void drawPauseMenu(GLenum mode, Camera *camera);
    void drawBlackWinsMenu(GLenum mode, Camera *camera);
    void drawWhiteWinsMenu(GLenum mode, Camera *camera);
    void drawBlankBoard(Camera *camera);
    int mouseClickMenuEvent(int x, int y, Camera *camera, bool isStart);
    void reshape(GLint w, GLint h);
};

#endif /* Menu_hpp */
