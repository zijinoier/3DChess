#include "Piece.h"


Piece::Piece(const glm::vec3 pos, const glm::vec3 rot, const PieceType type, const bool isBlack,
             ObjLoader *pObjLoader, ObjLoader *pBlackQueenObjLoader, ObjLoader *pWhiteQueenObjLoader) {
    this->position = pos;
    this->rotation = rot;
    this->p_type = type;
    this->isBlack = isBlack;
    this->pObjLoader = pObjLoader;
    
    this->pBlackQueenObjLoader = pBlackQueenObjLoader;
    this->pWhiteQueenObjLoader = pWhiteQueenObjLoader;
}

void Piece::draw() {
    if (isValid) {
        glPushMatrix();
        
        glTranslatef(this->position.x, this->position.y, this->position.z);
        glRotatef(this->rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(this->rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(this->rotation.z, 0.0f, 0.0f, 1.0f);
        glScalef(2.0f, 2.0f, 2.0f);
        
        glCallList(this->pObjLoader->listIndex);
        
        glPopMatrix();
    }
}

bool Piece::move(double x, double y, double z){

    position = glm::vec3(x*8-28, y*8-28, z);
    if(p_type == PAWN) {
        if(isBlack == true && x == 7.0){
            pObjLoader = pBlackQueenObjLoader;
        }
        if(isBlack == false && x == 0.0){
            pObjLoader = pWhiteQueenObjLoader;
        }
    }
    return true;
}

