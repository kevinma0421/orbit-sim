#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <glad/glad.h> // OpenGL functions
#include <glm/glm.hpp>     // Math library
#include "shader.h"

class Sphere
{
public:
    Sphere(float radius, int stacks, int slices); // Constructor
    ~Sphere();                                    // Destructor

    void generateSphere();             // Function to generate vertices & indices
    void setupBuffers();               // Function to set up OpenGL buffers
    void render(const Shader &shader); // Function to render the sphere
    GLuint setTexture(const char *texturePath);

    glm::mat4 rotate(float tilt, float angle);

private:
    float radius;
    int stacks, slices;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int VAO, VBO, EBO;
};

#endif
