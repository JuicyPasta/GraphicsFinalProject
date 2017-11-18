//
// Created by Darrow, Jackson on 11/17/17.
//

#include "Application.h"

void Application::scrollCallback(GLFWwindow *window, double deltaX, double deltaY) {

}

void Application::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouseDown = true;
        Moving = true;
    }

    if (action == GLFW_RELEASE) {
        Moving = false;
        mouseDown = false;
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

void Application::addVars(std::shared_ptr<Program> prog) {
    prog->addUniform("P");
    prog->addUniform("V");
    prog->addUniform("M");
    prog->addUniform("diffuse");
    prog->addUniform("specular");
    prog->addUniform("shiny");
    prog->addUniform("ambient");
    prog->addUniform("source");
    prog->addUniform("sourceColor");

    prog->addAttribute("vertPos");
    prog->addAttribute("vertNor");
    prog->addAttribute("eye");
}

void Application::init(const std::string &resourceDirectory) {
    int width, height;
    glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
    GLSL::checkVersion();


    // Set background color.
    glClearColor(.1f, .1f, .1f, 1.0f);
    // Enable z-buffer test.
    glEnable(GL_DEPTH_TEST);

    // PHONG
    prog = make_shared<Program>();
    prog->setVerbose(true);
    prog->setShaderNames(resourceDirectory + "/vert_phong.glsl", resourceDirectory + "/frag_phong.glsl");
    prog->init();
    prog->addAttribute("L");
    prog->addAttribute("E");
    prog->addAttribute("N");
    addVars(prog);

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

    //set up the shaders to blur the FBO just a placeholder pass thru now
    //next lab modify and possibly add other shaders to complete blur
    texProg = make_shared<Program>();
    texProg->setVerbose(true);
    texProg->setShaderNames(
            resourceDirectory + "/tex_vert.glsl",
            resourceDirectory + "/tex_frag.glsl");
    if (!texProg->init()) {
        std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
        exit(1);
    }
    addVars(texProg);
    texProg->addUniform("specularTexture");
    texProg->addUniform("diffuseTexture");
    texProg->addAttribute("vertTex");
    texProg->addAttribute("L");
    texProg->addAttribute("E");
    texProg->addAttribute("N");

    p1 = make_shared<Player>(0);
    p2 = make_shared<Player>(0);
}

void Application::initGeom(const std::string &resourceDirectory) {
    cube = make_shared<Shape>();
    cube->loadMesh(resourceDirectory + "/sphere.obj");
    cube->resize();
    cube->init();


    ship = make_shared<Shape>();
    ship->loadMesh(resourceDirectory + "/ship.obj");
    ship->resize();
    ship->init();

    //Initialize the geometry to render a quad to the screen
    initQuad();
    initFloor();
}


void Application::initTex(const std::string &resourceDirectory) {
    texture0 = make_shared<Texture>();
    texture0->setFilename(resourceDirectory + "/Ball1.jpg");
    texture0->init();
    texture0->setUnit(0);
    texture0->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

    specularTexture = make_shared<Texture>();
    specularTexture->setFilename(resourceDirectory + "/shiny_metal_specular.jpg");
    specularTexture->init();
    specularTexture->setUnit(1);
    specularTexture->setWrapModes(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
}

void Application::initQuad() {
    //now set up a simple quad for rendering FBO
    glGenVertexArrays(1, &quad_VertexArrayID);
    glBindVertexArray(quad_VertexArrayID);

    static const GLfloat g_quad_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
    };

    glGenBuffers(1, &quad_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

void Application::initFloor() {
    float g_groundSize = 20;
    float g_groundY = -1.5;

    // A x-z plane at y = g_groundY of dim[-g_groundSize, g_groundSize]^2
    float GrndPos[] = {
            -g_groundSize, g_groundY, -g_groundSize,
            -g_groundSize, g_groundY, g_groundSize,
            g_groundSize, g_groundY, g_groundSize,
            g_groundSize, g_groundY, -g_groundSize
    };

    float GrndNorm[] = {
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0,
            0, 1, 0
    };

    float GrndTex[] = {
            0, 0, // back
            0, 1,
            1, 1,
            1, 0
    };

    unsigned short idx[] = {0, 1, 2, 0, 2, 3};

    GLuint VertexArrayID;
    //generate the VAO
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    gGiboLen = 6;
    glGenBuffers(1, &GrndBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);

    glGenBuffers(1, &GrndNorBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndNorm), GrndNorm, GL_STATIC_DRAW);

    glGenBuffers(1, &GrndTexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndTex), GrndTex, GL_STATIC_DRAW);

    glGenBuffers(1, &GIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);
}

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

void Application::drawTV(GLuint inTex, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P) {
    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
    texProg->unbind();
}

void Application::renderGround() {
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, GrndNorBuffObj);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, GrndTexBuffObj);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // draw!
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
    glDrawElements(GL_TRIANGLES, gGiboLen, GL_UNSIGNED_SHORT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
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

    int numPlayers = 1;
    if (numPlayers == 1) {
        V->pushMatrix();
        V->multMatrix(p1->getViewMatrix());
        renderScene(actors, numActors, 0, M, V, P);
        V->popMatrix();
    } else {
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

    prog->bind();
    {
        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));

        M->pushMatrix();
        M->scale(vec3(4, 1, 4));
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
        setMaterial(4);
        renderGround();
        M->popMatrix();

//        M->pushMatrix();
//        M->loadIdentity();
//        M->translate(p1->getPosition());
//        M->scale(.3);
//        M->rotate(p1->getTheta(), vec3(0, 1, 0));
//        M->rotate(radians(90.f), vec3(1, 0, 0));
//        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
//        ship->draw(prog);
//        M->popMatrix();

//        M->pushMatrix();
//        M->loadIdentity();
//        M->translate(p2->getPosition());
//        M->scale(.3);
//        M->rotate(p2->getTheta(), vec3(0, 1, 0));
//        M->rotate(radians(90.f), vec3(1, 0, 0));
//        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
//        ship->draw(prog);
        M->popMatrix();
    } prog->unbind();

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
            PxGeometryHolder h = shapes[j]->getGeometry();

//            glm::mat4 *M = new mat4(shapePose.front());

            texProg->bind();
            {
                glUniformMatrix4fv(texProg->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
                glUniformMatrix4fv(texProg->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
                glUniformMatrix4fv(texProg->getUniform("M"), 1, GL_FALSE, shapePose.front());

                texture0->bind(texProg->getUniform("diffuseTexture"));
                specularTexture->bind(texProg->getUniform("specularTexture"));

                cube->draw(texProg);
            } texProg->unbind();

        }
    }

}

