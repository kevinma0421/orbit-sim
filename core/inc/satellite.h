#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"

class Satellite
{
public:
    Satellite(const char *binaryPath, const char *orbitvs, const char *orbitfs);
    ~Satellite();

    void update(float time, float rotationSpeed);
    void render(Camera &camera);

private:
    void loadBinary(const char *path);
    void setupBuffers();

    Shader satShader;

    std::vector<float> orbitVertices;
    std::vector<glm::vec3> satellitePositions;
    std::vector<float> meanMotions;
    GLuint pointVAO = 0, pointVBO = 0;
    int numSats = 0;
    int pointsPerOrbit = 100;
    float orbitSpeed = 1.0f;
};
