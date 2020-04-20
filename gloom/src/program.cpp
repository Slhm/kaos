// Local headers
#include <glm/vec3.hpp>
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "gloom/camera.hpp"
#include "glm/glm.hpp"
#include "sceneGraph.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <math.h>
#include <stdio.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include "gloom/tiny_obj_loader.h"
#include "gloom/imageLoader.hpp"

#define GLM_ENABLE_EXPERIMENTAL

SceneNode* rootNode;
SceneNode* koalaNode;
SceneNode* koalaNode2;
SceneNode* koalaNode3;
SceneNode* koalaNode4;
SceneNode* koalaNode5;
SceneNode* koalaNode6;


SceneNode* lightNode;
SceneNode* lightNode2;

Gloom::Shader *shader;
Gloom::Camera *camera;
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);


bool polyMode = false;


void runProgram(GLFWwindow *window) {
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Enable dithering
    glEnable(GL_DITHER);
    camera = new Gloom::Camera();


    // Enable transparency
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glfwSetCursorPosCallback(window, mouseFunc);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    // Set default colour after clearing the colour buffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    shader = new Gloom::Shader();
    shader->makeBasicShader("../gloom/shaders/simple.vert", "../gloom/shaders/simple.frag", "../gloom/shaders/simple.geom");

    shader->activate();

    // Set up your scene here (create Vertex Array Objects, etc.)
    initProg(window);

    // Rendering Loop
    while (!glfwWindowShouldClose(window)) {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        updateFrame(window);
        renderFrame(window);

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window, koalaNode, camera);

        // Flip buffers
        glfwSwapBuffers(window);
    }
}

void mouseFunc(GLFWwindow* window, double xpos, double ypos){
    camera->handleCursorPosInput(xpos, ypos);
}

void initProg(GLFWwindow *window) {


    Image koalaTex = loadImageFile("/home/martin/github/ExplodingTriangles/gloom/resources/Koala_D.png");
    Image koalaNormTex = loadImageFile("/home/martin/github/ExplodingTriangles/gloom/resources/Koala_N.png");


    GLuint koalaTexID = loadTex(koalaTex);
    GLuint koalaNormTexID = loadTex(koalaNormTex);
    glGetError();

    std::string koalaFile = "../gloom/resources/Koala_Tris.obj";

    // tinyObjLoader callback variables.
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, koalaFile.c_str());
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;
    std::vector<uint> indices;

    // load koala mesh
    if (ret) {
        size_t index_offset = 0;

        glm::vec2 tmpTC;
        glm::vec3 tmpVert;
        glm::vec3 tmpNorm;

        // loops over the shapes
        for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[0].mesh.num_face_vertices[f];

            // loops over the faces
            for (size_t v = 0; v < fv; v++) {
                // texture coordinates
                tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
                tmpTC.x = attrib.texcoords[2 * idx.texcoord_index + 0];
                tmpTC.y = 1 - attrib.texcoords[2 * idx.texcoord_index + 1];
                textures.push_back(tmpTC);

                // vertices
                tmpVert.x = attrib.vertices[3 * idx.vertex_index];
                tmpVert.y = attrib.vertices[3 * idx.vertex_index + 1];
                tmpVert.z = attrib.vertices[3 * idx.vertex_index + 2];
                vertices.push_back(tmpVert);

                // normals
                tmpNorm.x = attrib.normals[3 * idx.normal_index];
                tmpNorm.y = attrib.normals[3 * idx.normal_index + 1];
                tmpNorm.z = attrib.normals[3 * idx.normal_index + 2];
                normals.push_back(tmpNorm);
            }
            index_offset += fv;
        }

        // indices.
        // the vertices, normals, and texture coordinates are now sorted, so the indices are just an ascending array of the mesh size.
        for(int i = 0; i < shapes[0].mesh.indices.size(); i++){
            indices.push_back(i);
        }
    }


    unsigned int koalaVAO = generateVAO(vertices, normals, indices, textures);
    unsigned int koalaVAO2 = generateVAO(vertices, normals, indices, textures);
    unsigned int koalaVAO3 = generateVAO(vertices, normals, indices, textures);
    unsigned int koalaVAO4 = generateVAO(vertices, normals, indices, textures);
    unsigned int koalaVAO5 = generateVAO(vertices, normals, indices, textures);
    unsigned int koalaVAO6 = generateVAO(vertices, normals, indices, textures);

    rootNode = createSceneNode();
    koalaNode = createSceneNode();
    koalaNode2 = createSceneNode();
    koalaNode3 = createSceneNode();
    koalaNode4 = createSceneNode();
    koalaNode5 = createSceneNode();
    koalaNode6 = createSceneNode();

    lightNode = createSceneNode();
    lightNode2 = createSceneNode();

    setupKoalaNode(koalaNode, koalaTexID, koalaNormTexID, koalaVAO, indices.size(), glm::vec3(0, 0, 50), glm::vec3(0,2.5,0));
    setupKoalaNode(koalaNode2, koalaTexID, koalaNormTexID, koalaVAO2, indices.size(), glm::vec3(50, 0, 0),glm::vec3(0,3,0));
    setupKoalaNode(koalaNode3, koalaTexID, koalaNormTexID, koalaVAO3, indices.size(), glm::vec3(25, 0, 25),glm::vec3(0,4,0));
    setupKoalaNode(koalaNode4, koalaTexID, koalaNormTexID, koalaVAO4, indices.size(), glm::vec3(-30, 0, 40),glm::vec3(0,5,0));
    setupKoalaNode(koalaNode5, koalaTexID, koalaNormTexID, koalaVAO5, indices.size(), glm::vec3(-50, 0, -20),glm::vec3(0,6,0));
    setupKoalaNode(koalaNode6, koalaTexID, koalaNormTexID, koalaVAO6, indices.size(), glm::vec3(0, 0, -40),glm::vec3(0,7,0));

    lightNode->nodeType = POINT_LIGHT;
    lightNode->position = {-10, 10, 10};
    lightNode->lightIndex = 0;

    lightNode2->nodeType = POINT_LIGHT;
    lightNode2->position = {10, 10, -10};
    lightNode2->lightIndex = 1;

    rootNode->children.push_back(lightNode);
    rootNode->children.push_back(lightNode2);
    rootNode->children.push_back(koalaNode);

    rootNode->children.push_back(koalaNode2);
    rootNode->children.push_back(koalaNode3);
    rootNode->children.push_back(koalaNode4);
    rootNode->children.push_back(koalaNode5);
    rootNode->children.push_back(koalaNode6);


    glGetError();
}

void setupKoalaNode(SceneNode* node, GLuint koalaTexID, GLuint koalaNormTexID, unsigned int vao, unsigned int is, glm::vec3 pos, glm::vec3 rot){

    node->position = pos;
    node->rotation = rot;
    node->scale = glm::vec3(0.2);
    node->vertexArrayObjectID = vao;
    node->textureID = koalaTexID;
    node->nMapID = koalaNormTexID;
    node->VAOIndexCount = is;
}

glm::vec3 calcCenter(std::vector<glm::vec3> vert){

    glm::vec3 avVert;
    for(int i; i < vert.size(); i++){
        avVert.x += vert[0].x / vert.size();
        avVert.y += vert[0].y / vert.size();
        avVert.z += vert[0].z / vert.size();
    }
    return avVert;
}

float calcViewDistance(glm::vec3 obj, glm::vec3 viewDir, glm::vec3 viewPos){
    float tooTiny = 0.0001f;

    glm::vec3 objView = glm::normalize(viewPos - obj);
    glm::vec3 dirView = glm::normalize(viewDir);

    //from https://github.com/Unity-Technologies/UnityCsReference/blob/3417c31e48410974acf40a2a461b31f9a49051ba/Runtime/Export/Math/Vector3.cs#L304
    //float dem = glm::sqrt((objView.x*objView.x + objView.y*objView.y+objView.z*objView.z) * (dirView.x*dirView.x+dirView.y*dirView.y+dirView.z*dirView.z));

    if(dem < tooTiny){
        return 0;
    }
    float dot = glm::clamp(glm::dot(objView, dirView), -1.f, 1.f);

    // devide by 3 to get it in range [0,1],
    // multiply by negative 100 and add 100 to inverse it and make it 100 times larger.
    // 5 is just a multiplier to make the triangles go crazier.
    float r = (glm::acos(dot)/3 * -100 + 100) * 5;
    printf("dem: %f\n", dem);

    return r > 0 ? r : 0;
}





GLuint loadTex(Image img) {
    GLuint texId;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    return texId;
}



unsigned int generateVAO(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<uint> indices,
                         std::vector<glm::vec2> textureCoordinates) {


    unsigned int vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    unsigned int vertID;
    glGenBuffers(1, &vertID);
    glBindBuffer(GL_ARRAY_BUFFER, vertID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * indices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    unsigned int normID;
    glGenBuffers(1, &normID);
    glBindBuffer(GL_ARRAY_BUFFER, normID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * indices.size(), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


    unsigned int texMapID;
    glGenBuffers(1, &texMapID);
    glBindBuffer(GL_ARRAY_BUFFER, texMapID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * indices.size(), textureCoordinates.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    unsigned int indexID;
    glGenBuffers(1, &indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    return vaoID;
}

void updateNodeTransformations(SceneNode *node, glm::mat4 transformationThusFar) {
    glm::mat4 transformationMatrix =
            glm::translate(node->position)
            * glm::translate(node->referencePoint)
            * glm::rotate(node->rotation.y, glm::vec3(0, 1, 0))
            * glm::rotate(node->rotation.x, glm::vec3(1, 0, 0))
            * glm::rotate(node->rotation.z, glm::vec3(0, 0, 1))
            * glm::scale(node->scale)
            * glm::translate(-node->referencePoint);

    node->currentTransformationMatrix = transformationThusFar * transformationMatrix;

    switch (node->nodeType) {
        case GEOMETRY:
            break;
        case POINT_LIGHT:
            break;
        case SPOT_LIGHT:
            break;
    }

    for (SceneNode *child : node->children) {
        updateNodeTransformations(child, node->currentTransformationMatrix);
    }
}


void renderLoop(SceneNode *node) {
    // Draw your scene here

    glUniformMatrix4fv(4, 1, GL_FALSE, glm::value_ptr(node->currentTransformationMatrix));
    glm::mat3 a = glm::inverse(glm::transpose(node->currentTransformationMatrix));

    glUniformMatrix3fv(5, 1, GL_FALSE, glm::value_ptr(a));

    switch (node->nodeType) {
        case GEOMETRY:
            if (node->vertexArrayObjectID != -1) {
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                glEnableVertexAttribArray(2);
                glBindTextureUnit(0, node->textureID);
                glBindTextureUnit(1, node->nMapID);
                glUniform1f(7, calcViewDistance(node->position, cameraFront, cameraPos));
                glBindTexture(GL_TEXTURE_2D, node->textureID);
                glBindVertexArray(node->vertexArrayObjectID);
                glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, nullptr);
                glGetError();
            }
            break;
        case POINT_LIGHT:
            glUniform3fv(node->lightIndex + 10, 1, glm::value_ptr(node->currentTransformationMatrix[3]));
            break;
        case SPOT_LIGHT:
            break;
    }

    glGetError();

    for (SceneNode *child : node->children) {
        renderLoop(child);
    }
}


void updateFrame(GLFWwindow *window) {
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(windowWidth) / float(windowHeight), 0.1f, 100.f);


    glm::mat4 viewMatrix = glm::lookAt(cameraPos, cameraPos + cameraFront, glm::vec3(0, 1, 0));

    glUniformMatrix4fv(6, 1, GL_FALSE, glm::value_ptr(viewMatrix));


    glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(projection));

    updateNodeTransformations(rootNode, glm::mat4(1));
}


void renderFrame(GLFWwindow *window) {
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);

    renderLoop(rootNode);
}


void handleKeyboardInput(GLFWwindow *window, SceneNode *node, Gloom::Camera *camera) {
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cameraFront -= glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0))) * 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cameraFront += glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0))) * 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        auto r = glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0)));
        cameraFront -= glm::normalize(glm::cross(cameraFront, r)) * 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        auto r = glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0)));
        cameraFront += glm::normalize(glm::cross(cameraFront, r)) * 0.1f;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos +=  glm::vec3(cameraFront.x,0,cameraFront.z) * 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -=  glm::vec3(cameraFront.x,0,cameraFront.z) * 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -=  glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0))) * 0.3f;
        cameraPos.y = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos +=  glm::normalize(glm::cross(cameraFront, glm::vec3(0, 1, 0))) * 0.3f;
        cameraPos.y = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cameraPos = glm::vec3(0);
    }
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
        if(!polyMode) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            polyMode = true;
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            polyMode = false;
        }

    }
}
