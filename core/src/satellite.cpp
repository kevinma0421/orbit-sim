#include "satellite.h"
#include <fstream>
#include <iostream>
#include <camera.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "utils.h"

Satellite::Satellite(const char *binaryPath, const char *orbitvs, const char *orbitfs) : satShader(orbitvs, orbitfs)
{
    loadBinary(binaryPath);
    setupBuffers();
    satellitePositions.resize(numSats);
    logMessage(std::to_string(numSats));
}

Satellite::~Satellite()
{
    glDeleteBuffers(1, &pointVBO);
    glDeleteVertexArrays(1, &pointVAO);
}

void Satellite::loadBinary(const char *path)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file)
    {
        logMessage("failed to open file");
        return;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    if (size <= 0 || size % sizeof(float) != 0)
    {
        logMessage(std::to_string(size));
        return;
    }

    size_t floatCount = size / sizeof(float);
    const size_t floatsPerSat = 1 + pointsPerOrbit * 3;
    numSats = floatCount / floatsPerSat;

    std::vector<float> buffer(floatCount);
    if (!file.read(reinterpret_cast<char *>(buffer.data()), size))
    {
        logMessage("failed to read orbit data");
        return;
    }

    meanMotions.resize(numSats);
    orbitVertices.resize(numSats * pointsPerOrbit * 3);

    for (int i = 0; i < numSats; ++i)
    {
        size_t base = i * floatsPerSat;
        meanMotions[i] = buffer[base];

        for (int j = 0; j < pointsPerOrbit * 3; ++j)
        {
            orbitVertices[i * pointsPerOrbit * 3 + j] = buffer[base + 1 + j];
        }
    }
}

void Satellite::setupBuffers()
{
    glGenVertexArrays(1, &pointVAO);
    glGenBuffers(1, &pointVBO);

    glBindVertexArray(pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, numSats * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);

    glBindVertexArray(0);
}

void Satellite::update(float time, float rotationSpeed)
{
    if (rotationSpeed == 0)
        return;

    // logMessage(std::to_string(rotationSpeed));

    for (int i = 0; i < numSats; ++i)
    {
        float revPerSec = meanMotions[i] / 86400.0f;
        float simSpeed = rotationSpeed / (2 * glm::pi<float>() / 86164.0f);
        // logMessage(std::to_string(meanMotions[i]));

        float fractionalIndex = fmod(time * simSpeed * revPerSec * pointsPerOrbit, pointsPerOrbit);
        int indexA = static_cast<int>(fractionalIndex);
        int indexB = (indexA + 1) % pointsPerOrbit;
        float t = fractionalIndex - indexA;

        int base = i * pointsPerOrbit * 3;

        glm::vec3 A = glm::vec3(
            orbitVertices[base + indexA * 3 + 0],
            orbitVertices[base + indexA * 3 + 1],
            orbitVertices[base + indexA * 3 + 2]);

        glm::vec3 B = glm::vec3(
            orbitVertices[base + indexB * 3 + 0],
            orbitVertices[base + indexB * 3 + 1],
            orbitVertices[base + indexB * 3 + 2]);

        satellitePositions[i] = glm::mix(A, B, t);
    }

    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, satellitePositions.size() * sizeof(glm::vec3), satellitePositions.data(), GL_DYNAMIC_DRAW);
}

void Satellite::render(Camera &camera)
{
    satShader.use();
    satShader.setMat4("view", camera.getViewMatrix());
    satShader.setMat4("projection", camera.getProjectionMatrix());

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(23.44f), glm::vec3(0.0f, 0.0f, 1.0f));
    satShader.setMat4("model", model);
    glEnable(GL_PROGRAM_POINT_SIZE);
    satShader.setFloat("scaleMultiplier", camera.getRadius());
    satShader.setVec3("orbitColor", glm::vec3(0.6f, 0.0f, 0.0f));

    glBindVertexArray(pointVAO);
    glPointSize(4.0f);
    glDrawArrays(GL_POINTS, 0, numSats);
    glBindVertexArray(0);
}
