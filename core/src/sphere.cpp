#define _USE_MATH_DEFINES

#include <vector>
#include <cmath>
#include <iostream>

#include "sphere.h"
#include "stb.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

using namespace std;

Sphere::Sphere(float radius, int stacks, int slices) : radius(radius), stacks(stacks), slices(slices)
{
    generateSphere();
    setupBuffers();
}

Sphere::~Sphere()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Sphere::generateSphere()
{
    vertices.clear();
    indices.clear();

    for (int i = 0; i <= stacks; i++)
    {
        float lat = M_PI * (-0.5f + (float)i / stacks);
        float y = sin(lat);
        float zr = cos(lat);

        for (int j = 0; j <= slices; j++)
        {
            float lon = 2 * M_PI * (float)j / slices;
            float x = cos(lon) * zr;
            float z = sin(lon) * zr;

            // position
            float px = x * radius;
            float py = y * radius;
            float pz = z * radius;

            // position
            vertices.push_back(px);
            vertices.push_back(py);
            vertices.push_back(pz);

            // normal
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // UV
            float u = 1.0f - (float)j / slices;
            float v = (float)i / stacks;
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    for (int i = 0; i < stacks; i++)
    {
        for (int j = 0; j < slices; j++)
        {
            int first = (i * (slices + 1)) + j;
            int second = first + slices + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void Sphere::setupBuffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // UV
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Sphere::render(const Shader &shader)
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
GLuint Sphere::setTexture(const char *texturePath)
{
    // Load Earth Texture
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 nrChannels == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    // Texture settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // GLfloat aniso = 0.0f;
    // glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &aniso);
    // glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, aniso);
    return texture;
}
glm::mat4 Sphere::rotate(float tilt, float angle)
{
    glm::mat4 rotation = glm::mat4(1.0f);
    rotation = glm::rotate(rotation, glm::radians(tilt), glm::vec3(0.0f, 0.0f, 1.0f)); // tilt the axis
    rotation = glm::rotate(rotation, angle, glm::vec3(0.0f, 1.0f, 0.0f));              // spin around tilted axis
    return rotation;
}
