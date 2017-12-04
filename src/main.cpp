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

    Physics *physics = new Physics();
    int numSpheres = 16;
    UserData spheres[numSpheres+1];
    spheres[0].ballNum = 0;
    physics->addSphere(PxVec3(10, 3, 10), &spheres[0]);

    double xCenter = 0;
    double zCenter = 0;
    double xOffset = 1;
    double zOffset = 1.8;
    double ballDiameter = 2.05;

    int ballNum = 1;
    for (int i = 5; i >= 1; i--) {
        for (int j = 1; j <= i; j++) {
            spheres[ballNum].ballNum = ballNum;
            double xPos = xCenter + (5-i)*xOffset + (j-1)*ballDiameter;
            double zPos = zCenter + (5-i)*zOffset;
            physics->addSphere(PxVec3(xPos, .1, zPos), &spheres[ballNum]);
            ballNum++;
        }
    }

//    for (int i = 1; i < numSpheres; i++) {
//        spheres[i].ballNum = i;
//        physics->addSphere(PxVec3(0, 2 * i, 0), &spheres[i]);
//    }

    physics->addGround();
    PxActor *actors[400];

    while (!glfwWindowShouldClose(windowManager->getHandle())) {

        if (physics->advance((PxReal) glfwGetTime())) {
            glfwSetTime(0);
            physics->fetchResults();
            int numActors = physics->getActors(actors);

            application->render(actors, numActors);

            glfwSwapBuffers(windowManager->getHandle());
            glfwPollEvents(); // do this while we are waiting for physics
        }
    }

    windowManager->shutdown();
    return 0;
};
