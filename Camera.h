#ifndef _Camera_h
#define _Camera_h

#include "Board.h"

extern bool needTurn;

class Camera {
public:
    GLfloat tx;
    GLfloat ty;
    GLfloat tz;
    GLfloat rx;
    GLfloat ry;
    GLfloat rz;
    GLfloat centerX;
    GLfloat centerY;
    GLfloat centerZ;

    int judgewin = 0;
    bool isblackturn = false;

    Camera();
    ~Camera();
    
    void initAI();
    void update(Board *board,bool single);
    void keyboard(unsigned char key, Board *board);
    void specialkey(unsigned char key, Board *board);
    int mouseClick(int button, int state, int x, int y, Board *board, bool single);
    void mouseMove(int x, int y, Board *board);
    void reshape(GLint w, GLint h);
    void judge( Board *board, bool single);

private:
    GLfloat prevX;
    GLfloat prevY;
    GLfloat prevZ;

    glm::vec3 cameraPos;        //juedui zuobiao
    glm::vec3 cameraTarget;     //mianxiang fangxiang
    glm::vec3 headDir;          //touding fangxiang
    GLfloat sphereR;
    GLfloat sphereTheta;
    GLfloat spherePhi;
    bool needJudge=true;
    bool leftKey;
    bool rightKey;

    GLint w;
    GLint h;
    bool isselected = false;
    int newx, newy;
    int selx = -1;
    int sely = -1;
    int mouseClickEvent(int x, int y, Board *board, bool single);
    
    bool isValid(GLfloat, GLfloat, GLfloat, Board *board);
    void absolte2Sphere();
    bool sphere2cameraPos();
    
};

#endif // _Camera_h

