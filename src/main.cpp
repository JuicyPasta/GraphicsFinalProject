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

    application->init(resourceDir);
    application->initGeom(resourceDir);
    application->initTex(resourceDir);

    Physics *physics = new Physics();
    physics->addSphere(PxVec3(0, 3, 0));
    for (int i = 3; i < 400; i++) {
        physics->addSphere(PxVec3(0, 2 * i, 0));

    }
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
