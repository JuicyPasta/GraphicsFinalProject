//
// Created by Darrow, Jackson on 11/11/17.
//

#ifndef INC_3DBILLIARDS_PLAYER_H
#define INC_3DBILLIARDS_PLAYER_H

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/vec2.hpp>
#include <PxPhysicsAPI.h>
#include <foundation/PxVec2.h>
#include <foundation/PxVec3.h>


using namespace physx;
using namespace glm;

class Player {
public:
    Player(int input);

    // input
    void keyboardInputCB(GLFWwindow *window, int key, int scancode, int action, int mods);
    void mouseInputCB(GLFWwindow *window, double x, double y);

    void getControllerInput();

    void update(float deltaTime);
    mat4 getViewMatrix();
    mat4 getSkyBoxViewMatrix();
    vec3 getPosition() {
        if (physX) return vec3(physX->getGlobalPose().p.x,physX->getGlobalPose().p.y,physX->getGlobalPose().p.z);
        return position;
    }
    float getTheta() {
        return orientationInput.x;
    }

    PxRigidDynamic *physX;

private:
    int input;

    vec3 position = vec3(0, 0, 0);
    vec3 lookAtPoint = vec3(0, 0, -1);
    vec3 upVector = vec3(0, 1, 0);

    // x, y, z
    vec3 strafeInput = vec3(0, 0, 0);
    // theta, phi, omega
    vec3 orientationInput = vec3(0, 0, 0);

    double lastMouseX = 0;
    double lastMouseY = 0;
};


#endif //INC_3DBILLIARDS_PLAYER_H
