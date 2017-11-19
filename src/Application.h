//
// Created by Darrow, Jackson on 11/17/17.
//

#ifndef INC_3DBILLIARDS_APPLICATION_H
#define INC_3DBILLIARDS_APPLICATION_H


#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"
#include "Player.h"
#include "Physics.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Application : public EventCallbacks {
public:
    WindowManager *windowManager = nullptr;

    std::shared_ptr<Program> prog;
    std::shared_ptr<Program> texProg;
    std::shared_ptr<Program> skyProg;

    shared_ptr<Texture> texture0;
    shared_ptr<Texture> specularTexture;

    shared_ptr<Player> p1, p2;

    shared_ptr<Shape> cube, ship, box;

    // Contains vertex information for OpenGL
    GLuint VertexArrayID;
    // Data necessary to give our triangle to OpenGL
    GLuint VertexBufferID;

    //geometry for texture render
    GLuint quad_VertexArrayID;
    GLuint quad_vertexbuffer;

    GLuint sky_VertexArrayId;
    GLuint sky_vertexbuffer;

    //ground plane info
    GLuint GrndBuffObj, GrndNorBuffObj, GrndTexBuffObj, GIndxBuffObj;
    int gGiboLen;

    //reference to texture FBO
    GLuint frameBuf[3];
    GLuint texBuf[3];
    GLuint texSkybox;
    GLuint depthBuf;

    bool FirstTime = true;
    bool Moving = false;
    int gMat = 0;

    bool mouseDown = false;

    void scrollCallback(GLFWwindow *window, double deltaX, double deltaY);
    void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void cursorPositionCallback(GLFWwindow *window, double x, double y);
    void resizeCallback(GLFWwindow *window, int width, int height);
    void setMaterial(int i);
    void addVars(std::shared_ptr<Program> prog);


    void create_cube_map(string front, string back, string top, string bottom, string left, string right, GLuint* tex_cube);
    bool load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name);

    void init(const std::string &resourceDirectory);
    void initGeom(const std::string &resourceDirectory);
    void initTex(const std::string &resourceDirectory);
    void initQuad();
    void initFloor();
    void initSkybox();
    void createFBO(GLuint &fb, GLuint &tex);

    void render(PxActor** actors, int numActors);
    void renderScene(PxActor **actors, int numActors, GLuint buffer, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P);
    void renderActors(PxActor **actors, int numActors, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P);
    void drawTV(GLuint inTex, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P);
    void renderSkyBox();
    void renderGround();
};


#endif //INC_3DBILLIARDS_APPLICATION_H
