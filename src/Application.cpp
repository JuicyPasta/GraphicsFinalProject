//
// Created by Darrow, Jackson on 11/17/17.
//

#include "Application.h"
#include "stb_image.h"

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
    glViewport(0, 0, width, height);

    int downsampleScale = 2;
    largeRender->setDimensions(width*downsampleScale, height*downsampleScale);
    largeRender->init();
    largeRender->setUnit(20);
    largeRender->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    leftSplitScreen->setDimensions(width/2, height);
    leftSplitScreen->init();
    leftSplitScreen->setUnit(21);
    leftSplitScreen->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    rightSplitScreen->setDimensions(width/2, height);
    rightSplitScreen->init();
    rightSplitScreen->setUnit(22);
    rightSplitScreen->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    shadowMap->setDimensions(width/4, height/4);
    shadowMap->init();
    shadowMap->setUnit(23);
    shadowMap->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
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

    //create two frame buffer objects to toggle between
    glGenFramebuffers(2, frameBuf);
    glGenTextures(2, texBuf);
    glGenRenderbuffers(1, &depthBuf);
    createFBO(frameBuf[0], texBuf[0]);

    //set up depth necessary as rendering a mesh that needs depth test
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

    //more FBO set up
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    p1 = make_shared<Player>(0);
    p2 = make_shared<Player>(0);
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
    prog->addAttribute("L");
    prog->addAttribute("E");
    prog->addAttribute("N");
    prog->addAttribute("vertPos");
    prog->addAttribute("vertNor");
    prog->addAttribute("vertTex");

    texProg = make_shared<Program>();
    texProg->setVerbose(true);
    texProg->setShaderNames( resourceDirectory + "/tex_vert.glsl", resourceDirectory + "/tex_frag.glsl");
    texProg->init();
    texProg->addUniform("P");
    texProg->addUniform("V");
    texProg->addUniform("M");
    texProg->addUniform("specularTexture");
    texProg->addUniform("diffuseTexture");
    texProg->addAttribute("vertPos");
    texProg->addAttribute("vertNor");
    texProg->addAttribute("vertTex");
    texProg->addAttribute("vTexCoord");
    texProg->addAttribute("L");
    texProg->addAttribute("E");
    texProg->addAttribute("N");

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

    depthProg = make_shared<Program>();
    depthProg->setVerbose(true);
    depthProg->setShaderNames( resourceDirectory + "/shadowmap_vert.glsl", resourceDirectory + "/shadowmap_frag.glsl");
    depthProg->init();
    depthProg->addUniform("depthMVP");
    depthProg->addUniform("P");
    depthProg->addUniform("V");
    depthProg->addUniform("M");
    depthProg->addAttribute("vertPos");
    depthProg->addAttribute("vertNor");
    depthProg->addAttribute("vertTex");
    depthProg->addAttribute("vertexPosition_modelspace");
}

void Application::initGeomatry(const std::string &resourceDirectory) {
    cube = make_shared<Shape>();
    cube->loadMesh(resourceDirectory + "/sphere.obj");
    cube->resize(1);
    cube->init();

    box = make_shared<Shape>();
    box->loadMesh(resourceDirectory + "/cube.obj");
    box->resize(10);
    box->init();

    ship = make_shared<Shape>();
    ship->loadMesh(resourceDirectory + "/ship.obj");
    ship->resize(1);
    ship->init();

    floor = make_shared<Shape>();
    floor->loadFloorGeom();
    floor->init();

    quad = make_shared<Shape>();
    quad->loadQuadGeom();
    quad->init();
}

void Application::initTextures(const std::string &resourceDirectory) {
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

    create_cube_map(
            resourceDirectory + "/skybox/Box_Back.jpg",
            resourceDirectory + "/skybox/Box_Front.jpg",
            resourceDirectory + "/skybox/Box_Top.jpg",
            resourceDirectory + "/skybox/Box_Bottom.jpg",
            resourceDirectory + "/skybox/Box_Left.jpg",
            resourceDirectory + "/skybox/Box_Right.jpg",
            &texSkybox);

    texture0 = make_shared<Texture>();
    texture0->setFilename(resourceDirectory + "/Ball1.jpg");
    texture0->init();
    texture0->setUnit(1);
    texture0->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    for (int i = 0; i <= 15; i++) {
        ballTexture[i] = make_shared<Texture>();
//        string filename = i == 0 ? resourceDirectory + "/balls/BallCue.jpg" :resourceDirectory + "/balls/Ball"+i+".jpg";
        string filename = resourceDirectory + "/balls/BallCue.jpg";
//        cout << filename << "\n";
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

    // init FBOs
    largeRender = make_shared<Texture>();
    leftSplitScreen = make_shared<Texture>();
    rightSplitScreen = make_shared<Texture>();
    shadowMap = make_shared<Texture>();
    resizeCallback(windowManager->getHandle(), width, height);
}
void Application::create_cube_map( string front, string back, string top, string bottom, string left, string right, GLuint* tex_cube) {
    // generate a cube-map texture to hold all the sides
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, tex_cube);

    // load each image and copy into a side of the cube-map texture
    load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front.data());
    load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back.data());
    load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top.data());
    load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom.data());
    load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left.data());
    load_cube_map_side(*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right.data());
    // format cube map texture
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
bool Application::load_cube_map_side(GLuint texture, GLenum side_target, const char* file_name) {
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int x, y, n;
    int force_channels = 4;
    unsigned char*  image_data = stbi_load(
            file_name, &x, &y, &n, force_channels);
    if (!image_data) {
        fprintf(stderr, "ERROR: could not load %s\n", file_name);
        return false;
    }
    // non-power-of-2 dimensions check
    if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
        fprintf(stderr,
                "WARNING: image %s is not power-of-2 dimensions\n",
                file_name);
    }

    // copy image data into 'target' side of cube map
    glTexImage2D( side_target, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    free(image_data);
    return true;
}

// delte this
void Application::createFBO(GLuint &fb, GLuint &tex) {
    //initialize FBO
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);

    //set up framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    //set up texture
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        cout << "Error setting up frame buffer - exiting" << endl;
        exit(0);
    }
}

// draw2dBuff
void Application::drawTV(GLuint inTex, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frameBuf[0]);

    // example applying of 'drawing' the FBO texture - change shaders
    texProg->bind();
    glUniform1i(texProg->getUniform("texBuf"), 0);
    glUniformMatrix4fv(texProg->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
    glUniformMatrix4fv(texProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
    glUniformMatrix4fv(texProg->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));

    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    glUniform1i(texProg->getUniform("width"), width);
    glUniform1i(texProg->getUniform("height"), height);

//    glEnableVertexAttribArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
//    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glDisableVertexAttribArray(0);
    quad->draw(texProg);

    texProg->unbind();
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

    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    glViewport(0, 0, width, height);
    float aspect = width / (float) (height / 2.0f);
    P->pushMatrix();
    P->perspective(45.0f, aspect, 0.01f, 100.0f);

    O->pushMatrix();
    aspect /= 2;
    if (width > height) {
        O->ortho(-1 * aspect, 1 * aspect, -1, 1, -2, 100.0f);
    } else {
        O->ortho(-1, 1, -1 * 1 / aspect, 1 * 1 / aspect, -2, 100.0f);
    }

    int downsampleScale = 2;

    int numPlayers = 1;
    if (true) {
        GLint largeBuffer = largeRender->getID();

        // render to buffer
        V->pushMatrix();
        V->multMatrix(p1->getViewMatrix());
        renderScene(actors, numActors, largeBuffer, M, V, P); // COMMENTED FOR PERFORMANCE
        V->popMatrix();

        // TODO: downscale

        V->pushMatrix();
        V->multMatrix(p1->getViewMatrix());
        renderScene(actors, numActors, 0, M, V, P);
        V->popMatrix();
    } else {
        Texture *leftSplit = new Texture();
        leftSplit->setDimensions(width/2, height);
        leftSplit->setUnit(20);
        leftSplit->init();
        GLint leftBuffer = leftSplit->getID();

        Texture *rightSplit = new Texture();
        rightSplit->setDimensions(width/2, height);
        leftSplit->setUnit(21);
        rightSplit->init();
        GLint rightBuffer = rightSplit->getID();

        V->pushMatrix();
        V->multMatrix(p1->getViewMatrix());
        renderScene(actors, numActors, frameBuf[0], M, V, P);
        V->popMatrix();

        M->pushMatrix();
        V->pushMatrix();
        M->loadIdentity();
        V->loadIdentity();
        V->lookAt(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0));
        M->scale(vec3(2, .48, 1));
        M->translate(vec3(0, 1, 0));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawTV(frameBuf[0], M, V, O);
        V->popMatrix();
        M->popMatrix();

        V->pushMatrix();
        V->multMatrix(p2->getViewMatrix());
        renderScene(actors, numActors, frameBuf[0], M, V, P);
        V->popMatrix();

        M->pushMatrix();
        V->pushMatrix();
        M->loadIdentity();
        M->loadIdentity();

        V->loadIdentity();
        V->lookAt(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, 1, 0));
        M->scale(vec3(2, .48, 1));
        M->translate(vec3(0, -1, 0));
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawTV(frameBuf[0], M, V, O);
        V->popMatrix();
        M->popMatrix();
    }

    P->popMatrix();
}

void Application::renderScene(PxActor **actors, int numActors, GLuint buffer, shared_ptr<MatrixStack> M,
                              shared_ptr<MatrixStack> V,
                              shared_ptr<MatrixStack> P) {
    glBindFramebuffer(GL_FRAMEBUFFER, buffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    skyProg->bind();
    {
        glUniformMatrix4fv(skyProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        V->pushMatrix();
        V->loadIdentity();
        V->multMatrix(p1->getSkyBoxViewMatrix());
        glUniformMatrix4fv(skyProg->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
        V->popMatrix();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texSkybox);
        glUniform1i(skyProg->getUniform("skyTexture"), 0);
        glDepthMask(GL_FALSE);

        box->draw(skyProg);
        glDepthMask(GL_TRUE);
    }
    skyProg->unbind();

    prog->bind();
    {
        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));

        M->pushMatrix();
        M->scale(vec3(4, 1, 4));
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
        setMaterial(4);
        floor->draw(prog);
        M->popMatrix();

//        M->pushMatrix();
//        M->loadIdentity();
//        M->translate(p1->getPosition());
//        M->scale(.3);
//        M->rotate(p1->getTheta() - M_PI_2, vec3(0, 1, 0));
//        M->rotate(radians(90.f), vec3(1, 0, 0));
//        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
//        ship->draw(prog);
//        M->popMatrix();
////
//        M->pushMatrix();
//        M->loadIdentity();
//        M->translate(p2->getPosition());
//        M->scale(.3);
//        M->rotate(p2->getTheta() - M_PI_2, vec3(0, 1, 0));
//        M->rotate(radians(90.f), vec3(1, 0, 0));
//        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
//        ship->draw(prog);
//       M->popMatrix();
    }
    prog->unbind();

    renderActors(actors, numActors, V, P);
}

void Application::renderActors(PxActor **actors, int numActors, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P) {
    static const int MAX_SHAPES = 100;
    PxShape *shapes[10];
    for (int i = 0; i < numActors; i++) {
        PxRigidActor *actor = actors[i]->is<PxRigidActor>();
        int nbShapes = actor->getNbShapes();
        void *data = actor->userData;

        actor->getShapes(shapes, nbShapes);
        bool sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;

        for (int j = 0; j < nbShapes; j++) {
            const PxMat44 shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actor));

            texProg->bind();
            {
                glUniformMatrix4fv(texProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
                glUniformMatrix4fv(texProg->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
                glUniformMatrix4fv(texProg->getUniform("M"), 1, GL_FALSE, shapePose.front());

                texture0->bind(texProg->getUniform("diffuseTexture"));
                specularTexture->bind(texProg->getUniform("specularTexture"));

                cube->draw(texProg);
            }
            texProg->unbind();

        }
    }

}
