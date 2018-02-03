#include "Camera.h"
#include "Menu.h"
#include "Environment.h"

static GLsizei winWidth = 800;
static GLsizei winHeight = 800;

Board *board = NULL;
Camera *camera = NULL;
Menu *menu = NULL;
Environment *environment = NULL;
std::vector<ObjLoader> objLoaders;
std::vector<Piece> pieces;

bool isSingle = true;
bool isStart = true;
bool isPause = false;

/* 2018.1.15 lyw add */
bool needTurn = false;

void drawScene() {
    /* draw chess board */
    if (needTurn) {
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    }
    
    board->drawBoard(GL_RENDER);
    board->drawBottom();
    
    /* draw room */
    environment->draw();

    /* draw chess pieces */
    for(int i=0; i<pieces.size(); i++) {
        pieces[i].draw();
    }
}

void drawMenu() {
    if (needTurn) {
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    }
    if(isStart){
        menu->drawBlankBoard(camera);
        menu->drawStartMenu(GL_RENDER, camera);
    }
    
    if(isPause){
        menu->drawBlankBoard(camera);
        menu->drawPauseMenu(GL_RENDER, camera);
    }
    
    if(camera->judgewin == -1){
        menu->drawBlankBoard(camera);
        menu->drawBlackWinsMenu(GL_RENDER, camera);
    }
    
    if(camera->judgewin == 1){
        menu->drawBlankBoard(camera);
        menu->drawWhiteWinsMenu(GL_RENDER, camera);
    }
}

void Light() {
    GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};       //白色主光源环境光
    GLfloat diffuseLight[] = {1.0f, 1.0f, 1.0f, 1.0f};       //白色主光源漫反射
    GLfloat specularLight[] = {1.0f, 1.0f, 1.0f, 1.0f};      //白色主光源镜面光
    GLfloat lightPos[] = { 0.0f, 0.0f, 250.0f, 1.0f};

    GLfloat globalAmbient[] = {0.8f, 0.8f, 0.8f, 1.0f};

    glEnable(GL_LIGHTING);                                  //启用光照
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);         //设置环境光源
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);         //设置漫反射光源
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);       //设置镜面光源
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);            //设置灯光位置

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.1);    //二次衰减
    
    glEnable(GL_LIGHT0);                                //打开白色主光源
    
    glEnable(GL_COLOR_MATERIAL);                        //启用材质的颜色跟踪
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);   //指定材料着色的面
    glMaterialfv(GL_FRONT,GL_SPECULAR, specularLight);   //指定材料对镜面光的反应
    glMateriali(GL_FRONT,GL_SHININESS, 50.0);              //指定反射系数
}

int initEnvironment() {
    environment = new Environment(glm::vec3(0.0f, 0.0f, 0.0f), 1000.0f, 1000.0f, 400.0f,
                                "./environment/wall.jpg", "./environment/wall.jpg",
                                "./environment/wall.jpg", "./environment/wall.jpg",
                                "./environment/floor.jpg", "./environment/ceiling.jpg");
    environment->setup();
    environment->addFurniture("table", "table/table.obj",
                                glm::vec3(50.0f, 50.0f, 70.0f),
                                glm::vec3(0.0f, 0.0f, -5.9f),
                                glm::vec3(90.0f, 0.0f, 0.0f));
    environment->addFurniture("light", "light/light.obj",
                                glm::vec3(250.0f, 250.0f, 250.0f),
                                glm::vec3(0.0f, 0.0f, 4.0f),
                                glm::vec3(90.0f, 0.0f, 0.0f)); 
    environment->addFurniture("ai_player", "woman/woman.obj",
                                glm::vec3(1.6f, 1.6f, 1.6f),
                                glm::vec3(-300.0f, 0.0f, -240.0f),
                                glm::vec3(90.0f, 90.0f, 0.0f));
    /*
    environment->addFurniture("ai_player", "nanosuit/nanosuit.obj",
                                glm::vec3(60.0f, 60.0f, 60.0f),
                                glm::vec3(-6.8f, 0.0f, -6.8f),
                                glm::vec3(90.0f, 90.0f, 0.0f));
    */
    return 0;
}

int initObjLoader() {
    objLoaders.push_back(ObjLoader("WHITE_KING", "./Chess/white/King.obj"));      // white king  , index 0
    objLoaders.push_back(ObjLoader("WHITE_QUEEN", "./Chess/white/Queen.obj"));     // white queen , index 1
    objLoaders.push_back(ObjLoader("WHITE_BISHOP", "./Chess/white/Bishop.obj"));    // white bishop, index 2
    objLoaders.push_back(ObjLoader("WHITE_KNIGHT", "./Chess/white/Knight.obj"));    // white knight, index 3
    objLoaders.push_back(ObjLoader("WHITE_ROOK", "./Chess/white/Rook.obj"));      // white rook  , index 4
    objLoaders.push_back(ObjLoader("WHITE_PAWN", "./Chess/white/Pawn.obj"));      // white pawn  , index 5
    
    objLoaders.push_back(ObjLoader("BLACK_KING", "./Chess/black/King.obj"));      // black king  , index 6
    objLoaders.push_back(ObjLoader("BLACK_QUEEN", "./Chess/black/Queen.obj"));     // black queen , index 7
    objLoaders.push_back(ObjLoader("BLACK_BISHOP", "./Chess/black/Bishop.obj"));    // black bishop, index 8
    objLoaders.push_back(ObjLoader("BLACK_KNIGHT", "./Chess/black/Knight.obj"));    // black knight, index 9
    objLoaders.push_back(ObjLoader("BLACK_ROOK", "./Chess/black/Rook.obj"));      // black rook  , index 10
    objLoaders.push_back(ObjLoader("BLACK_PAWN", "./Chess/black/Pawn.obj"));      // black pawn  , index 11
    
    objLoaders.push_back(ObjLoader("CHESS_BOARD", "./Chess/board/ChessBoard.obj"));         // white board , index 12
    
    return objLoaders.size();
}

void initPieces() {
    pieces.push_back(Piece(glm::vec3(-28.0f, -28.0f, 8.0f), glm::vec3(90.0f, 180.0f, 0.0f), ROOK, true,
                           &(objLoaders[10]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(-28.0f, -20.0f, 8.0f), glm::vec3(90.0f, 180.0f, 0.0f), KNIGHT, true,
                           &(objLoaders[9]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(-28.0f, -12.0f, 8.0f), glm::vec3(90.0f, 180.0f, 0.0f), BISHOP, true,
                           &(objLoaders[8]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(-28.0f, -4.0f, 8.0f), glm::vec3(90.0f, 180.0f, 0.0f), QUEEN, true,
                           &(objLoaders[7]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(-28.0f, 4.0f, 8.0f), glm::vec3(90.0f, 180.0f, 0.0f), KING, true,
                           &(objLoaders[6]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(-28.0f, 12.0f, 8.0f), glm::vec3(90.0f, 180.0f, 0.0f), BISHOP, true,
                           &(objLoaders[8]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(-28.0f, 20.0f, 8.0f), glm::vec3(90.0f, 180.0f, 0.0f), KNIGHT, true,
                           &(objLoaders[9]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(-28.0f, 28.0f, 8.0f), glm::vec3(90.0f, 180.0f, 0.0f), ROOK, true,
                           &(objLoaders[10]), &(objLoaders[7]), &(objLoaders[1])));
    for(int i=0; i<8; i++){
        pieces.push_back(Piece(glm::vec3(-20.0f, -28.0f + (GLfloat)i*8, 8.0f), glm::vec3(90.0f, 180.0f, 0.0f), PAWN, true,
                               &(objLoaders[11]), &(objLoaders[7]), &(objLoaders[1])));
    }
    pieces.push_back(Piece(glm::vec3(28.0f, -28.0f, 8.0f), glm::vec3(90.0f, 0.0f, 0.0f), ROOK, false,
                           &(objLoaders[4]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(28.0f, -20.0f, 8.0f), glm::vec3(90.0f, 0.0f, 0.0f), KNIGHT, false,
                           &(objLoaders[3]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(28.0f, -12.0f, 8.0f), glm::vec3(90.0f, 0.0f, 0.0f), BISHOP, false,
                           &(objLoaders[2]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(28.0f, -4.0f, 8.0f), glm::vec3(90.0f, 0.0f, 0.0f), QUEEN, false,
                           &(objLoaders[1]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(28.0f, 4.0f, 8.0f), glm::vec3(90.0f, 0.0f, 0.0f), KING, false,
                           &(objLoaders[0]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(28.0f, 12.0f, 8.0f), glm::vec3(90.0f, 0.0f, 0.0f), BISHOP, false,
                           &(objLoaders[2]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(28.0f, 20.0f, 8.0f), glm::vec3(90.0f, 0.0f, 0.0f), KNIGHT, false,
                           &(objLoaders[3]), &(objLoaders[7]), &(objLoaders[1])));
    pieces.push_back(Piece(glm::vec3(28.0f, 28.0f, 8.0f), glm::vec3(90.0f, 0.0f, 0.0f), ROOK, false,
                           &(objLoaders[4]), &(objLoaders[7]), &(objLoaders[1])));
    for(int i=0; i<8; i++){
        pieces.push_back(Piece(glm::vec3(20.0f, -28.0f + (GLfloat)i*8, 8.0f), glm::vec3(90.0f, 0.0f, 0.0f), PAWN, false,
                               &(objLoaders[5]), &(objLoaders[7]), &(objLoaders[1])));
    }
}

void initNewGame(){
    camera->initAI();
    camera->judgewin = 0;
    camera->isblackturn = false;
    pieces.clear();
    initPieces();
    board = new Board(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 180.0f), &(objLoaders[12]));
}

void init() {
    pieces.clear();
    initObjLoader();
    initPieces();
    initEnvironment();
    
    camera = new Camera();
    board = new Board(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 180.0f), &(objLoaders[12]));
    menu = new Menu();
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor (0.1f, 0.1f, 0.1f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEdgeFlag(GL_TRUE);
}

void Display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* adjust the view */
    camera->update(board,isSingle);
    
    Light();
    
    drawScene();
    
    drawMenu();

    glutSwapBuffers();
}

void Idle() {
    
    glutPostRedisplay();
}

void Reshape(GLint w, GLint h) {
    if (h == 0) {
        h = 1;
    }
    
    camera->reshape(w, h);
    menu->reshape(w, h);
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0, (GLfloat)w / (GLfloat)h, 0.1f, 100000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(-100.f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f);
    
    glutPostRedisplay();
}

void KeyboardFunc(unsigned char key,int x,int y) {
    if(isStart == false && isPause == false && camera->judgewin == 0)
        camera->keyboard(key, board);
    if(key == 27 && isStart == false && camera->judgewin == 0)
        isPause = !isPause;
}

void SpecialKey(int key,int x,int y){
    if(isStart == false && isPause == false && camera->judgewin == 0)
        camera->specialkey(key, board);
}

void MouseFunc(int button, int state, int x, int y)
{
    if(isStart == false && isPause == false && camera->judgewin == 0){
        camera->mouseClick(button, state, x, y, board, isSingle);
    }
    else if(state == GLUT_DOWN){
        switch (menu->mouseClickMenuEvent(x, y, camera, isStart)) {
            case 81:
                isStart = false;
                isSingle = true;
                initNewGame();
                break;
            case 82:
                isStart = false;
                isSingle = false;
                initNewGame();
                break;
            case 84:
                isPause = false;
                break;
            case 85:
                isPause = false;
                isStart = true;
                initNewGame();
                break;
            case 86:
                isPause = false;
                initNewGame();
                break;
            default:
                break;
        }
        
    }
}

void MotionFunc(int x,int y) {
    camera->mouseMove(x, y, board);
}

int main(int argc,char* argv[])
{

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("3D Chess");

    init();
    
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutKeyboardFunc(KeyboardFunc);
    glutSpecialFunc(SpecialKey);
    glutIdleFunc(Idle);
    glutMouseFunc(MouseFunc);
    glutMotionFunc(MotionFunc);
    
    glutMainLoop();
    
    return 0;
}



