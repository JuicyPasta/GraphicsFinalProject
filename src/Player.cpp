//
// Created by Darrow, Jackson on 11/11/17.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Player.h"

Player::Player(int input) {
    this->input = input;

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
    orientationInput.x = -x / 1000;
    orientationInput.y = -y / 1000;
}

void Player::getControllerInput() {
    using namespace std;
    int count;
    const float *axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
    float deadzone = .2;

    strafeInput.x = abs(axes[0]) > deadzone ? axes[0] : 0;
#ifdef _WIN32
	strafeInput.y = abs(axes[1]) > deadzone ? axes[1] : 0;
#else
    strafeInput.y = abs(axes[1]) > deadzone ? -axes[1] : 0;
#endif

    orientationInput.x -= abs(axes[2]) > deadzone ? axes[2]/10 : 0;
    orientationInput.y -= abs(axes[3]) > deadzone ? axes[3]/10 : 0;

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

    vec3 delta = vec3(0, 0, 0);
	direction.x = sin(theta);
	direction.y = cos(theta);
    delta -= strafeSpeed * strafeInput.y * direction;
	theta += strafeSpeed * -strafeInput.x * .1f;

    //delta += strafeSpeed * -strafeInput.x * glm::cross(upVector, direction);
    float posy = position.y;
    position += delta;
    position.y = posy;
//    lookAtPoint = position + direction;

    if (orientationInput.y > .9)
        orientationInput.y = .9;
    if (orientationInput.y < -.9)
        orientationInput.y = -.9;

    lookAtPoint = position + glm::vec3(cos(orientationInput.x + theta) * cos(orientationInput.y),
                                       sin(orientationInput.y),
                                       -cos(orientationInput.y) * sin(orientationInput.x + theta));

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
	return glm::lookAt(lookAtPoint, position, upVector);
    //return glm::lookAt(position, lookAtPoint, upVector);
}
