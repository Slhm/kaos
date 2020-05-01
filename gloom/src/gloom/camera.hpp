#ifndef CAMERA_HPP
#define CAMERA_HPP
#pragma once

// System headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>


const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
//bool wireframeMode = false;

namespace Gloom
{
    class Camera
    {
    public:

        glm::vec3 Up;
        glm::vec3 Right;

        Camera(glm::vec3 position         = glm::vec3(0.0f, 0.0f, 0.0f),
               GLfloat   movementSpeed    = 30.0f,
               GLfloat   mouseSensitivity = 0.05f
        )
        {
            cPosition         = position;
            cMovementSpeed    = movementSpeed;
            cMouseSensitivity = mouseSensitivity;

            // Set up the initial view matrix
            updateViewMatrix();
        }

        // Public member functions

        /* Getter for the view matrix */
        glm::mat4 getViewMatrix() { return matView; }
        glm::vec3 getViewDir() { return cfront; }
        glm::vec3 getCamPos() { return cPosition; }


        /* Handle keyboard inputs from a callback mechanism */
        void handleKeyboardInputs(int key, int action)
        {
            // Keep track of pressed/released buttons
            if (key >= 0 && key < 512)
            {
                if (action == GLFW_PRESS)
                {
                    keysInUse[key] = true;
                }
                else if (action == GLFW_RELEASE)
                {
                    keysInUse[key] = false;
                }
            }
        }


        /* Handle mouse button inputs from a callback mechanism */
        void handleMouseButtonInputs(int button, int action)
        {
            // Ensure that the camera only rotates when the left mouse button is
            // pressed
            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            {
                isMousePressed = true;
            }
            else
            {
                isMousePressed = false;
                resetMouse = true;
            }
        }


        /* Handle cursor position from a callback mechanism */
        void handleCursorPosInput(double xpos, double ypos)
        {
            // Keep track of pitch and yaw for the current frame
            float xOff  = 0;
            float yOff = 0;
            if (!firstMouse)
            {
                xOff  = xpos - lastXPos;
                yOff = lastYPos - ypos;
                firstMouse = false;
            }

            // Update last known cursor position
            lastXPos = xpos;
            lastYPos = ypos;

            // Adjust cursor movement using the specified sensitivity
            xOff *= cMouseSensitivity;
            yOff *= cMouseSensitivity;

            fYaw += xOff;
            fPitch += yOff;

            // Caps the pitch hight at 90 deg.
            if(fPitch > 89.0f) fPitch = 89.0f;
            if(fPitch < -89.0f) fPitch = -89.0f;

            updateViewMatrix();
        }


        /* Update the camera position and view matrix
           `deltaTime` is the time between the current and last frame */
        void updateCamera(GLfloat deltaTime)
        {

            glm::vec3 move = glm::vec3(0);
            if (keysInUse[GLFW_KEY_W]) {
                move +=  glm::vec3(cfront.x, 0, cfront.z);
            }
            if (keysInUse[GLFW_KEY_S]) {
                move -=  glm::vec3(cfront.x, 0, cfront.z);
            }
            if (keysInUse[GLFW_KEY_A]) {
                move -=  glm::normalize(glm::cross(cfront, glm::vec3(0, 1, 0)));
                move.y = 0;
            }
            if (keysInUse[GLFW_KEY_D]) {
                move +=  glm::normalize(glm::cross(cfront, glm::vec3(0, 1, 0)));
                move.y = 0;
            }
            if (keysInUse[GLFW_KEY_R]) {
                cPosition = glm::vec3(0);
            }
            /*
            if(keysInUse[GLFW_KEY_P]){
                if(!wireframeMode) {
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                    wireframeMode = true;
                }else{
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    wireframeMode = false;
                }

            }
            */
            // Trick to balance PC speed with movement
            GLfloat velocity = cMovementSpeed * deltaTime;

            // Update camera position using the appropriate velocity
            cPosition += move * velocity;

            // Update the view matrix based on the new information
            updateViewMatrix();
        }

    private:
        // Disable copying and assignment
        Camera(Camera const &) = delete;
        Camera & operator =(Camera const &) = delete;

        // Private member function

        /* Update the view matrix based on the current information */
        void updateViewMatrix()
        {

            printf("yaw: %f, pitch: %f\n", fYaw, fPitch);

            cfront.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
            cfront.y = sin(glm::radians(fPitch));
            cfront.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
            cfront = glm::normalize(cfront);

            Right = glm::normalize(glm::cross(cfront, glm::vec3(0.0, 1.0, 0.0)));
            Up = glm::normalize(glm::cross(Right, cfront));


            // Update view matrix
            matView = glm::lookAt(cPosition, cPosition + cfront, Up);
        }

        // Private member variables

        // Camera quaternion and frame pitch and yaw
        glm::quat cQuaternion;
        GLfloat fPitch = 0.0f;
        GLfloat fYaw   = 0.0f;

        //front
        glm::vec3 cfront;

        // Camera position
        glm::vec3 cPosition = glm::vec3(0.0);

        // Variables used for bookkeeping
        GLboolean resetMouse     = true;
        bool firstMouse = false;
        GLboolean isMousePressed = false;
        GLboolean keysInUse[512];

        // Last cursor position
        GLfloat lastXPos;
        GLfloat lastYPos;

        // Camera settings
        GLfloat cMovementSpeed;
        GLfloat cMouseSensitivity;

        // View matrix
        glm::mat4 matView;
    };
}

#endif
