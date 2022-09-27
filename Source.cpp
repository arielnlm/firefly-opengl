#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <glm/gtc/type_ptr.hpp>

#include "RawModel.h"
#include "Loader.h"
#include "EntityRenderer.h"
#include "Shader.h"
#include "ModelTexture.h"
#include "TexturedModel.h"
#include "StaticShader.h"
#include "ObjectModel.h"
#include "Maths.h"
#include "ObjLoader.h"
#include "PlayerCamera.h"
#include "Light.h"
#include "MasterRenderer.h"

#include <iostream>
#include <filesystem>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

PlayerCamera playerCamera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
                -0.5f,0.5f,0,
                -0.5f,-0.5f,0,
                0.5f,-0.5f,0,
                0.5f,0.5f,0,

                -0.5f,0.5f,1,
                -0.5f,-0.5f,1,
                0.5f,-0.5f,1,
                0.5f,0.5f,1,

                0.5f,0.5f,0,
                0.5f,-0.5f,0,
                0.5f,-0.5f,1,
                0.5f,0.5f,1,

                -0.5f,0.5f,0,
                -0.5f,-0.5f,0,
                -0.5f,-0.5f,1,
                -0.5f,0.5f,1,

                -0.5f,0.5f,1,
                -0.5f,0.5f,0,
                0.5f,0.5f,0,
                0.5f,0.5f,1,

                -0.5f,-0.5f,1,
                -0.5f,-0.5f,0,
                0.5f,-0.5f,0,
                0.5f,-0.5f,1

    };

    float textureCoords[] = {

            0,0,
            0,1,
            1,1,
            1,0,
            0,0,
            0,1,
            1,1,
            1,0,
            0,0,
            0,1,
            1,1,
            1,0,
            0,0,
            0,1,
            1,1,
            1,0,
            0,0,
            0,1,
            1,1,
            1,0,
            0,0,
            0,1,
            1,1,
            1,0


    };

    int indices[] = {
            0,1,3,
            3,1,2,
            4,5,7,
            7,5,6,
            8,9,11,
            11,9,10,
            12,13,15,
            15,13,14,
            16,17,19,
            19,17,18,
            20,21,23,
            23,21,22

    };

    Loader* loader = new Loader();

    //------------- TERRAIN TEXTURE -------------------

    TerrainTexture backgroundTexture(loader->loadTexture("resources/objects/grass1.jpg"));
    TerrainTexture rTexture(loader->loadTexture("resources/objects/clover.jpg"));
    TerrainTexture gTexture(loader->loadTexture("resources/objects/rock01.jpg"));
    TerrainTexture bTexture(loader->loadTexture("resources/objects/whiterock.jpg"));

    TerrainTexturePack texturePack(backgroundTexture, rTexture, gTexture, bTexture);
    TerrainTexture blendMap(loader->loadTexture("resources/objects/blendMap.jpg"));

    //-------------------------------------------------

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    
    RawModel* model = ObjLoader::loadObjModel("resources/objects/lowPolyTree.obj", loader);
    
    TexturedModel* grassModel = new TexturedModel
        (model, new ModelTexture(loader->loadTexture("resources/objects/lowPolyTree.png")));
    grassModel->getTexture()->setHasTransparency(true);
    grassModel->getTexture()->setUseFakeLighting(true);
    vector<Entity*> entities;
    
    RawModel* stallModel = ObjLoader::loadObjModel("resources/objects/stall.obj", loader);
    TexturedModel* stallTModel = new TexturedModel(
        stallModel, new ModelTexture(loader->loadTexture("resources/objects/stallTexture.png")));
    entities.push_back(new Entity(stallTModel, glm::vec3(2, 10, -5), 0, 0, 0, 2));
    for (int i = 0; i < 500; i++) {
        float rnd1 = (float)rand() / (float)(RAND_MAX), rnd2 = (float)rand() / (float)(RAND_MAX);
        entities.push_back(new Entity(grassModel, glm::vec3(rnd1 * 800.0 - 400, 0, rnd2 * -600), 0, 0, 0, 1));
    }
    
    Light light(glm::vec3(20000, 20000, 2000), glm::vec3(1,1,1));

    // Terrains
    Terrain terrain(0, -1, loader, texturePack, blendMap);
    Terrain terrain2(-1, -1, loader, texturePack, blendMap);


    MasterRenderer* renderer = new MasterRenderer();
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        // -----
        processInput(window);
        renderer->processTerrain(terrain);
        renderer->processTerrain(terrain2);
        for (Entity* e : entities) {
            // e->increaseRotation(0.2, 0.2, 0);
            renderer->processEntity(e);
        }
        renderer->render(light, playerCamera);
        
  
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    renderer->cleanUp();
    loader->cleanUp();
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        playerCamera.move(PlayerCamera::FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        playerCamera.move(PlayerCamera::BACKWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        playerCamera.move(PlayerCamera::LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        playerCamera.move(PlayerCamera::RIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        playerCamera.move(PlayerCamera::UP);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        playerCamera.move(PlayerCamera::DOWN);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
