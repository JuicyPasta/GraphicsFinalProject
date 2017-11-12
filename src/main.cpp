/*
Base code
Currently will make 2 FBOs and textures (only uses one in base code)
and writes out frame as a .png (Texture_output.png)

Winter 2017 - ZJW (Piddington texture write)
2017 integration with pitch and yaw camera lab (set up for texture mapping lab)
*/

#include <iostream>
#include <glad/glad.h>

#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"
#include "Shape.h"
#include "WindowManager.h"
#include "GLTextureWriter.h"
#include "Player.h"

// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

class Application : public EventCallbacks {

public:

    WindowManager *windowManager = nullptr;

    // Our shader program
    std::shared_ptr<Program> prog;
    std::shared_ptr<Program> texProg;

    shared_ptr<Player> p1, p2;

    // Shape to be used (from obj file)
    shared_ptr<Shape> cube;

    // Contains vertex information for OpenGL
    GLuint VertexArrayID;

    // Data necessary to give our triangle to OpenGL
    GLuint VertexBufferID;

    //geometry for texture render
    GLuint quad_VertexArrayID;
    GLuint quad_vertexbuffer;

    //ground plane info
    GLuint GrndBuffObj, GrndNorBuffObj, GrndTexBuffObj, GIndxBuffObj;
    int gGiboLen;

    //reference to texture FBO
    GLuint frameBuf[3];
    GLuint texBuf[3];
    GLuint depthBuf;

    bool FirstTime = true;
    bool Moving = false;
    int gMat = 0;

    bool mouseDown = false;

    void scrollCallback(GLFWwindow *window, double deltaX, double deltaY) {
    }

    void mouseCallback(GLFWwindow *window, int button, int action, int mods) {
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

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

        p1->keyboardInputCB(window, key, scancode, action, mods);
    }

    void cursorPositionCallback(GLFWwindow *window, double x, double y) {
        p1->mouseInputCB(window, x, y);
    }

    void resizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void setMaterial(int i) {
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

    void addVars(std::shared_ptr<Program> prog) {
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
        prog->addAttribute("partColor");
        prog->addAttribute("eye");
        prog->addAttribute("L");
        prog->addAttribute("H");
    }

    void init(const std::string &resourceDirectory) {
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
        texProg->addUniform("texBuf");
        texProg->addUniform("dir");
        texProg->addUniform("width");
        texProg->addUniform("height");

        p1 = make_shared<Player>(0);
        p2 = make_shared<Player>(1);
    }

    void initGeom(const std::string &resourceDirectory) {
        cube = make_shared<Shape>();
        cube->loadMesh(resourceDirectory + "/cube.obj");
        cube->resize();
        cube->init();

        //Initialize the geometry to render a quad to the screen
        initQuad();
        initFloor();
    }

    /**** geometry set up for a quad *****/
    void initQuad() {
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

    /**** geometry set up for ground plane *****/
    void initFloor() {
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

    /* Helper function to create the framebuffer object and
        associated texture to write to */
    void createFBO(GLuint &fb, GLuint &tex) {
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

    void drawTV(GLuint inTex, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P) {
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

    void renderGround() {
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

    void render() {
        double seconds = glfwGetTime();
        p1->update(seconds);
        p2->update(seconds);
        glfwSetTime(0);

        auto P = make_shared<MatrixStack>();
        auto M = make_shared<MatrixStack>();
        auto V = make_shared<MatrixStack>();
        V->loadIdentity();

        int width, height;
        glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
        glViewport(0, 0, width, height);
        float aspect = width / (float) height;
        P->pushMatrix();
        P->perspective(45.0f, aspect, 0.01f, 100.0f);

        int numPlayers = 2;
        if (numPlayers == 1) {
            V->pushMatrix();
            V->multMatrix(p1->getViewMatrix());
            render_scene(0, M, V, P);
            V->popMatrix();
        } else {
            V->pushMatrix();
                V->multMatrix(p1->getViewMatrix());
                render_scene(frameBuf[0], M, V, P);
            V->popMatrix();

            M->pushMatrix();
            V->pushMatrix();
                M->loadIdentity();
                M->translate(vec3(-.55,0,-1));
                V->loadIdentity();
                V->lookAt(vec3(0,0,0),vec3(0,0,-1),vec3(0,1,0));
                M->scale(vec3(.5, .5, 1));
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            drawTV(frameBuf[0], M, V, P);
            V->popMatrix();
            M->popMatrix();

            V->pushMatrix();
                V->multMatrix(p2->getViewMatrix());
                render_scene(frameBuf[0], M, V, P);
            V->popMatrix();


            M->pushMatrix();
            V->pushMatrix();
            M->loadIdentity();
            M->translate(vec3(+.55,0,-1));
            V->loadIdentity();
            V->lookAt(vec3(0,0,0),vec3(0,0,-1),vec3(0,1,0));
            M->scale(vec3(.5, .5, 1));
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            drawTV(frameBuf[0], M, V, P);
            V->popMatrix();
            M->popMatrix();
        }


        P->popMatrix();
    }

    void render_scene(GLuint buffer, shared_ptr<MatrixStack> M, shared_ptr<MatrixStack> V, shared_ptr<MatrixStack> P) {
        glBindFramebuffer(GL_FRAMEBUFFER, buffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        prog->bind();
        M->pushMatrix();
        M->scale(vec3(4, 1, 4));
        glUniformMatrix4fv(prog->getUniform("M"), 1, GL_FALSE, value_ptr(M->topMatrix()));
        glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, value_ptr(P->topMatrix()));
        glUniformMatrix4fv(prog->getUniform("V"), 1, GL_FALSE, value_ptr(V->topMatrix()));
        setMaterial(4);
        renderGround();
        prog->unbind();
        M->popMatrix();
    }
};

int main(int argc, char **argv) {
    // Where the resources are loaded from
    std::string resourceDir = "../resources";

    if (argc >= 2) {
        resourceDir = argv[1];
    }

    Application *application = new Application();

    // Your main will always include a similar set up to establish your window
    // and GL context, etc.

    WindowManager *windowManager = new WindowManager();
    windowManager->init(2048, 1024);
    windowManager->setEventCallbacks(application);
    application->windowManager = windowManager;

    // This is the code that will likely change program to program as you
    // may need to initialize or set up different data and state

    application->init(resourceDir);
    application->initGeom(resourceDir);

    // Loop until the user closes the window.
    while (!glfwWindowShouldClose(windowManager->getHandle())) {
        // Render scene.
        application->render();

        // Swap front and back buffers.
        glfwSwapBuffers(windowManager->getHandle());
        // Poll for and process events.
        glfwPollEvents();
    }

    // Quit program.
    windowManager->shutdown();
    return 0;
}
