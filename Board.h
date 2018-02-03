#ifndef _Board_h
#define _Board_h

#include "Piece.h"

extern bool needTurn;

class Block{
private:
public:
    GLuint name;
    PieceType type;
    int pieceIndex;
    bool isBlack;
    bool passibleBlock = false;
    
    Block(){};
    void drawBlock(int row, int col, GLenum mode);
    Block(PieceType type, bool isblack, int pieceIndex);
};

class Board{
private:
    ObjLoader *pObjLoader;
    glm::vec3 position;
    glm::vec3 rotation;
    bool moveDone=true;

    
public:
    Block grid[8][8];
    int preX=0,preY=0,targetX=0,targetY=0,targetZ=0;
    double nowX=0,nowY=0,nowZ=8;
    int movePhase=0;
    bool movePieceDone() {return moveDone;}
    Board(){};
    Board(const glm::vec3 pos, const glm::vec3 rot, ObjLoader *pObjLoader);
    
    void drawBoard(GLenum mode);
    void drawBottom();
    
    void getPassibleBlock(bool isblackturn, int row, int col);
    int movePiece(int oldx, int oldy, int newx, int newy);
    void initPassibleBlock();
};

extern std::vector<Piece> pieces;

#endif /* Board_h */

