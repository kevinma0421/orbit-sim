#pragma once

#include <string>
#include <glad/glad.h>

#include "sphere.h"
#include "camera.h"

class Planet : public Sphere
{
public:
    float tilt;
    float rotationSpeed;

    Planet(const char *texturePath, const char *vertexShader, const char *fragmentShader, Camera &camera);

    void renderEarth(Camera &camera);
    void update();

private:
    GLuint planetTexture;
    GLuint cloudTexture;
    glm::vec3 initLighting;
    Shader shader;
    float currentAngle = 0.0f;
    float lastUpdateTime = 0.0f;
};
