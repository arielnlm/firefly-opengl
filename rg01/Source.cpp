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
#include "Player.h"

#include <iostream>
#include <filesystem>
#include "MousePicker.h"
#include "ParticleMaster.h"
#include "ParticleSystem.h"
#include "WaterRenderer.h"
#include "WaterFrameBuffers.h"
#include "PostProcessing.h"

/*----------------------- CONTROLS -----------------------*/
/*
    WASD - movement
    JUMP - hold to fly, can also tap
    LMB hold + left/right mouse movement - moves camera left or right
    RMB hold + up/down mouse movement - moves camera up or down
    Scrool wheel - zoom in/out camera
*/

/*----------------------- FUNCTIONS -----------------------*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

/*----------------------- [SETTINGS] GAME SETTINGS [SETTINGS] -----------------------*/
const float ZOOM_IN_OUT_STRENGTH = 0.5f;

float particlesPerSecond = 100.0f;
float particleSpeed = 25.0f;
float particleGravityComplient = 0.3f;
float particleLifeTime = 3.0f;

const char* PARTICLE_TEXTURE = "resources/objects/particleAtlas.png";
const int PARTICLE_TEXTURE_ROWS = 4; // higher than 1 if using atlases

glm::vec3 sunPosition(0, 10000, -7000);
glm::vec3 sunColor(.82, .82, .82);

const int TREES_TO_GENERATE = 200;
const int FERNS_TO_GENERATE = 150;

glm::vec3 playerSpawnPosition(150, 20, -150);

const float BLUR_SCREEN_AFTER_SECONDS = 3.0f;
const float UNBLUR_SCREEN_AFTER_SECONDS = 1.5f;

const char* TERRAIN_GROUND_TEXTURE_1 = "resources/objects/grass1.jpg";
const char* TERRAIN_GROUND_TEXTURE_2 = "resources/objects/clover.jpg";
const char* TERRAIN_GROUND_TEXTURE_3 = "resources/objects/rock01.jpg";
const char* TERRAIN_GROUND_TEXTURE_4 = "resources/objects/whiterock.jpg";
const char* TERRAIN_GROUND_BLEND_MAP = "resources/objects/blendMap.jpg";

const float WATER_HEIGHT = -15.0; // higher means more water will show

const bool RENDER_PARTICLE_REFLECTION = false; // beefy pc can handle this

/*----------------------- [SETTINGS] SCREEN [SETTINGS] -----------------------*/
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

bool shouldBlurScreen = false;

/*----------------------- [SETTINGS] CAMERA AND MOUSE [SETTINGS] -----------------------*/
bool leftClick = false;
int mouseState = GLFW_CURSOR_NORMAL;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

/*----------------------- [SETTINGS] FPS AND TIME [SETTINGS] -----------------------*/
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int frameCounter = 0;
float allDeltaTimes = 0.0f;
float timeHeld = 0.0f;

/*----------------------- INITIALIZAION -----------------------*/
Player* player;
PlayerCamera playerCamera(player);

ParticleMaster* particleMaster;
ParticleSystem* particleSystem;

Terrain terrain;

/*----------------------- CONTROLS -----------------------*/
/*
    WASD - movement
    JUMP - hold to fly, can also tap
    LMB hold + left/right mouse movement - moves camera left or right
    RMB hold + up/down mouse movement - moves camera up or down
    Scrool wheel - zoom in/out camera
*/

int main()
{
    /*----------------------- GLWF: INITALIZATION AND CONFIGURATION -----------------------*/
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /*----------------------- GLFW: WINDOW CREATION -----------------------*/
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FIREFLY_EXPLORER", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /*----------------------- GLFW: CALLBACKS -----------------------*/
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    /*----------------------- GLAD: LOAD ALL OPENGL FUNCTION POINTERS -----------------------*/
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Loader* loader = new Loader();

    /*----------------------- TERRAIN  -----------------------*/
    TerrainTexture backgroundTexture(loader->loadTexture(TERRAIN_GROUND_TEXTURE_1));
    TerrainTexture rTexture(loader->loadTexture(TERRAIN_GROUND_TEXTURE_2));
    TerrainTexture gTexture(loader->loadTexture(TERRAIN_GROUND_TEXTURE_3));
    TerrainTexture bTexture(loader->loadTexture(TERRAIN_GROUND_TEXTURE_4));

    TerrainTexturePack texturePack(backgroundTexture, rTexture, gTexture, bTexture);
    TerrainTexture blendMap(loader->loadTexture(TERRAIN_GROUND_BLEND_MAP));

    terrain.lateLoadTerrain(0, -1, loader, texturePack, blendMap, "resources/objects/heightmap.png");

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /*----------------------- MODELS -----------------------*/
    RawModel* model = ObjLoader::loadObjModel("resources/objects/lowPolyTree.obj", loader);   
    TexturedModel* grassModel = new TexturedModel(model, new ModelTexture(loader->loadTexture("resources/objects/lowPolyTree.png")));
    
    grassModel->getTexture()->setHasTransparency(false);
    grassModel->getTexture()->setUseFakeLighting(false);

    RawModel* model2 = ObjLoader::loadObjModel("resources/objects/fern.obj", loader);
    ModelTexture* fernMT = new ModelTexture(loader->loadTexture("resources/objects/fernatlas.png"));
    fernMT->setNumberOfRows(2); // using texture atlas
    TexturedModel* fernModel = new TexturedModel(model2, fernMT);

    fernModel->getTexture()->setHasTransparency(true);
    fernModel->getTexture()->setUseFakeLighting(true);

    RawModel* lampModel = ObjLoader::loadObjModel("resources/objects/lamp.obj", loader);
    TexturedModel* lampTModel = new TexturedModel(lampModel, new ModelTexture(loader->loadTexture("resources/objects/lamp.png")));
    int lampToPlace = 3;

    /*----------------------- LIGHTS -----------------------*/
    vector<Light> lights;
    float lightYOffset = 13.8f; // lamp model to actual light Y offset
    
    Light sun(sunPosition, sunColor);
    lights.push_back(sun);
   
    /*----------------------- MASTER RENDERER -----------------------*/
    MasterRenderer* renderer = new MasterRenderer(loader);

    /*----------------------- WATER -----------------------*/
    vector<WaterTile> waters;

    WaterFrameBuffers* fbos = new WaterFrameBuffers();

    WaterShader waterShader;
    WaterRenderer* waterRenderer = new WaterRenderer(loader, waterShader, renderer->getProjectionMatrix(), fbos);
    waters.push_back(WaterTile(255, -337, WATER_HEIGHT));

    /*----------------------- ENTITY LOADING -----------------------*/
    vector<Entity*> entities;

    // trees and lamps
    for (int i = 0; i < TREES_TO_GENERATE; i++) {
        float rnd1 = (float)rand() / (float)(RAND_MAX), rnd2 = (float)rand() / (float)(RAND_MAX);
        float x = rnd1 * 800.0 , z = rnd2 * -600;
        float y = terrain.getHieghtOfTerrain(x, z);   
        if (y < waters[0].getHeight() + 1) // prevent entities from spawning in water
            continue;
        if (lampToPlace > 0) {
            lampToPlace--;
            entities.push_back(new Entity(lampTModel, glm::vec3(x, y, z), 0, 0, 0, 1));
            lights.push_back(Light(glm::vec3(x, y + lightYOffset, z), glm::vec3(2, lampToPlace, lampToPlace/2.0), glm::vec3(1, 0.01, 0.002)));
        }
        else {
            entities.push_back(new Entity(grassModel, glm::vec3(x, y, z), 0, 0, 0, 1));
        }
    }

    // ferns
    for (int i = 0; i < FERNS_TO_GENERATE; i++) {
        float rnd1 = (float)rand() / (float)(RAND_MAX), rnd2 = (float)rand() / (float)(RAND_MAX);
        float x = rnd1 * 800.0 , z = rnd2 * -600;
        float y = terrain.getHieghtOfTerrain(x, z);
        float scale = 1;
        if (y < waters[0].getHeight()) { // place fern on water
            y = waters[0].getHeight() + 0.5f;
            scale = 0.5;
        }
        entities.push_back(new Entity(fernModel, i%4, glm::vec3(x, y, z), 0, 0, 0, scale));
    }

    /*----------------------- PLAYER -----------------------*/
    RawModel* playerRaw = ObjLoader::loadObjModel("resources/objects/box.obj", loader);
    TexturedModel* playerTM = new TexturedModel(playerRaw, new ModelTexture(loader->loadTexture("resources/objects/white.png")));

    player = new Player(playerTM, playerSpawnPosition, 0, 0, 0, 1);
    playerCamera.setPlayer(player);

    entities.push_back(player);
    
    
    /*----------------------- MOUSE PICKER : WIP -----------------------*/
    MousePicker picker(playerCamera, renderer->getProjectionMatrix());

    /*----------------------- PARTICLES -----------------------*/

    particleMaster = new ParticleMaster(loader, renderer->getProjectionMatrix()); 
    ParticleTexture particleTexture(loader->loadTexture(PARTICLE_TEXTURE), PARTICLE_TEXTURE_ROWS);
    particleSystem = new ParticleSystem(particleMaster, particleTexture,
        particlesPerSecond, particleSpeed, particleGravityComplient, particleLifeTime);
    
    /*----------------------- FBO AND POST PROCESSING -----------------------*/
    Fbo* fbo = new Fbo(SCR_WIDTH, SCR_HEIGHT, 2);
    PostProcessing* postProcessing = new PostProcessing(loader);
    
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        /*----------------------- FPS & TIME CALCULATION -----------------------*/
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        allDeltaTimes += deltaTime;
        frameCounter++;
        if (allDeltaTimes >= 1.0f) {
            allDeltaTimes = 0.0f;
            printf("FPS: %d\n", frameCounter);
            frameCounter = 0;
        }

        /*----------------------- PROCESS PLAYER INPUT -----------------------*/
        processInput(window);

        /*----------------------- WIP : MOUSE MOVEMENT & RAY TRACING -----------------------*/
       /*
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        picker.update(mouseX, mouseY, playerCamera);
        */

        glEnable(GL_CLIP_DISTANCE0);

        /*----------------------- RENDERING -----------------------*/
        particleMaster->update(deltaTime, playerCamera);
        fbos->bindReflectionFrameBuffer();
        float distance = 2.0 * (playerCamera.getPosition().y - waters[0].getHeight());
        playerCamera.subtractPosY(distance);
        playerCamera.invertPitch();
        // render on water reflection FBO
        renderer->renderScene(entities, terrain, lights, playerCamera, deltaTime, glm::vec4(0, 1, 0, -waters[0].getHeight()));
        if (RENDER_PARTICLE_REFLECTION)
            particleMaster->renderParticles(playerCamera);

        playerCamera.subtractPosY(-distance);
        playerCamera.invertPitch();
        fbos->bindRefractionFrameBuffer();
        // render on water refraction FBO
        renderer->renderScene(entities, terrain, lights, playerCamera, deltaTime, glm::vec4(0, -1, 0, waters[0].getHeight()));

        // render on screen
        fbos->unbindCurrentFrameBuffer();
        glDisable(GL_CLIP_DISTANCE0);
        fbo->bindFrameBuffer();
        renderer->renderScene(entities, terrain, lights, playerCamera, deltaTime, glm::vec4(0, -1, 0, 100000));
        waterRenderer->render(waters, playerCamera, deltaTime, sun);
        particleMaster->renderParticles(playerCamera);
        fbo->unbindFrameBuffer();
        // apply post processing effects
        postProcessing->doPostProcessing(fbo->getColourTexture(), shouldBlurScreen);

        //printf("PLAYER POS %f %f %f\n", player->getPosition().x, player->getPosition().y, player->getPosition().z);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /*----------------------- CLEANING UP -----------------------*/
    fbo->cleanUp();
    fbos->cleanUp();
    particleMaster->cleanUp();
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
    bool moved = false;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player->move(PlayerInput::W, terrain.getHieghtOfTerrain(player->getPosition().x, player->getPosition().z), deltaTime, moved);
        playerCamera.updateCameraPosition();
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player->move(PlayerInput::S, terrain.getHieghtOfTerrain(player->getPosition().x, player->getPosition().z), deltaTime, moved);
        playerCamera.updateCameraPosition();
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player->move(PlayerInput::A, terrain.getHieghtOfTerrain(player->getPosition().x, player->getPosition().z), deltaTime, moved);
        playerCamera.updateCameraPosition();
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player->move(PlayerInput::D, terrain.getHieghtOfTerrain(player->getPosition().x, player->getPosition().z), deltaTime, moved);
        playerCamera.updateCameraPosition();
        moved = true;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        player->jump();
        player->move(PlayerInput::NOTHING, terrain.getHieghtOfTerrain(player->getPosition().x, player->getPosition().z), deltaTime, moved);
        playerCamera.updateCameraPosition();
        moved = true;
        particleSystem->generateParticles(player->getPosition(), deltaTime);
        //particleMaster->addParticle(Particle(glm::vec3(player->getPosition()), glm::vec3(0, 30, 0), 1, 4, 0, 1));
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
        timeHeld -= deltaTime;
        if (timeHeld < UNBLUR_SCREEN_AFTER_SECONDS) {
            timeHeld = 0;
            shouldBlurScreen = false;
        }
    }
    else {
        timeHeld += deltaTime;
        if (timeHeld > BLUR_SCREEN_AFTER_SECONDS) {
            timeHeld = BLUR_SCREEN_AFTER_SECONDS;
            shouldBlurScreen = true;
        }
        
    }

    if (!moved) {
        player->move(PlayerInput::NOTHING, terrain.getHieghtOfTerrain(player->getPosition().x, player->getPosition().z), deltaTime, moved);
        playerCamera.updateCameraPosition();
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
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;
    // ako drzimo desni klik
    if (mouseState == GLFW_CURSOR_DISABLED)
        playerCamera.calculatePitch(yoffset);
    if (leftClick) {
        playerCamera.calculateAngeAroundPlayer(xoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    playerCamera.calculateZoom(yoffset * ZOOM_IN_OUT_STRENGTH);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        leftClick = true;
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        leftClick = false;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        mouseState = GLFW_CURSOR_DISABLED;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        mouseState = GLFW_CURSOR_NORMAL;
}