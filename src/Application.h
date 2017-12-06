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
#include "TextureCube.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Application : public EventCallbacks {
public:
    WindowManager *windowManager = nullptr;
    shared_ptr<Player> p1;
    shared_ptr<Player> p2;

    // shader programs
    shared_ptr<Program> prog;
    shared_ptr<Program> texProg;
    shared_ptr<Program> skyProg;
    shared_ptr<Program> depthProg;
    shared_ptr<Program> shadowProg;
    shared_ptr<Program> fboTexProg;

    // textures
    shared_ptr<Texture> texture0; // TODO: move to ballTexture
    shared_ptr<Texture> ballTexture[16];
    shared_ptr<Texture> specularTexture;
    shared_ptr<TextureCube> skyboxTex;

    // texture FBOs
    shared_ptr<Texture> largeRender;
    shared_ptr<Texture> leftSplitScreen;
    shared_ptr<Texture> rightSplitScreen;
    shared_ptr<Texture> shadowMap;

    // shapes
    shared_ptr<Shape> cube; // TODO: rename this to sphere
    shared_ptr<Shape> ship;
    shared_ptr<Shape> box;
    shared_ptr<Shape> floor;
    shared_ptr<Shape> quad;


    //reference to texture FBO
    GLuint frameBuf[3];
    GLuint texBuf[3];
    GLuint texSkybox;
    GLuint depthBuf;

    GLuint quad_VertexArrayID;
    GLuint quad_vertexbuffer,quad_normalbuffer;

    float time = -2.0f;

//    bool FirstTime = true;
//    bool Moving = false;
//    int gMat = 0;
//    bool mouseDown = false;

    void scrollCallback(GLFWwindow *window, double deltaX, double deltaY);
    void mouseCallback(GLFWwindow *window, int button, int action, int mods);
    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void cursorPositionCallback(GLFWwindow *window, double x, double y);
    void resizeCallback(GLFWwindow *window, int width, int height);
    void setMaterial(int i);

    void init();
    void initQuad();
    void initShaders(const std::string &resourceDirectory);
    void initGeometry(const std::string &resourceDirectory);
    void initTextures(const std::string &resourceDirectory);
//    void createFBO(GLuint &fb, GLuint &tex);

    void renderSquare(shared_ptr<Program> prog,
                      shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> V,
                      float x, float z, int side);
    void renderBuilding(shared_ptr<Program> prog,
                        shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> V,
                        float x, float z);
    void renderBuildingScene(shared_ptr<Program> prog,
                     shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> V);

    float getY(float x, float z);

    void SetMaterial(shared_ptr<Program> prog,int mode);


    inline void bindUniforms(shared_ptr<Program> program, const float *M, const float *V, const float *P,
                                          shared_ptr<Texture> diffTex, shared_ptr<Texture> specTex,
                                          int material, shared_ptr<Player> player);

    void renderDepthBuffer(PxActor **actors, int numActors, shared_ptr<MatrixStack> M,
                                        shared_ptr<MatrixStack> V,
                                        shared_ptr<MatrixStack> P, shared_ptr<Player> player);


    mat4 getDepthMVP();

    void render(PxActor** actors, int numActors);
    void renderScene(PxActor **actors, int numActors, GLuint buffer, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P, shared_ptr<Player> player);
    void drawFBO(shared_ptr<Texture> fboTex, shared_ptr<Shape> geom, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P);

    void renderPxActors(PxActor **actors, int numActors, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V,
                                     shared_ptr<MatrixStack> P, shared_ptr<Player> player, shared_ptr<Program> geomProg);
};


#endif //INC_3DBILLIARDS_APPLICATION_H
