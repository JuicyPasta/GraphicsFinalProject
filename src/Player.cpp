//
// Created by Darrow, Jackson on 11/11/17.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Player.h"

#ifdef GLM_PLATFORM_LINUX
#define LEFT_STICK_X 0
#define LEFT_STICK_Y 1
#define RIGHT_STICK_X 3
#define RIGHT_STICK_Y 4
#elif GLM_PLATFORM_WINDOWS
#define LEFT_STICK_X 0
#define LEFT_STICK_Y 1
#define RIGHT_STICK_X 2
#define RIGHT_STICK_Y 3
#endif




Player::Player(int input) {
    this->input = input;
    //std::cout << "hi\n";
    if (input > 0) {
        int present = glfwJoystickPresent(GLFW_JOYSTICK_1 + input - 1);
        if(!present) {
            std::cout << "PLEASE CONNECT CONTROLLER\n";
        }
    }
}

void Player::keyboardInputCB(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        strafeInput.y = 1;
    } else if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
        strafeInput.y = 0;
    } else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
        strafeInput.y = -1;
    } else if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
        strafeInput.y = 0;
    } else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        strafeInput.x = -1;
    } else if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
        strafeInput.x = 0;
    } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        strafeInput.x = 1;
    } else if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
        strafeInput.x = 0;
    }
}

void Player::mouseInputCB(GLFWwindow *window, double x, double y) {

    double dx = lastMouseX - x;
    double dy = lastMouseY - y;

    orientationInput.x += dx / 1000;
    orientationInput.y += dy / 1000;

    lastMouseX = x;
    lastMouseY = y;
}

void Player::getControllerInput() {
    using namespace std;
    int count;
    const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
    float deadzone = .2;

    strafeInput.x = abs(axes[LEFT_STICK_X]) > deadzone ? axes[LEFT_STICK_X] : 0;
#ifdef _WIN32
	strafeInput.y = abs(axes[1]) > deadzone ? axes[LEFT_STICK_X] : 0;
#else
    strafeInput.y = abs(axes[LEFT_STICK_Y]) > deadzone ? -axes[LEFT_STICK_Y] : 0;
#endif

#ifdef GLM_PLATFORM_LINUX
    orientationInput.x += abs(axes[RIGHT_STICK_X]) > deadzone ? axes[RIGHT_STICK_X]/10 : 0;
    orientationInput.y -= abs(axes[RIGHT_STICK_Y]) > deadzone ? axes[RIGHT_STICK_Y]/10 : 0;
    //printf("%f\n", axes[2]);

#else
    orientationInput.x -= abs(axes[2]) > deadzone ? axes[RIGHT_STICK_X]/10 : 0;
    orientationInput.y -= abs(axes[3]) > deadzone ? axes[RIGHT_STICK_Y]/10 : 0;
#endif

    // axes[4] = left trigger
    // axes[5] = right trigger
//    const unsigned char *buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);
//    for (int i = 0; i < count; i++) {
//        cout << "\t" << buttons[i];
//    }
//    cout << "\n";
}

void Player::update(float deltaTime) {
    if (input > 0) {

        this->getControllerInput();
    }

    float strafeSpeed = .5;

    if (orientationInput.x > 2*3.14159) orientationInput.x -= 3.14159*2;
    if (orientationInput.x < -0.0) orientationInput.x += 3.14159*2;
/**
    if(strafeInput.y > 0) {
        orientationInput.x -= .00001;
    }


    vec3 delta = vec3(0, 0, 0);
    theta += strafeSpeed * -strafeInput.x * .1f;
    if (theta > 2*3.14159) theta -= 3.14159*2;
    if (theta < 0) theta -= 3.14159*2;
    direction.x = sin(theta);
	direction.y = cos(theta);
    delta -= strafeSpeed * strafeInput.y * direction;

**/
    //delta += strafeSpeed * -strafeInput.x * glm::cross(upVector, direction);
    vec3 direction = vec3(0, 0, 0);
    direction += strafeSpeed * strafeInput.y * (lookAtPoint - position);
    direction += strafeSpeed * -strafeInput.x * glm::cross(upVector, lookAtPoint - position);

    float posy = position.y;
    position += direction;
    position.y = posy;
//    lookAtPoint = position + direction;

    if (orientationInput.y > .9)
        orientationInput.y = .9f;
    if (orientationInput.y < -.9)
        orientationInput.y = -.9f;

    std::cout << orientationInput.x << std::endl;

    lookAtPoint = position + glm::vec3(cos(orientationInput.x) * cos(orientationInput.y),
                                       sin(orientationInput.y),
                                       -cos(orientationInput.y) * sin(orientationInput.x));

//      vec3 viewVec = lookAtPoint - position;

    // update phi
//      vec3 phiAxis = glm::cross(viewVec, upVector);
//			mat4 rotatePhi = glm::rotate(glm::mat4(1.0), (float)deltaPhi, phiAxis);
//			mat4 rotate90 = glm::rotate(glm::mat4(1.0), (float)M_PI/2, phiAxis);
//      viewVec = vec3(rotatePhi * vec4(viewVec, 1));
//			upVector = vec3(rotate90 * vec4(viewVec, 1));

    // update theta
//			mat4 rotateTheta = glm::rotate(glm::mat4(1.0), (float)deltaTheta, upVector);
//			viewVec = vec3((rotateTheta * vec4(viewVec, 1)));
//			viewVec = glm::normalize(viewVec);
//			upVector = glm::normalize(upVector);
}

mat4 Player::getViewMatrix() {
//	return glm::lookAt(lookAtPoint, position, upVector);
    return glm::lookAt(position, lookAtPoint, upVector);
}

mat4 Player::getSkyBoxViewMatrix() {
//    return glm::lookAt(lookAtPoint-position, vec3(0, 0, 0), upVector);
    return glm::lookAt(vec3(0, 0, 0), lookAtPoint - position, upVector);
}
