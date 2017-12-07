//
// Created by Darrow, Jackson on 11/17/17.
//

#include "Application.h"

void Application::scrollCallback(GLFWwindow *window, double deltaX, double deltaY) {

}
void Application::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (action == GLFW_RELEASE) {
    }
}
void Application::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    p1->keyboardInputCB(window, key, scancode, action, mods);
}
void Application::cursorPositionCallback(GLFWwindow *window, double x, double y) {
    p1->mouseInputCB(window, x, y);
}
void Application::resizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height/2);
}
void Application::setMaterial(int i) {
    switch (i) {
        case 0: //shiny blue plastic
            glUniform3f(prog->getUniform("ambient"), 0.02, 0.04, 0.2);
            glUniform3f(prog->getUniform("diffuse"), 0.0, 0.16, 0.9);
            glUniform3f(prog->getUniform("specular"), 0.14, 0.2, 0.8);
            glUniform1f(prog->getUniform("shiny"), 120.0);
            break;
        case 1: // flat grey
            glUniform3f(prog->getUniform("ambient"), 0.13, 0.13, 0.14);
            glUniform3f(prog->getUniform("diffuse"), 0.3, 0.3, 0.4);
            glUniform3f(prog->getUniform("specular"), 0.3, 0.3, 0.4);
            glUniform1f(prog->getUniform("shiny"), 4.0);
            break;
        case 2: //brass
            glUniform3f(prog->getUniform("ambient"), 0.329, 0.2235, 0.02745);
            glUniform3f(prog->getUniform("diffuse"), 0.780, 0.5686, 0.11373);
            glUniform3f(prog->getUniform("specular"), 0.992, 0.941176, 0.80784);
            glUniform1f(prog->getUniform("shiny"), 27.9);
            break;
        case 3: //slime
            glUniform3f(prog->getUniform("ambient"), 0.129, 0.1235, 0.02745);
            glUniform3f(prog->getUniform("diffuse"), 0.480, 0.8686, 0.01373);
            glUniform3f(prog->getUniform("specular"), 0.192, 0.941176, 0.80784);
            glUniform1f(prog->getUniform("shiny"), 200);
            break;
        case 4: //snow
            glUniform3f(prog->getUniform("ambient"), 0.2, 0.2, 0.2);
            glUniform3f(prog->getUniform("diffuse"), 0.8, 0.8, 0.8);
            glUniform3f(prog->getUniform("specular"), 0.95, 0.95, 0.95);
            glUniform1f(prog->getUniform("shiny"), 200);
            break;
            
    }
}
void Application::init() {
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    GLSL::checkVersion();

    // Set background color.
    glClearColor(.1f, .1f, .1f, 1.0f);
    // Enable z-buffer test.
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //create two frame buffer objects to toggle between
    glGenFramebuffers(2, frameBuf);
    glGenTextures(2, texBuf);
    glGenRenderbuffers(1, &depthBuf);

    //set up depth necessary as rendering a mesh that needs depth test
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

    //more FBO set up
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    p1 = make_shared<Player>(0);
    p2 = make_shared<Player>(1);
}

void Application::initShaders(const std::string &resourceDirectory) {
    prog = make_shared<Program>();
    prog->setVerbose(true);
    prog->setShaderNames(resourceDirectory + "/vert_phong.glsl", resourceDirectory + "/frag_phong.glsl");
    prog->init();
    prog->addUniform("P");
    prog->addUniform("V");
    prog->addUniform("M");
    prog->addUniform("diffuse");
    prog->addUniform("specular");
    prog->addUniform("shiny");
    prog->addUniform("ambient");
    prog->addUniform("source");
    prog->addUniform("depthMVP");
    prog->addUniform("depthBiasMVP");
    prog->addUniform("depthMap");
    prog->addAttribute("L");
    prog->addAttribute("E");
    prog->addAttribute("N");
    prog->addAttribute("vertPos");
    prog->addAttribute("vertNor");
    prog->addAttribute("vertTex");
    prog->addUniform("time");


    debugProg = make_shared<Program>();
    debugProg->setVerbose(true);
    debugProg->setShaderNames( resourceDirectory + "/debug_vert.glsl", resourceDirectory + "/debug_frag.glsl");
    debugProg->init();
    debugProg->addUniform("P");
    debugProg->addUniform("V");
    debugProg->addUniform("M");
    debugProg->addUniform("diffuseTexture");
    debugProg->addAttribute("vertPos");
    debugProg->addAttribute("vertNor");
    debugProg->addAttribute("vertTex");
    debugProg->addAttribute("vTexCoord");

    texProg = make_shared<Program>();
    texProg->setVerbose(true);
    texProg->setShaderNames( resourceDirectory + "/shatter_vert.glsl", resourceDirectory + "/shatter_frag.glsl",
                             resourceDirectory + "/shatter_geo.glsl");
    texProg->init();
    texProg->addUniform("P");
    texProg->addUniform("V");
    texProg->addUniform("M");
    texProg->addUniform("specularTexture");
    texProg->addUniform("diffuseTexture");
    texProg->addUniform("eyePos");
    texProg->addUniform("width");
    texProg->addUniform("height");
    texProg->addUniform("texBuf");
    texProg->addUniform("time");
    texProg->addUniform("depthMVP");
    texProg->addUniform("depthBiasMVP");
    texProg->addUniform("depthMap");
    texProg->addAttribute("vertPos");
    texProg->addAttribute("vertNor");
    texProg->addAttribute("vertTex");
    texProg->addAttribute("vTexCoord");
    texProg->addAttribute("L");
    texProg->addAttribute("E");
    texProg->addAttribute("N");
    texProg->addAttribute("triNum");

    fboTexProg = make_shared<Program>();
    fboTexProg->setVerbose(true);
    fboTexProg->setShaderNames( resourceDirectory + "/fbo_tex_vert.glsl", resourceDirectory + "/fbo_tex_frag.glsl");
    fboTexProg->init();
    fboTexProg->addUniform("P");
    fboTexProg->addUniform("V");
    fboTexProg->addUniform("M");
    fboTexProg->addUniform("width");
    fboTexProg->addUniform("height");
    fboTexProg->addUniform("texBuf");
    fboTexProg->addUniform("depthMVP");
    fboTexProg->addUniform("depthBiasMVP");
    fboTexProg->addUniform("depthMap");
    fboTexProg->addAttribute("vertTex");
    fboTexProg->addAttribute("vertPos");
    fboTexProg->addAttribute("vertNor");
    fboTexProg->addAttribute("texCoord");
    fboTexProg->addUniform("time");

    skyProg = make_shared<Program>();
    skyProg->setVerbose(true);
    skyProg->setShaderNames( resourceDirectory + "/sky_vert.glsl", resourceDirectory + "/sky_frag.glsl");
    skyProg->init();
    skyProg->addUniform("P");
    skyProg->addUniform("V");
    skyProg->addUniform("M");
    skyProg->addUniform("skyTexture");
    skyProg->addAttribute("vertPos");
    skyProg->addAttribute("vertNor");
    skyProg->addAttribute("vertTex");
    skyProg->addAttribute("vTexCubeCoord");
    skyProg->addUniform("time");

    depthProg = make_shared<Program>();
    depthProg->setVerbose(true);
    depthProg->setShaderNames( resourceDirectory + "/shadowmap_vert.glsl", resourceDirectory + "/shadowmap_frag.glsl");
    depthProg->init();
    depthProg->addUniform("depthMVP");
    depthProg->addUniform("P");
    depthProg->addUniform("V");
    depthProg->addUniform("M");
    depthProg->addUniform("depthMVP");
    depthProg->addUniform("depthBiasMVP");
    depthProg->addUniform("depthMap");
    depthProg->addUniform("eyePos");
    depthProg->addUniform("diffuseTexture");
    depthProg->addUniform("specularTexture");
    depthProg->addAttribute("vertPos");
    depthProg->addAttribute("vertNor");
    depthProg->addAttribute("vertTex");
    depthProg->addAttribute("vertexPosition_modelspace");
    depthProg->addUniform("time");
}

void Application::initGeometry(const std::string &resourceDirectory) {
    sphere = make_shared<Shape>();
    sphere->loadMesh(resourceDirectory + "/sphere.obj");
    sphere->resize(1);
    sphere->init();

    box = make_shared<Shape>();
    box->loadMesh(resourceDirectory + "/cube.obj");
    box->resize(1);
    box->init();

    ship = make_shared<Shape>();
    ship->loadMesh(resourceDirectory + "/grid_ball.obj");
    ship->resize(1);
    ship->init();

    shipInner = make_shared<Shape>();
    shipInner->loadMesh(resourceDirectory + "/ship.obj");
    shipInner->resize(1);
    shipInner->init();

    floor = make_shared<Shape>();
    floor->loadFloorGeom();
    floor->init();

    quad = make_shared<Shape>();
    quad->loadQuadGeom();
    quad->init();

    initQuad();
}

void Application::initTextures(const std::string &resourceDirectory) {
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

    skyboxTex = make_shared<TextureCube>();
    skyboxTex->init(
            resourceDirectory + "/skybox/Box_Back.jpg",
            resourceDirectory + "/skybox/Box_Front.jpg",
            resourceDirectory + "/skybox/Box_Top.jpg",
            resourceDirectory + "/skybox/Box_Bottom.jpg",
            resourceDirectory + "/skybox/Box_Left.jpg",
            resourceDirectory + "/skybox/Box_Right.jpg"
    );
    skyboxTex->setUnit(0);
    skyboxTex->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    texture0 = make_shared<Texture>();
    texture0->setFilename(resourceDirectory + "/Ball1.jpg");
    texture0->init();
    texture0->setUnit(1);
    texture0->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    for (int i = 0; i <= 16; i++) {
        ballTexture[i] = make_shared<Texture>();
        string filename = (i == 0 || i == 1) ? resourceDirectory + "/balls/BallCue.jpg" :
                          resourceDirectory + "/balls/Ball"+std::to_string(i-1)+".jpg";
        ballTexture[i]->setFilename(filename);
        ballTexture[i]->init();
        ballTexture[i]->setUnit(1);
        ballTexture[i]->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    }

    specularTexture = make_shared<Texture>();
    specularTexture->setFilename(resourceDirectory + "/glossy_specular.png");
    specularTexture->init();
    specularTexture->setUnit(2);
    specularTexture->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    shadowMap = make_shared<Texture>();
    shadowMap->initDepth();
    shadowMap->setUnit(3);
}

void Application::initPlayers(PxActor **actors, int actor1, int actor2) {
    p1->physX = (PxRigidDynamic *) actors[actor1];
    p2->physX = (PxRigidDynamic *) actors[actor2];
}

mat4 Application::getDepthMVP() {
    glm::vec3 lightInvDir = glm::vec3(10,30,10);

    glm::mat4 depthProjectionMatrix = glm::ortho<float>(-70,70,-70,70,-35,70);

    glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 depthModelMatrix = glm::mat4(1.0);
    glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

    return depthMVP;
}

void Application::renderDepthBuffer(PxActor **actors, int numActors, shared_ptr<MatrixStack> M,
                              shared_ptr<MatrixStack> V,
                              shared_ptr<MatrixStack> P, shared_ptr<Player> player) {
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->getFBO());
    glViewport(0,0,1024*4,1024*4); // Render on the whole framebuffer, complete from the lower left corner to the upper right

    // We don't use bias in the shader, but instead we draw back faces,
    // which are already separated from the front faces by a small distance
    // (if your geometry is made this way)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 depthMVP = getDepthMVP();

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform

//    shadowMap->bind(shadowProg->getUniform("shadowMap"));
//    glUniformMatrix4fv(shadowProg->getUniform("depthMVP"), 1, GL_FALSE, &depthMVP[0][0]);
    renderPxActors(actors, numActors, M, V, P, player, depthProg);
}

void Application::render(PxActor **actors, int numActors) {
    int seconds = 0;
    p1->update(seconds);
    p2->update(seconds);

    auto P = make_shared<MatrixStack>();
    auto O = make_shared<MatrixStack>();
    auto M = make_shared<MatrixStack>();
    auto V = make_shared<MatrixStack>();
    V->loadIdentity();

    int numPlayers = 1;
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    glViewport(0, 0, width, height);
    float aspect = width / (float) (height);
    P->pushMatrix();
    P->perspective(45.0f, aspect*numPlayers, 0.01f, 300.0f);

    O->pushMatrix();
    aspect /= 2;
    if (width > height) {
        O->ortho(-1 * aspect, 1 * aspect, -1, 1, -2, 100.0f);
    } else {
        O->ortho(-1, 1, -1 * 1 / aspect, 1 * 1 / aspect, -2, 100.0f);
    }

    renderDepthBuffer(actors, numActors, M, V, P, p1);

    if (numPlayers == 1) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, 0, width, height);

        V->pushMatrix();
        V->multMatrix(p1->getViewMatrix());
        renderScene(actors, numActors, 0, M, V, P, p1);
        V->popMatrix();

    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glViewport(0, height/2, width, height/2);

        V->pushMatrix();
        V->multMatrix(p1->getViewMatrix());
        renderScene(actors, numActors, 0, M, V, P, p1);
        V->popMatrix();

        glViewport(0, 0, width, height/2);

        V->pushMatrix();
        V->multMatrix(p2->getViewMatrix());
        renderScene(actors, numActors, 0, M, V, P, p2);
        V->popMatrix();
    }

    P->popMatrix();
}


float Application::getY(float x, float z) {
    return 4.f*sin(1.3f*x+2.2f*z) + 6.f;
}

void Application::renderBuildingScene(shared_ptr<Program> prog,shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> V) {
    for (int i = -6; i <=6; i+=2) {
        for (int j = -8; j <=9; j+=3) {
            if (abs(j) + abs(i) > 4)
                renderBuilding(prog,P,V,i,j);
        }
    }
}

void Application::renderBuilding(shared_ptr<Program> prog,
                    shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> V,
                    float x, float z) {
    prog->bind();
    glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
    glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
    renderSquare(prog,P,V,x,z,0);
    renderSquare(prog,P,V,x,z,1);
    renderSquare(prog,P,V,x,z,2);
    renderSquare(prog,P,V,x,z,3);
    renderSquare(prog,P,V,x,z,4);
    renderSquare(prog,P,V,x,z,5);
}

void Application::renderSquare(shared_ptr<Program> prog,
                  shared_ptr<MatrixStack> P, shared_ptr<MatrixStack> V,
                  float x, float z, int side) {
    auto M = make_shared<MatrixStack>();
    M->pushMatrix();
    M->loadIdentity();
    M->translate(vec3(0,-1,0));
    x*=2;
    z*=2;
    M->scale(8);
    switch (side) {
        case 0://top
            M->translate(vec3(x,2*getY(x,z),z));
            M->scale(.7);
            break;
        case 1://left
            M->translate(vec3(x - .7,getY(x,z),z));
            M->scale(vec3(.7,getY(x,z),.7));
            M->rotate(radians(90.f),vec3(0,0,1));
            break;
        case 2://right
            M->translate(vec3(x + .7,getY(x,z),z));
            M->scale(vec3(.7,getY(x,z),.7));
            M->rotate(radians(-90.f),vec3(0,0,1));
            break;
        case 3://farside
            M->translate(vec3(x,getY(x,z),z + .7));
            M->scale(vec3(.7,getY(x,z),.7));
            M->rotate(radians(90.f),vec3(1,0,0));
            break;
        case 4://closeside
            M->translate(vec3(x,getY(x,z),z - .7));
            M->scale(vec3(.7,getY(x,z),.7));
            M->rotate(radians(-90.f),vec3(1,0,0));
            break;
        case 5://bottom
            M->translate(vec3(x,.02,z));
            M->scale(.8);
            break;
    }
    M->rotate(radians(-90.f),vec3(1,0,0));

    SetMaterial(prog,7);
    //glUniform3f(prog->getUniform("eye"), eye.x,eye.y,eye.z);
    glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));

//    glUniform3f(prog->getUniform("lightPos"), 0,20,0);
    glBindVertexArray(quad_VertexArrayID);
    glDrawArrays(GL_TRIANGLES,0,6);
//        prog->unbind();
}


void Application::renderScene(PxActor **actors, int numActors, GLuint buffer, shared_ptr<MatrixStack> M,
                              shared_ptr<MatrixStack> V,
                              shared_ptr<MatrixStack> P, shared_ptr<Player> player) {

    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    skyProg->bind(); {
        glUniformMatrix4fv(skyProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        V->pushMatrix();
        V->loadIdentity();
        V->multMatrix(player->getSkyBoxViewMatrix());
        glUniformMatrix4fv(skyProg->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
        skyboxTex->bind(skyProg->getUniform("skyTexture"));
        glDepthMask(GL_FALSE);
        box->draw(skyProg);
        glDepthMask(GL_TRUE);
        V->popMatrix();
    }
    skyProg->unbind();




    debugProg->bind(); {

        glUniformMatrix4fv(debugProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(debugProg->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));

        mat4 emm = glm::scale(mat4(1), vec3(1, 1, 1)) * glm::translate(mat4(1), vec3(0, 1, 0));
        glUniformMatrix4fv(debugProg->getUniform("M"), 1, GL_FALSE, value_ptr(emm));

        shadowMap->bind(debugProg->getUniform("diffuseTexture"));
        ballTexture[2]->bind(debugProg->getUniform("diffuseTexture"));
        quad->draw(debugProg);

    } debugProg->unbind();




    prog->bind(); {
        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
        renderBuildingScene(prog,P,V);
    } prog->unbind();

    renderPxActors(actors, numActors, M, V, P, player, NULL);
}

inline void Application::bindUniforms(shared_ptr<Program> program, const float *M, const float *V, const float *P,
                                      shared_ptr<Texture> diffTex, shared_ptr<Texture> specTex,
                                      int material, shared_ptr<Player> player) {

    glUniformMatrix4fv(program->getUniform("M"), 1, GL_FALSE, M);
    glUniformMatrix4fv(program->getUniform("V"), 1, GL_FALSE, V);
    glUniformMatrix4fv(program->getUniform("P"), 1, GL_FALSE, P);

    glm::mat4 biasMatrix(
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0
    );
    glm::mat4 depthBiasMVP = biasMatrix*getDepthMVP();

    glUniformMatrix4fv(program->getUniform("depthMVP"), 1, GL_FALSE, value_ptr(getDepthMVP()));
    glUniformMatrix4fv(program->getUniform("depthBiasMVP"), 1, GL_FALSE, value_ptr(depthBiasMVP));

    shadowMap->bind(program->getUniform("depthMap"));

    if (diffTex != NULL)
        diffTex->bind(program->getUniform("diffuseTexture"));
    if (specTex != NULL)
        specTex->bind(program->getUniform("specularTexture"));
    if (player != NULL)
        glUniform3f(program->getUniform("eyePos"), player->getPosition().x, player->getPosition().y, player->getPosition().z);
    if (material > 0)
        SetMaterial(program, material);
}

void Application::renderPxActors(PxActor **actors, int numActors, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V,
                                 shared_ptr<MatrixStack> P, shared_ptr<Player> player, shared_ptr<Program> geomProg) {
    static const int MAX_SHAPES = 100;
    PxShape *shapes[10];
    for (int i = 0; i < numActors; i++) {
        auto *actor = actors[i]->is<PxRigidActor>();
        int nbShapes = actor->getNbShapes();
        auto userData = (UserData *) actor->userData;
        if (userData && i > 1) {
            if (actor->is<PxRigidBody>()){
                vec3 corner = vec3(0,100,0);
                if (userData->time > -2.0f) {
                    userData->time += .007;
                } else {
                    if (i < 10) {
                        corner = vec3(47.5,-1,-47.5);
                    } else if (i > 10) {
                        corner = vec3(-47.5,-1,47.5);
                    }
                    PxTransform temp = actor->getGlobalPose();
                    vec3 pos = vec3(temp.p.x,temp.p.y,temp.p.z);
                    if (pow(pos.x - corner.x,2.) + pow(pos.y - corner.y,2.) + pow(pos.z - corner.z,2.) < 6.f) {
                        userData->time = .0;
                    }
                }
            }
        }
        //if (i == 0) userData->time += .007;

        actor->getShapes(shapes, nbShapes);
        bool sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;

        for (int j = 0; j < nbShapes; j++) {
            PxGeometryHolder h = shapes[j]->getGeometry();

            PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actor));
//            shared_ptr<Material> material = userData->materialMap.at(h.getType());
            shared_ptr<Material> material = make_shared<Material>();

            material->shader = texProg;
            material->diffuseTex = ballTexture[(i > 17 ? 17 : i)];

            shared_ptr<Program> program = geomProg == NULL ? material->shader : geomProg;
            program->bind(); {
                if (userData) {
                    glUniform1f(program->getUniform("time"), (userData->time > 0 ? userData->time : 0.f));
                }

                mat4 M = make_mat4(shapePose.front()) * glm::rotate(glm::mat4(1), (float)M_PI_2, vec3(0, 1, 0));
                PxVec3 dims;
                PxBoxGeometry boxGeom;
                mat4 oM;

                switch (h.getType()) {
                    case PxGeometryType::eBOX:
                        shapes[j]->getBoxGeometry(boxGeom);
                        dims = boxGeom.halfExtents;

                        M *= glm::scale(glm::mat4(1), vec3(dims.z,dims.y, dims.x));
                        bindUniforms(program, value_ptr(M), value_ptr(V->topMatrix()), value_ptr(P->topMatrix()),
                                     material->diffuseTex, material->specularTex, material->material, player);
                        box->draw(program);
                        break;

                    case PxGeometryType::eSPHERE:
                        if (i == 0 || i == 1) {
                            oM = M;
                            oM[0][0] = oM[1][1] = oM[2][2] = .5f;
                            oM[0][1] = oM[1][0] = oM[0][2] = oM[2][0] = oM[1][2] = oM[2][1] = 0;
                        }
                        bindUniforms(program, value_ptr(M), value_ptr(V->topMatrix()), value_ptr(P->topMatrix()),
                                     material->diffuseTex, material->specularTex, material->material, player);

                        if (i == 0 || i == 1) {
                            ship->draw(program);
                            if (i == 0) {
                                bindUniforms(program, value_ptr(oM), value_ptr(V->topMatrix()), value_ptr(P->topMatrix()),
                                             ballTexture[4], material->specularTex, material->material, player);
                            } else {
                                bindUniforms(program, value_ptr(oM), value_ptr(V->topMatrix()), value_ptr(P->topMatrix()),
                                             ballTexture[3], material->specularTex, material->material, player);
                            }
                            shipInner->draw(program);
                        }
                        else
                            sphere->draw(program);
                        break;

                    case PxGeometryType::ePLANE:
                        M *= glm::scale(glm::mat4(1), vec3(100, 100, 100));
                        bindUniforms(program, value_ptr(M), value_ptr(V->topMatrix()), value_ptr(P->topMatrix()),
                                     material->diffuseTex, material->specularTex, material->material, player);
                        quad->draw(program);
                        break;

                    default:
                        break;
                }
            } program->unbind();
        }
    }
}

void Application::SetMaterial(shared_ptr<Program> active, int i) {
    switch (i) {
        case 1: //shiny blue plastic
            glUniform3f(active->getUniform("ambient"), 0.02, 0.04, 0.2);
            glUniform3f(active->getUniform("diffuse"), 0.0, 0.16, 0.9);
            glUniform3f(active->getUniform("specular"), 0.14, 0.2, 0.8);
            glUniform1f(active->getUniform("shiny"), 120.0);
//            glUniform1f(active->getUniform("texWeight"), 0);
            break;
        case 2: // bunny
            glUniform3f(active->getUniform("ambient"), 0.33, 0.23, 0.14);
            glUniform3f(active->getUniform("diffuse"), 0.2, 0.2, 0.2);
            glUniform3f(active->getUniform("specular"), 0.3, 0.3, 0.3);
            glUniform1f(active->getUniform("shiny"), 4.0);
//            glUniform1f(active->getUniform("texWeight"), 0);
            break;
        case 3: //brass
            glUniform3f(active->getUniform("ambient"), 0.3294, 0.2235, 0.02745);
            glUniform3f(active->getUniform("diffuse"), 0.7804, 0.5686, 0.11373);
            glUniform3f(active->getUniform("specular"), 0.9922, 0.941176, 0.80784);
            glUniform1f(active->getUniform("shiny"), 27.9);
//            glUniform1f(active->getUniform("texWeight"), 0);
            break;
        case 4: //wood
            glUniform3f(active->getUniform("ambient"), 0.2, 0.1, 0.0);
            glUniform3f(active->getUniform("diffuse"), 0.54, 0.34, 0.02);
            glUniform3f(active->getUniform("specular"), 0.1, 0.1, 0.1);
            glUniform1f(active->getUniform("shiny"), 27.9);
//            glUniform1f(active->getUniform("texWeight"), .05);
//            glUniform1f(active->getUniform("texSize"), 120);
            break;
        case 5: //leaves
            glUniform3f(active->getUniform("ambient"), 0.1, 0.4, 0.1);
            glUniform3f(active->getUniform("diffuse"), 0.11, 0.79, 0.12);
            glUniform3f(active->getUniform("specular"), 0.1, 0.2, 0.1);
            glUniform1f(active->getUniform("shiny"), 27.9);
//            glUniform1f(active->getUniform("texWeight"), .02);
//            glUniform1f(active->getUniform("texSize"), 12);
            break;
        case 6: //plant
            glUniform3f(active->getUniform("ambient"), 0.0, 0.3, 0.1);
            glUniform3f(active->getUniform("diffuse"), 0.02, 0.59, 0.12);
            glUniform3f(active->getUniform("specular"), 0.1, 0.2, 0.1);
            glUniform1f(active->getUniform("shiny"), 27.9);
//            glUniform1f(active->getUniform("texWeight"), .01);
            break;
        case 7: //building
            glUniform3f(active->getUniform("ambient"), 0.1, 0.1, 0.1);
            glUniform3f(active->getUniform("diffuse"), 0.2, 0.2, 0.2);
            glUniform3f(active->getUniform("specular"), 0.7, 0.7, 0.7);
            glUniform1f(active->getUniform("shiny"), 16.9);
//            glUniform1f(active->getUniform("texWeight"), .01);
            break;
        case 0: //plain
            glUniform3f(active->getUniform("ambient"), 0.8, 0.1, 0.1);
            glUniform3f(active->getUniform("diffuse"), 0.7, 0.1, 0.1);
            glUniform3f(active->getUniform("specular"), 1.0, 1.0, 1.0);
            glUniform1f(active->getUniform("shiny"), 80);
//            glUniform1f(active->getUniform("texWeight"), 0);

            break;
    }
}
void Application::initQuad() {
    //now set up a simple quad for rendering FBO
    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);

    static const GLfloat g_quad_vertex_buffer_data[] =
            {
                    -1.0f, -1.0f, 0.0f,
                    1.0f, -1.0f, 0.0f,
                    -1.0f,  1.0f, 0.0f,
                    -1.0f,  1.0f, 0.0f,
                    1.0f, -1.0f, 0.0f,
                    1.0f,  1.0f, 0.0f,
            };

    static const GLfloat g_quad_vertex_buffer_normals[] =
            {
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f,
                    0.0f, 0.0f, 1.0f
            };

    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glGenBuffers(1, &quad_normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_normals), g_quad_vertex_buffer_normals, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}