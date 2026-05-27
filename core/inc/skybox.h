#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h" // Your shader wrapper class
#include "stb.h"
#include "camera.h"

class Skybox
{
public:
    Skybox(const char *skyboxfs, const char *skyboxvs);
    void render(Camera &camera);

private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubemapTexture = 0;
    Shader skyboxShader;

    GLuint loadTexture();
    void setupBuffers();

    // Cubemap Paths
    /*const char *space_1 = "C:/Users/123ke/projects/opengl/textures/skybox/space_bk.png";
    const char *space_2 = "C:/Users/123ke/projects/opengl/textures/skybox/space_ft.png";
    const char *space_3 = "C:/Users/123ke/projects/opengl/textures/skybox/space_dn.png";
    const char *space_4 = "C:/Users/123ke/projects/opengl/textures/skybox/space_up.png";
    const char *space_5 = "C:/Users/123ke/projects/opengl/textures/skybox/space_lf.png";
    const char *space_6 = "C:/Users/123ke/projects/opengl/textures/skybox/space_rt.png";*/

    const char *space_1 = "textures/skybox/ny1.png";
    const char *space_2 = "textures/skybox/nx1.png";
    const char *space_3 = "textures/skybox/nz1.png";
    const char *space_4 = "textures/skybox/px1.png";
    const char *space_5 = "textures/skybox/py1.png";
    const char *space_6 = "textures/skybox/pz1.png";

    // Skybox init
    /*std::vector<std::string> faces = {
        space_1,
        space_2,
        space_3,
        space_4,
        space_5,
        space_6};*/
    std::vector<std::string> faces = {
        space_2, // +X (right)
        space_4, // -X (left)
        space_5, // +Y (top)
        space_1, // -Y (bottom)
        space_3, // +Z (front ← originally back)
        space_6  // -Z (back  ← originally front)
    };
};
