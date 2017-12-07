#include <iostream>

#include "WindowManager.h"
#include "Physics.h"
#include "Application.h"

int main(int argc, char **argv) {
    string resourceDir = "../resources";

    if (argc >= 2) {
        resourceDir = argv[1];
    }

    Application *application = new Application();

    WindowManager *windowManager = new WindowManager();
    windowManager->init(1024, 512);
    windowManager->setEventCallbacks(application);
    application->windowManager = windowManager;

    application->init();
    application->initShaders(resourceDir);
    application->initTextures(resourceDir);
    application->initGeometry(resourceDir);

    shared_ptr<Material> materials[100];


    for (int i = 0; i < 17; i++) {
        materials[i] = make_shared<Material>();
        materials[i]->shader = application->texProg;
        materials[i]->diffuseTex = application->ballTexture[i + 1];
    }

    Physics *physics = new Physics();
    int numSpheres = 16;
    UserData spheres[numSpheres+1];

    spheres[0].materialMap.insert({PxGeometryType::eBOX, materials[0]});
    spheres[1].materialMap.insert({PxGeometryType::eBOX, materials[1]});

    physics->addSphere(PxVec3(40, 3, 0), &spheres[0], 100, 100, .5);
    physics->addSphere(PxVec3(-40, 3, 0), &spheres[1], 100, 100, .5);

    double xCenter = 0;
    double zCenter = -5;
    double xOffset = 1;
    double zOffset = sqrt(3.0f);
    double ballDiameter = 2;


    int ballNum = 1;
    for (int i = 5; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            spheres[ballNum].ballNum = ballNum;
            double xPos = xCenter + (5-i)*xOffset + (j-1)*ballDiameter;
            double zPos = zCenter + (5-i)*zOffset;
            physics->addSphere(PxVec3(xPos, 50, zPos), &spheres[ballNum + 1], .5, .5, .5);
            ballNum++;
        }
    }

    physics->addGround();
    PxActor *actors[400];


    while (!glfwWindowShouldClose(windowManager->getHandle())) {

        if (physics->advance((PxReal) glfwGetTime())) {
            glfwSetTime(0);
            physics->fetchResults();
            int numActors = physics->getActors(actors);

            application->initPlayers(actors, 0, 1);

            application->render(actors, numActors);

            glfwSwapBuffers(windowManager->getHandle());
            glfwPollEvents(); // do this while we are waiting for physics
        }
    }

    windowManager->shutdown();
    return 0;
};
