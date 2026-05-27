#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "sat_sim.h"
#include "sphere.h"
#include "utils.h"
#include "shader.h"
#include "window.h"
#include "stb.h"
#include "input.h"
#include "camera.h"
#include "gui.h"
#include "planet.h"
#include "skybox.h"
#include "satellite.h"

// window sizes
const int height = 800;
const int width = 800;

// Paths
const char *earthvs = "shaders/earth.vs";
const char *earthfs = "shaders/earth.fs";
const char *skyboxfs = "shaders/skybox.fs";
const char *skyboxvs = "shaders/skybox.vs";
const char *lightfs = "shaders/light.fs";
const char *lightvs = "shaders/light.vs";
const char *orbitfs = "shaders/orbit.fs";
const char *orbitvs = "shaders/orbit.vs";

const char *earthPath = "textures/earth4k.jpg";
const char *binaryPath = "scripts/precomputed_orbits.bin";

int main()
{
    // Create objects
    Window myWindow(width, height, "sat_sim");
    Camera myCamera(static_cast<float>(width), static_cast<float>(height));
    Planet earth(earthPath, earthvs, earthfs, myCamera);
    Gui myGui(myWindow.getGLFWwindow());
    Skybox skybox(skyboxvs, skyboxfs);
    Satellite satellite(binaryPath, orbitvs, orbitfs);

    // GL Inits
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glfwSetWindowUserPointer(myWindow.getGLFWwindow(), &myCamera);

    glfwSetScrollCallback(myWindow.getGLFWwindow(), Window::scroll_callback);
    glfwSetMouseButtonCallback(myWindow.getGLFWwindow(), Window::mouse_button_callback);
    glfwSetCursorPosCallback(myWindow.getGLFWwindow(), Window::cursor_position_callback);

    // timing stuff
    float simTime = 0.0f;
    float lastRealTime = 0.0f;

    while (!myWindow.shouldClose())
    {
        // prep frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // input handling
        processInput(myWindow.getGLFWwindow());

        // GUI frame
        myGui.frame(myCamera, earth.rotationSpeed);

        // render Earth
        earth.renderEarth(myCamera);

        // render sats
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastRealTime;
        lastRealTime = currentTime;

        if (earth.rotationSpeed != 0)
            simTime += deltaTime;

        satellite.update(simTime, earth.rotationSpeed);
        satellite.render(myCamera);

        // render cubeMap
        skybox.render(myCamera);

        // render Gui
        myGui.render();

        myWindow.swapBuffers();
        myWindow.pollEvents();
    }

    return 0;
}
