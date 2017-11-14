//
// Created by Darrow, Jackson on 11/11/17.
//

#ifndef INC_3DBILLIARDS_PLAYER_H
#define INC_3DBILLIARDS_PLAYER_H

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/vec2.hpp>

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
    vec3 getPosition() {
        return position;
    }
	float getAngle() {
		return theta;
	}

private:
    int input;

    vec3 position = vec3(0, 0, 0);
    vec3 direction = vec3(0, 0, -1);
	float theta = 3.14f;
    vec3 lookAtPoint = vec3(0, 0, 1);
    vec3 upVector = vec3(0, 1, 0);

    // x, y, z
    vec3 strafeInput = vec3(0, 0, 0);
    // theta, phi, omega
    vec3 orientationInput = vec3(0, 0, 0);
};


#endif //INC_3DBILLIARDS_PLAYER_H
