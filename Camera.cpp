#include "Camera.h"
#include "ChessAI/playchess.h"
#include "Piece.h"
#include "Board.h"
#include <iostream>
#define PI 3.14159

extern bool isSingle;

Camera::Camera() :  cameraPos(0.0f,0.0f,100.0f),cameraTarget(0.0f,0.0f,0.0f),headDir(1.0f,0.0f,0.0f),
    tx(0.0f), ty(0.0f), tz(0.0f),rx(0.0f), ry(0.0f), rz(0.0f),prevX(0.0f), prevY(0.0f), prevZ(0.0f),
    centerX(0.0f), centerY(0.0f),centerZ(-500.0) {
    leftKey = rightKey = false;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1f, 5000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 100.0f, centerX, centerY, centerZ, -1.0f, 0.0f, 0.0f);
    glOrtho(-1000.f, 1000.0f, -1000.0f, 1000.0f, -1000.0f, 1000.0f);

    glTranslatef(tx, ty, tz);
    glRotatef(rx, 1.0f, 0.0f, 0.0f);
    glRotatef(ry, 0.0f, 1.0f, 0.0f);
    glRotatef(rz, 0.0f, 0.0f, 1.0f);
}

Camera::~Camera() {
    
}

void Camera::initAI(){
    char userinput[5];
    getAIMove(userinput,true);
}

void Camera::update(Board *board,bool single) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1f, 5000.0f);
    
    /* set the eyes Position which is fixed */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0f, 0.0f, 100.0f, centerX, centerY, centerZ, -1.0f, 0.0f, 0.0f);
    
    if (board->movePieceDone()!=true){
        judgewin = board->movePiece(board->preX,board->preY,board->targetX,board->targetY);
        if (board->movePieceDone()==true) {
            if (!single)
                judge(board,single);
            else {
                if (needJudge) {
                    judge(board,single);
                    needJudge=false;
                }
                else {
                    needJudge=true;
                }
            }
            board->initPassibleBlock();
            isselected = false;
        }
    }
    
    glTranslatef(tx, ty, tz);
    glRotatef(rx, 1.0f, 0.0f, 0.0f);
    glRotatef(ry, 0.0f, 1.0f, 0.0f);
    glRotatef(rz, 0.0f, 0.0f, 1.0f);
}


void Camera::keyboard(unsigned char key, Board *board) {
    glm::vec3 cameraDir = glm::normalize(cameraTarget - cameraPos);
    glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(headDir), glm::vec3(cameraDir)));
    switch (key) {
        case 'W':
        case 'w':
        {
            glm::vec4 vec(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, -3.14f/180, glm::vec3(0.0, 1.0, 0.0));
            vec = trans * vec;
            glm::vec4 vec1(headDir.x, headDir.y, headDir.z, 1.0f);
            vec1 = trans * vec1;
            if (isValid(vec.x,vec.y,vec.z,board))
            {
                cameraPos.x=vec.x;
                cameraPos.y=vec.y;
                cameraPos.z=vec.z;
                headDir.x=vec1.x;
                headDir.y=vec1.y;
                headDir.z=vec1.z;
                headDir = glm::normalize(headDir);
                ry -= 1.0f;
            }
            break;
        }
        case 'D':
        case 'd':
        {
            glm::vec4 vec(cameraPos.x, cameraPos.y, cameraPos.z,1.0f);
            glm::vec4 vec2(cameraTarget.x, cameraTarget.y, cameraTarget.z,1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, 3.14f/180, headDir);
            vec = trans * vec;
            vec2 = trans * vec2;
            if (isValid(vec.x,vec.y,vec.z,board))
            {
                cameraPos.x=vec.x;
                cameraPos.y=vec.y;
                cameraPos.z=vec.z;
                cameraTarget.x=vec2.x;
                cameraTarget.y=vec2.y;
                cameraTarget.z=vec2.z;
                rx += 1.0f;
            }
            break;
        }
        case 'S':
        case 's':
        {
            glm::vec4 vec(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, 3.14f/180, glm::vec3(0.0, 1.0, 0.0));
            vec = trans * vec;
            glm::vec4 vec1(headDir.x, headDir.y, headDir.z, 1.0f);
            vec1 = trans * vec1;
            if (isValid(vec.x,vec.y,vec.z,board))
            {
                cameraPos.x=vec.x;
                cameraPos.y=vec.y;
                cameraPos.z=vec.z;
                headDir.x=vec1.x;
                headDir.y=vec1.y;
                headDir.z=vec1.z;
                headDir = glm::normalize(headDir);
                ry += 1.0f;
            }
            break;
        }
        case 'A':
        case 'a':
        {
            glm::vec4 vec(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
            glm::vec4 vec2(cameraTarget.x, cameraTarget.y, cameraTarget.z,1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, -3.14f/180, headDir);
            vec = trans * vec;
            vec2 = trans * vec2;
            if (isValid(vec.x,vec.y,vec.z,board))
            {
                cameraPos.x=vec.x;
                cameraPos.y=vec.y;
                cameraPos.z=vec.z;
                cameraTarget.x=vec2.x;
                cameraTarget.y=vec2.y;
                cameraTarget.z=vec2.z;
                rx -= 1.0f;
            }
            break;
        }
        
        case 'U':
        case 'u':
        {
            glm::vec4 vec(centerX, centerY, centerZ, 1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, 3.14f/90, glm::vec3(0.0, 0.0, 1.0));
            vec = trans * vec;
            centerX=vec.x;
            centerY=vec.y;
            centerZ=vec.z;
            break;
        }
        case 'O':
        case 'o':
        {
            glm::vec4 vec(centerX, centerY, centerZ, 1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, -3.14f/90, glm::vec3(0.0, 0.0, 1.0));
            vec = trans * vec;
            centerX=vec.x;
            centerY=vec.y;
            centerZ=vec.z;
            break;
        }
        case 'J':
        case 'j':
        {
            glm::vec4 vec(centerX, centerY, centerZ, 1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, -3.14f/90, glm::vec3(1.0, 0.0, 0.0));
            vec = trans * vec;
            centerX=vec.x;
            centerY=vec.y;
            centerZ=vec.z;
            break;
        }
        case 'I':
        case 'i':
        {
            glTranslatef(cameraPos.x,cameraPos.y,cameraPos.z);
            glm::vec4 vec(centerX, centerY, centerZ, 1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, 3.14f/90, glm::vec3(0.0, 1.0, 0.0));
            vec = trans * vec;
            centerX=vec.x;
            centerY=vec.y;
            centerZ=vec.z;
            break;
        }
        case 'L':
        case 'l':
        {
            glm::vec4 vec(centerX, centerY, centerZ, 1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, 3.14f/90, glm::vec3(1.0, 0.0, 0.0));
            vec = trans * vec;
            centerX=vec.x;
            centerY=vec.y;
            centerZ=vec.z;
            break;
        }
        case 'K':
        case 'k':
        {
            glm::vec4 vec(centerX, centerY, centerZ, 1.0f);
            glm::mat4 trans;
            trans = glm::rotate(trans, -3.14f/90, glm::vec3(0.0, 1.0, 0.0));
            vec = trans * vec;
            centerX=vec.x;
            centerY=vec.y;
            centerZ=vec.z;
            break;
        }
        case 'X':
        case 'x':
        {
            tx = ty = tz = 0.0f;
            rx = ry = rz = 0.0f;
            centerX = centerY = 0.0f;
            centerZ = -500.0f;
            cameraPos.x = cameraPos.y = 0.0f;
            cameraPos.z = 100.0f;
            cameraTarget.x = cameraTarget.y = cameraTarget.z = 0.0f;
            headDir.x=1.0f;
            headDir.y = headDir.z = 0.0f;
            break;
        }
        default:
            break;
    }
    // std::cout<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<std::endl;
}

void Camera::specialkey(unsigned char key, Board *board) {
    glm::vec3 cameraDir = glm::normalize(cameraTarget - cameraPos);
    glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(headDir), glm::vec3(cameraDir)));
    switch (key) {
        case GLUT_KEY_LEFT:
        {
            cameraPos += cameraRight;
            cameraTarget += cameraRight;
            if (isValid(cameraPos.x,cameraPos.y,cameraPos.z,board))
                ty += 1.0f;
            else
            {
                 cameraPos -= cameraRight;
                 cameraTarget -= cameraRight;
            }
            
            break;
        }
        case GLUT_KEY_UP:
        {
            cameraPos += cameraDir; 
            if (isValid(cameraPos.x,cameraPos.y,cameraPos.z,board))
                tz += 1.0f;
            else
                cameraPos -= cameraDir; 
            
            break;
        }
        case GLUT_KEY_RIGHT:
        {
            cameraPos -= cameraRight;
            cameraTarget -= cameraRight;
            if (isValid(cameraPos.x,cameraPos.y,cameraPos.z,board))
                ty -= 1.0f;
            else
            {
                 cameraPos += cameraRight;
                 cameraTarget += cameraRight;
            }
            break;
        }
        case GLUT_KEY_DOWN:
        {
           cameraPos -= cameraDir; 
            if (isValid(cameraPos.x,cameraPos.y,cameraPos.z,board))
                tz -= 1.0f;
            else
                cameraPos += cameraDir; 
            
            break;
        }
    }
    // std::cout<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<std::endl;
}

int Camera::mouseClick(int button, int state, int x, int y, Board *board, bool single)
{
    switch (button)
    {
        case 0:
            if(state == GLUT_DOWN && board->movePieceDone())
                return mouseClickEvent(x, y, board, single);
            break;
        case 1:
            break;
        case 2:
            rightKey = !state;
            break;
        default:
            break;
    }
    
    prevX = x;
    prevY = y;
    
    return 0;
}

void Camera::mouseMove(int x, int y, Board *board) {
    if (rightKey) {
        glm::vec4 vec(cameraPos.x, cameraPos.y, cameraPos.z, 1.0f);
        glm::vec4 vec1(headDir.x, headDir.y, headDir.z, 1.0f);
        glm::vec4 vec2(cameraTarget.x, cameraTarget.y, cameraTarget.z,1.0f);
        glm::mat4 trans;
        trans = glm::rotate(trans, -3.14f/180*((prevY - y) / 5), glm::vec3(0.0, 1.0, 0.0));
        vec1 = trans * vec1;
        trans = glm::rotate(trans, 3.14f/180*((prevX - x) / 5), glm::vec3(vec1.x,vec1.y,vec1.z));
        vec = trans * vec;
        vec2 = trans * vec2;
        if (isValid(vec.x,vec.y,vec.z,board))
        {
            cameraPos.x=vec.x;
            cameraPos.y=vec.y;
            cameraPos.z=vec.z;
            headDir.x=vec1.x;
            headDir.y=vec1.y;
            headDir.z=vec1.z;
            headDir = glm::normalize(headDir);
            cameraTarget.x=vec2.x;
            cameraTarget.y=vec2.y;
            cameraTarget.z=vec2.z;
            ry -= (prevY - y) / 5;
            rx -= -(prevX - x) / 5;
        }
    }
    prevX = x;
    prevY = y;
     // std::cout<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<std::endl;
}

void Camera::reshape(GLint w, GLint h){
    this->w = w;
    this->h = h;
}

int Camera::mouseClickEvent(int x, int y, Board *board, bool single){
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
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1f, 10000.0f);     // 指定视景体
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 100.0f, centerX, centerY, centerZ, -1.0f, 0.0f, 0.0f);
    glTranslatef(tx, ty, tz);
    glRotatef(rx, 1.0f, 0.0f, 0.0f);
    glRotatef(ry, 0.0f, 1.0f, 0.0f);
    glRotatef(rz, 0.0f, 0.0f, 1.0f);
    board->drawBoard(GL_SELECT);
    glPopMatrix();
    
    hits = glRenderMode(GL_RENDER);
    
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1f, 5000.0f);
    
    if(hits && judgewin == 0){
        
        newx = selectBuf[3]/8;
        newy = selectBuf[3]%8;
        if(isselected == false){
            board->getPassibleBlock(isblackturn, newx, newy);
            selx = newx;
            sely = newy;
            isselected = true;
        }
        if(isselected == true){
            if(board->grid[newx][newy].passibleBlock == true){
                judgewin = board->movePiece(selx, sely, newx, newy);
                board->initPassibleBlock();
                isselected = false;
            }
            else{
                board->initPassibleBlock();
                board->getPassibleBlock(isblackturn,newx,newy);
                selx = newx;
                sely = newy;
                isselected = true;
            }
            if(judgewin == -1){
                printf("Black Wins!\n");
            }
            if(judgewin == 1){
                printf("White Wins!\n");
            }
        }
        return judgewin;
    }
    
    return 0;
}

//wlc
void Camera::judge( Board *board, bool single)
{
    static int i = 0;
    if(!single){
        isblackturn = !isblackturn;
        needTurn = !needTurn;
    }
    else{
        char userinput[5];
        int blackinput;
        userinput[0] = sely + 'a';
        userinput[1] = 7 - selx + '1';
        userinput[2] = newy + 'a';
        userinput[3] = 7 - newx + '1';
        userinput[4] = '\0';
        blackinput = 6464 - getAIMove(userinput, false);
        selx = (blackinput/100-1)/8;
        sely = (8 - blackinput/100%8)%8;
        newx = (blackinput%100-1)/8;
        newy = (8 - blackinput%100%8)%8;
        judgewin = board->movePiece(selx, sely, newx, newy);
    }
}

bool Camera::isValid(GLfloat x, GLfloat y , GLfloat z,Board *board){
    //return true;
    if (z<=-95 || z>=300 || x<=-500 || x>=500 || y<=-500 || y>=500 ) return false;
    if (z>=-15 && z<=2 && sqrt(x*x+y*y)<=80.0) return false;  //table
    if (abs(x)<=40 && abs(y)<=40 && z<=8 && z>=-15) return false;
    if (z<=130 && sqrt((x-100)*(x-100)+y*y)<=30.0) return false;  //table
    if (abs(x)<32 && abs(y)<32){
        int indexX = ceil(x/8.0)+3;
        int indexY = ceil(y/8.0)+3;
        
        switch (board->grid[indexX][indexY].type)
        {
            case 0: //KING = 0,
                if (z<=30) return false;
                break;
            case 1://QUEEN,
                if (z<=30) return false;
                break;
            case 2:// BISHOP,
                if (z<=25) return false;
                break;
            case 3://KNIGHT,
                if (z<=25) return false;
                break;
            case 4://FORTRESS,
                if (z<=25) return false;
                break;
            case 5://PAWN,
                if (z<=25) return false;
                break;
            case 6://EMPTY
                return true;
                break;
            default:
                break;
        }
    }
    return true;
}
