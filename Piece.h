#ifndef _Piece_h
#define _Piece_h

#include <stdio.h>
#include <string>
#include <math.h>
#include <vector>
#include <GL/glut.h>
#include "ObjLoader.h"
#define BLOCK_LENGTH 8
#define BLOCK_WIDTH 8
#define BLOCK_HEIGHT 8

enum PieceType {
    KING = 0,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK,
    PAWN,
    EMPTY
};

class Piece {
public:
    bool isBlack;
    bool isValid = true;
    
    Piece() {};
    Piece(const glm::vec3 pos, const glm::vec3 rot, const PieceType type, const bool isBlack,
          ObjLoader *pObjLoader, ObjLoader *pBlackQueenObjLoader, ObjLoader *pWhiteQueenObjLoader);
    
    void draw();
    bool move(double x, double y, double z);
    
private:
    ObjLoader *pObjLoader;
    ObjLoader *pWhiteQueenObjLoader;
    ObjLoader *pBlackQueenObjLoader;
    glm::vec3 position;
    glm::vec3 rotation;
    PieceType p_type;
};

#endif /* Piece_h */

