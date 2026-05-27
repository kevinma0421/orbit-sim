#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb.h"
#include <planet.h>

Planet::Planet(const char *texturePath, const char *vertexShader, const char *fragmentShader, Camera &camera)
    : Sphere(1.0f, 100, 100),
      shader(vertexShader, fragmentShader),
      planetTexture(setTexture(texturePath))
{
    initLighting = camera.getPosition();
    rotationSpeed = 2 * glm::pi<float>() / 86164.0f;
    tilt = 23.5f;
    cloudTexture = setTexture("textures/cloud2.png");
}
void Planet::renderEarth(Camera &camera)
{
    update();

    // set earth texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, planetTexture);

    // set cloud texture
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, cloudTexture);

    // alpha blending for clouds
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // set up earth matrices
    glm::mat4 model = rotate(tilt, currentAngle);
    shader.use();
    shader.setInt("sphereTexture", 1);
    shader.setInt("cloudTexture", 2);
    shader.setMat4("projection", camera.getProjectionMatrix());
    shader.setMat4("view", camera.getViewMatrix());
    shader.setMat4("model", model);

    // setup lighting
    shader.setVec3("lightPos", initLighting);
    shader.setVec3("viewPos", camera.getPosition());
    shader.setVec3("lightColor", glm::vec3(1.0f));

    // render
    render(shader);

    glDisable(GL_BLEND);
}
void Planet::update()
{
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastUpdateTime;
    lastUpdateTime = currentTime;

    currentAngle += rotationSpeed * deltaTime;
}