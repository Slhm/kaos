#ifndef PROGRAM_HPP
#define PROGRAM_HPP
#pragma once


// System headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <string>
#include "sceneGraph.hpp"
#include "gloom/imageLoader.hpp"
#include "gloom/camera.hpp"
#include "gloom/mesh.h"
// Main OpenGL program
void runProgram(GLFWwindow* window);

//init program
void initProg(GLFWwindow* window);

//main loop
void renderLoop(SceneNode* node);
void updateFrame(GLFWwindow* window);
void renderFrame(GLFWwindow* window);
void updateNodeTransformations(SceneNode* node, glm::mat4 transformationThusFar);

//For calculating the center of an object
glm::vec3 calcCenter(std::vector<glm::vec3> vert);

//calculate the distance between the center of view and an object.
float calcViewDistance(glm::vec3 obj, glm::vec3 viewDir, glm::vec3 viewPos);

// setup VAO for the model.
//unsigned int generateVAO(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<uint> indices, std::vector<glm::vec2> textureCoordinates);
unsigned int generateVAO(Mesh mesh);


//load texture
GLuint loadTex(Image img);

void setupNodeProp(SceneNode* node, GLuint koalaTexID, GLuint koalaNormTexID, unsigned int vao, unsigned int is, glm::vec3 pos, glm::vec3 rot);

void mouseFunc(GLFWwindow* window, double xpos, double ypos);
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

// Function for handling keypresses
void handleKeyboardInput(GLFWwindow* window, SceneNode* node, Gloom::Camera* camera);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
// Checks for whether an OpenGL error occurred. If one did,
// it prints out the error type and ID
inline void printGLError() {
    int errorID = glGetError();

    if(errorID != GL_NO_ERROR) {
        std::string errorString;

        switch(errorID) {
            case GL_INVALID_ENUM:
                errorString = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_OPERATION:
                errorString = "GL_INVALID_OPERATION";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                errorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
            case GL_OUT_OF_MEMORY:
                errorString = "GL_OUT_OF_MEMORY";
                break;
            case GL_STACK_UNDERFLOW:
                errorString = "GL_STACK_UNDERFLOW";
                break;
            case GL_STACK_OVERFLOW:
                errorString = "GL_STACK_OVERFLOW";
                break;
            default:
                errorString = "[Unknown error ID]";
                break;
        }

        fprintf(stderr, "An OpenGL error occurred (%i): %s.\n",
                errorID, errorString.c_str());
    }
}


#endif
