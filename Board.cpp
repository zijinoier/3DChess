#include "Board.h"
#include "Particle.h"
#include <iostream>

Block::Block(PieceType type, bool isblack, int pieceIndex){
    this->type = type;
    this->isBlack = isblack;
    this->passibleBlock = false;
    this->pieceIndex = pieceIndex;
}

void Block::drawBlock(int row, int col, GLenum mode){
    double x = (row-4)*BLOCK_WIDTH;
    double y = (col-4)*BLOCK_LENGTH;
    
    GLfloat color = (row+col)%2 ? 0.2:1;
    
    if(mode == GL_SELECT)
    {
        glLoadName(name);
    }
    
    //每个block的8个顶点
    double p[8][3];
    p[0][0] = x;
    p[0][1] = y;
    p[0][2] = 0.01;
    
    p[1][0] = x+BLOCK_WIDTH;
    p[1][1] = y;
    p[1][2] = 0.01;
    
    p[2][0] = x;
    p[2][1] = y+BLOCK_LENGTH;
    p[2][2] = 0.01;
    
    p[3][0] = x+BLOCK_WIDTH;
    p[3][1] = y+BLOCK_LENGTH;
    p[3][2] = 0.01;
    
    p[4][0] = x;
    p[4][1] = y;
    p[4][2] = 0.01+BLOCK_HEIGHT;
    
    p[5][0] = x+BLOCK_WIDTH;
    p[5][1] = y;
    p[5][2] = 0.01+BLOCK_HEIGHT;
    
    p[6][0] = x;
    p[6][1] = y+BLOCK_LENGTH;
    p[6][2] = 0.01+BLOCK_HEIGHT;
    
    p[7][0] = x+BLOCK_WIDTH;
    p[7][1] = y+BLOCK_LENGTH;
    p[7][2] = 0.01+BLOCK_HEIGHT;
    
    if(passibleBlock == true){
        //glColor4f(0.5, (double)color/10, (double)color/10, 1.0);
        glColor3f(1.0, 0.0, 0.0);
        
        glBegin(GL_QUADS);
        
        glVertex3f( p[4][0], p[4][1], p[4][2]);
        glVertex3f( p[5][0], p[5][1], p[5][2]);
        glVertex3f( p[7][0], p[7][1], p[7][2]);
        glVertex3f( p[6][0], p[6][1], p[6][2]);
        glEnd();
    }
    else{
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glColor4f(color,color,color,1.0);
        
        glBegin(GL_QUADS);
        
        glVertex3f( p[4][0], p[4][1], p[4][2]);
        glVertex3f( p[5][0], p[5][1], p[5][2]);
        glVertex3f( p[7][0], p[7][1], p[7][2]);
        glVertex3f( p[6][0], p[6][1], p[6][2]);
        glEnd();
        
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
    }
}

Board::Board(const glm::vec3 pos, const glm::vec3 rot, ObjLoader *pObjLoader){
    grid[0][0] = Block(ROOK, true, 0);
    grid[0][1] = Block(KNIGHT, true, 1);
    grid[0][2] = Block(BISHOP, true, 2);
    grid[0][3] = Block(QUEEN, true, 3);
    grid[0][4] = Block(KING, true, 4);
    grid[0][5] = Block(BISHOP, true, 5);
    grid[0][6] = Block(KNIGHT, true, 6);
    grid[0][7] = Block(ROOK, true, 7);
    for(int i=0; i<8; i++){
        grid[1][i] = Block(PAWN, true, 8 + i);
        grid[6][i] = Block(PAWN, false, 24 + i);
        for(int j=2; j<6; j++){
            grid[j][i] = Block(EMPTY, false, -1);
        }
    }
    grid[7][0] = Block(ROOK, false, 16);
    grid[7][1] = Block(KNIGHT, false, 17);
    grid[7][2] = Block(BISHOP, false, 18);
    grid[7][3] = Block(QUEEN, false, 19);
    grid[7][4] = Block(KING, false, 20);
    grid[7][5] = Block(BISHOP, false, 21);
    grid[7][6] = Block(KNIGHT, false, 22);
    grid[7][7] = Block(ROOK, false, 23);
    
    this->pObjLoader = pObjLoader;
    this->position = pos;
    this->rotation = rot;
}

void Board::drawBottom()
{
    glPushMatrix();
    
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);

    glScaled(5.1, 20.0, 5.1);
    
    for (int j = 0; j < pObjLoader->Materials.size(); j++) {
        
        // set material
        float Ka[3] = { pObjLoader->Materials[j].Ka.x,
            pObjLoader->Materials[j].Ka.y,
            pObjLoader->Materials[j].Ka.z };
        float Kd[3] = { pObjLoader->Materials[j].Kd.x,
            pObjLoader->Materials[j].Kd.y,
            pObjLoader->Materials[j].Kd.z };
        float Ks[3] = { pObjLoader->Materials[j].Ks.x,
            pObjLoader->Materials[j].Ks.y,
            pObjLoader->Materials[j].Ks.z };
        float Ns = pObjLoader->Materials[j].Ns;
        
        glEdgeFlag(GL_TRUE);
        glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);      // 使环境材质颜色追踪当前颜色
        glEnable(GL_COLOR_MATERIAL);                // 启用材质颜色追踪当前颜色
        
        glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, Kd);
        glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);
        glMaterialf(GL_FRONT, GL_SHININESS, Ns);
        
        glColor3f(1.0f, 1.0f, 1.0f);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        for (int i = 0; i < pObjLoader->Materials[j].Meshes.size(); i++) {
            Vertex vertex_1 = pObjLoader->Materials[j].Meshes[i].Vertices[0];
            Vertex vertex_2 = pObjLoader->Materials[j].Meshes[i].Vertices[1];
            Vertex vertex_3 = pObjLoader->Materials[j].Meshes[i].Vertices[2];
            
            if (pObjLoader->Materials[j].textureKdId != 0) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, pObjLoader->Materials[j].textureKdId);
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            }

            glBegin(GL_TRIANGLES);
            
            glNormal3f(vertex_1.Normal.x, vertex_1.Normal.y, vertex_1.Normal.z);
            glTexCoord2f(vertex_1.TexCoords.x, vertex_1.TexCoords.y);
            glVertex3f(vertex_1.Position.x, vertex_1.Position.y, vertex_1.Position.z);
            
            glNormal3f(vertex_2.Normal.x, vertex_2.Normal.y, vertex_2.Normal.z);
            glTexCoord2f(vertex_2.TexCoords.x, vertex_2.TexCoords.y);
            glVertex3f(vertex_2.Position.x, vertex_2.Position.y, vertex_2.Position.z);
            
            glNormal3f(vertex_3.Normal.x, vertex_3.Normal.y, vertex_3.Normal.z);
            glTexCoord2f(vertex_3.TexCoords.x, vertex_3.TexCoords.y);
            glVertex3f(vertex_3.Position.x, vertex_3.Position.y, vertex_3.Position.z);
            
            glEnd();
            
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_COLOR_MATERIAL);
        }
    }
    
    glPopMatrix();
}

void Board::drawBoard(GLenum mode){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            if (needTurn) {
                grid[i][j].name = (7-i)*8+(7-j);
            }
            else {
                grid[i][j].name = i*8+j;
            }
            grid[i][j].drawBlock(i,j,mode);
        }
    }
}

void Board::getPassibleBlock(bool isblackturn, int row, int col){
    Block current_block = grid[row][col];
    int stop[8] = {0};
    if(current_block.type == EMPTY){
        return;
    }
    if(current_block.type != EMPTY && current_block.isBlack !=  isblackturn){
        return;
    }
    switch (current_block.type) {
        case KING:
            for(int i=row-1; i<=row+1; i++){
                for(int j=col-1; j<=col+1; j++){
                    if(i>=0 && i<8 && j>=0 && j<8 ){
                        if(grid[i][j].type!=EMPTY && grid[i][j].isBlack != current_block.isBlack)
                            grid[i][j].passibleBlock = true;
                        if(grid[i][j].type == EMPTY)
                            grid[i][j].passibleBlock = true;
                    }
                }
            }
            break;

        case QUEEN:
            for(int i=0; i<8; i++){
                stop[i] = 0;
            }
            for(int i=1; i<7; i++){
                if(stop[0] == 0 && row-i>=0 && row-i<8 && col-i>=0 && col-i<8){
                    if(grid[row-i][col-i].type!=EMPTY){
                        if(grid[row-i][col-i].isBlack != current_block.isBlack)
                            grid[row-i][col-i].passibleBlock = true;
                        stop[0]=1;
                    }
                    else{
                        grid[row-i][col-i].passibleBlock = true;
                    }
                }
                if(stop[1] == 0 && row-i>=0 && row-i<8 && col+i>=0 && col+i<8){
                    if(grid[row-i][col+i].type!=EMPTY){
                        if(grid[row-i][col+i].isBlack != current_block.isBlack)
                            grid[row-i][col+i].passibleBlock = true;
                        stop[1]=1;
                    }
                    else{
                        grid[row-i][col+i].passibleBlock = true;
                    }
                }
                if(stop[2] == 0 && row+i>=0 && row+i<8 && col-i>=0 && col-i<8){
                    if(grid[row+i][col-i].type!=EMPTY){
                        if(grid[row+i][col-i].isBlack != current_block.isBlack)
                            grid[row+i][col-i].passibleBlock = true;
                        stop[2]=1;
                    }
                    else{
                        grid[row+i][col-i].passibleBlock = true;
                    }
                }
                if(stop[3] == 0 && row+i>=0 && row+i<8 && col+i>=0 && col+i<8){
                    if(grid[row+i][col+i].type!=EMPTY){
                        if(grid[row+i][col+i].isBlack != current_block.isBlack)
                            grid[row+i][col+i].passibleBlock = true;
                        stop[3]=1;
                    }
                    else{
                        grid[row+i][col+i].passibleBlock = true;
                    }
                }
                if(stop[4] == 0 && row-i>=0 && row-i<8 && col>=0 && col<8){
                    if(grid[row-i][col].type!=EMPTY){
                        if(grid[row-i][col].isBlack != current_block.isBlack)
                            grid[row-i][col].passibleBlock = true;
                        stop[4]=1;
                    }
                    else{
                        grid[row-i][col].passibleBlock = true;
                    }
                }
                if(stop[5] == 0 && row+i>=0 && row+i<8 && col>=0 && col<8){
                    if(grid[row+i][col].type!=EMPTY){
                        if(grid[row+i][col].isBlack != current_block.isBlack)
                            grid[row+i][col].passibleBlock = true;
                        stop[5]=1;
                    }
                    else{
                        grid[row+i][col].passibleBlock = true;
                    }
                }
                if(stop[6] == 0 && row>=0 && row<8 && col-i>=0 && col-i<8){
                    if(grid[row][col-i].type!=EMPTY){
                        if(grid[row][col-i].isBlack != current_block.isBlack)
                            grid[row][col-i].passibleBlock = true;
                        stop[6]=1;
                    }
                    else{
                        grid[row][col-i].passibleBlock = true;
                    }
                }
                if(stop[7] == 0 && row>=0 && row<8 && col+i>=0 && col+i<8){
                    if(grid[row][col+i].type!=EMPTY){
                        if(grid[row][col+i].isBlack != current_block.isBlack)
                            grid[row][col+i].passibleBlock = true;
                        stop[7]=1;
                    }
                    else{
                        grid[row][col+i].passibleBlock = true;
                    }
                }
            }
            
        case BISHOP:
            for(int i=0; i<4; i++){
                stop[i] = 0;
            }
            for(int i=1; i<7; i++){
                if(stop[0] == 0 && row-i>=0 && row-i<8 && col-i>=0 && col-i<8){
                    if(grid[row-i][col-i].type!=EMPTY){
                        if(grid[row-i][col-i].isBlack != current_block.isBlack)
                            grid[row-i][col-i].passibleBlock = true;
                        stop[0]=1;
                    }
                    else{
                        grid[row-i][col-i].passibleBlock = true;
                    }
                }
                if(stop[1] == 0 && row-i>=0 && row-i<8 && col+i>=0 && col+i<8){
                    if(grid[row-i][col+i].type!=EMPTY){
                        if(grid[row-i][col+i].isBlack != current_block.isBlack)
                            grid[row-i][col+i].passibleBlock = true;
                        stop[1]=1;
                    }
                    else{
                        grid[row-i][col+i].passibleBlock = true;
                    }
                }
                if(stop[2] == 0 && row+i>=0 && row+i<8 && col-i>=0 && col-i<8){
                    if(grid[row+i][col-i].type!=EMPTY){
                        if(grid[row+i][col-i].isBlack != current_block.isBlack)
                            grid[row+i][col-i].passibleBlock = true;
                        stop[2]=1;
                    }
                    else{
                        grid[row+i][col-i].passibleBlock = true;
                    }
                }
                if(stop[3] == 0 && row+i>=0 && row+i<8 && col+i>=0 && col+i<8){
                    if(grid[row+i][col+i].type!=EMPTY){
                        if(grid[row+i][col+i].isBlack != current_block.isBlack)
                            grid[row+i][col+i].passibleBlock = true;
                        stop[3]=1;
                    }
                    else{
                        grid[row+i][col+i].passibleBlock = true;
                    }
                }
            }
            break;
            
        case ROOK:
            for(int i=4; i<8; i++){
                stop[i] = 0;
            }
            for(int i=1; i<7; i++){if(stop[4] == 0 && row-i>=0 && row-i<8 && col>=0 && col<8){
                if(grid[row-i][col].type!=EMPTY){
                    if(grid[row-i][col].isBlack != current_block.isBlack)
                        grid[row-i][col].passibleBlock = true;
                    stop[4]=1;
                }
                else{
                    grid[row-i][col].passibleBlock = true;
                }
            }
                if(stop[5] == 0 && row+i>=0 && row+i<8 && col>=0 && col<8){
                    if(grid[row+i][col].type!=EMPTY){
                        if(grid[row+i][col].isBlack != current_block.isBlack)
                            grid[row+i][col].passibleBlock = true;
                        stop[5]=1;
                    }
                    else{
                        grid[row+i][col].passibleBlock = true;
                    }
                }
                if(stop[6] == 0 && row>=0 && row<8 && col-i>=0 && col-i<8){
                    if(grid[row][col-i].type!=EMPTY){
                        if(grid[row][col-i].isBlack != current_block.isBlack)
                            grid[row][col-i].passibleBlock = true;
                        stop[6]=1;
                    }
                    else{
                        grid[row][col-i].passibleBlock = true;
                    }
                }
                if(stop[7] == 0 && row>=0 && row<8 && col+i>=0 && col+i<8){
                    if(grid[row][col+i].type!=EMPTY){
                        if(grid[row][col+i].isBlack != current_block.isBlack)
                            grid[row][col+i].passibleBlock = true;
                        stop[7]=1;
                    }
                    else{
                        grid[row][col+i].passibleBlock = true;
                    }
                }
            }
            break;
            
        case KNIGHT:
            if(row-2 >=0 && row-2 <8 && col-1 >=0 && col-1<8){
                if(grid[row-2][col-1].isBlack != current_block.isBlack && grid[row-2][col-1].type!=EMPTY)
                    grid[row-2][col-1].passibleBlock = true;
                else if(grid[row-2][col-1].type==EMPTY)
                    grid[row-2][col-1].passibleBlock = true;
            }
            if(row-2 >=0 && row-2 <8 && col+1 >=0 && col+1<8){
                if(grid[row-2][col+1].isBlack != current_block.isBlack && grid[row-2][col+1].type!=EMPTY)
                    grid[row-2][col+1].passibleBlock = true;
                else if(grid[row-2][col+1].type==EMPTY)
                    grid[row-2][col+1].passibleBlock = true;
            }
            if(row+2 >=0 && row+2 <8 && col-1 >=0 && col-1<8){
                if(grid[row+2][col-1].isBlack != current_block.isBlack && grid[row+2][col-1].type!=EMPTY)
                    grid[row+2][col-1].passibleBlock = true;
                else if(grid[row+2][col-1].type==EMPTY)
                    grid[row+2][col-1].passibleBlock = true;
            }
            if(row+2 >=0 && row+2 <8 && col+1 >=0 && col+1<8){
                if(grid[row+2][col+1].isBlack != current_block.isBlack && grid[row+2][col+1].type!=EMPTY)
                    grid[row+2][col+1].passibleBlock = true;
                else if(grid[row+2][col+1].type==EMPTY)
                    grid[row+2][col+1].passibleBlock = true;
            }
            if(row-1 >=0 && row-1 <8 && col-2 >=0 && col-2<8){
                if(grid[row-1][col-2].isBlack != current_block.isBlack && grid[row-1][col-2].type!=EMPTY)
                    grid[row-1][col-2].passibleBlock = true;
                else if(grid[row-1][col-2].type==EMPTY)
                    grid[row-1][col-2].passibleBlock = true;
            }
            if(row-1 >=0 && row-1 <8 && col+2 >=0 && col+2<8){
                if(grid[row-1][col+2].isBlack != current_block.isBlack && grid[row-1][col+2].type!=EMPTY)
                    grid[row-1][col+2].passibleBlock = true;
                else if(grid[row-1][col+2].type==EMPTY)
                    grid[row-1][col+2].passibleBlock = true;
            }
            if(row+1 >=0 && row+1 <8 && col-2 >=0 && col-2<8){
                if(grid[row+1][col-2].isBlack != current_block.isBlack && grid[row+1][col-2].type!=EMPTY)
                    grid[row+1][col-2].passibleBlock = true;
                else if(grid[row+1][col-2].type==EMPTY)
                    grid[row+1][col-2].passibleBlock = true;
            }
            if(row+1 >=0 && row+1 <8 && col+2 >=0 && col+2<8){
                if(grid[row+1][col+2].isBlack != current_block.isBlack && grid[row+1][col+2].type!=EMPTY)
                    grid[row+1][col+2].passibleBlock = true;
                else if(grid[row+1][col+2].type==EMPTY)
                    grid[row+1][col+2].passibleBlock = true;
            }
            break;
            
        case PAWN:
            int direct = 1;
            if(current_block.isBlack == false){
                direct = -1;
            }
            if(row+direct >= 0 && row+direct < 8){
                if(grid[row+direct][col].type == EMPTY)
                    grid[row+direct][col].passibleBlock = true;
                if(col-1 >= 0 && col-1 < 8 && grid[row+direct][col-1].type!=EMPTY && grid[row+direct][col-1].isBlack!=current_block.isBlack){
                    grid[row+direct][col-1].passibleBlock = true;
                }
                if(col+1 >= 0 && col+1 < 8 && grid[row+direct][col+1].type!=EMPTY && grid[row+direct][col+1].isBlack!=current_block.isBlack){
                    grid[row+direct][col+1].passibleBlock = true;
                }
            }
            if((current_block.isBlack == true && row == 1)||(current_block.isBlack == false && row == 6)){
                if(grid[row+direct+direct][col].type == EMPTY)
                    grid[row+direct+direct][col].passibleBlock = true;
            }
            break;
    }
}

void Board::initPassibleBlock(){
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            grid[i][j].passibleBlock = false;
        }
    }
}

int Board::movePiece(int oldx, int oldy, int newx, int newy){
    Particle particles[200];
    if (moveDone) {
        moveDone=false;
        preX=oldx;preY=oldy;
        targetX=newx;targetY=newy;
        nowX=oldx;nowY=oldy;nowZ=8;
        movePhase=1;
    }
    switch (movePhase){
        case 1:
            targetZ=20;
            if (nowZ!=targetZ){
                nowZ += 2;
                pieces[grid[oldx][oldy].pieceIndex].move(nowX, nowY,nowZ);
            }
            else {
                movePhase=2;
            }
            break;
        case 2:
            if (fabs(0.1+nowX-targetX-0.1)>0.001) nowX -= (oldx - targetX)/abs(oldx - targetX)/5.0;
            if (fabs(0.1+nowY-targetY-0.1)>0.001) nowY -= (oldy - targetY)/abs(oldy - targetY)/5.0;
            pieces[grid[oldx][oldy].pieceIndex].move(nowX, nowY,nowZ);
            if (fabs(0.1+nowX-targetX-0.1)<0.001 && fabs(0.1+nowY-targetY-0.1)<0.001) {
                movePhase=3;
                if(grid[newx][newy].type != EMPTY){
                    pieces[grid[newx][newy].pieceIndex].isValid = false;
                }
                
                for(int i = 0; i < 200; i++){
                    particles[i].CreateParticle(targetX*8-28, targetY*8-28, 15);
                }
            }
            break;
        case 3:
            targetZ=8;
            if (nowZ!=targetZ) {
                nowZ -= 2;
                pieces[grid[oldx][oldy].pieceIndex].move(nowX, nowY,nowZ);
                
                for (int i = 0; i < 200; i++)
                {
                    particles[i].EvolveParticle();    
                    particles[i].DrawObjects(needTurn);
                    //cout << particles[i].GetXPos()<< endl;
                }   
            }
            else {
                movePhase=0;
            }
            break;
    }
   
    if (fabs(0.1+nowX-targetX-0.1)<0.001 && fabs(0.1+nowY-targetY-0.1)<0.001&& fabs(0.1+nowZ-8-0.1)<0.001) {
        moveDone=true;
        pieces[grid[oldx][oldy].pieceIndex].move(targetX,targetY,8);
        
        //如果新的地方原本有棋子，就把它移除
        if (grid[newx][newy].type != EMPTY) {
            pieces[grid[newx][newy].pieceIndex].isValid = false;
        }
        
        int result = 0;
        if (grid[newx][newy].type == KING) {
            if (grid[newx][newy].isBlack == true)
                result = 1;
            else
                result = -1;
        }
        if (grid[oldx][oldy].type == PAWN) {
            if(grid[oldx][oldy].isBlack == true && newx == 7){
                grid[oldx][oldy].type = QUEEN;
            }
            if(grid[oldx][oldy].isBlack == false && newx == 0){
                grid[oldx][oldy].type = QUEEN;
            }
        }
        grid[newx][newy].type = grid[oldx][oldy].type;
        grid[newx][newy].isBlack = grid[oldx][oldy].isBlack;
        grid[newx][newy].pieceIndex = grid[oldx][oldy].pieceIndex;
        grid[oldx][oldy].type = EMPTY;
        grid[oldx][oldy].pieceIndex = -1;
        return result;
    }
    else {
        return 0;
    }    
}

