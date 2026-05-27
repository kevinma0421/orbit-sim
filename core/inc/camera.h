#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(float screenWidth, float screenHeight,
           float radius = 5.0f,
           float yaw = -90.0f,
           float pitch = 0.0f,
           glm::vec3 center = glm::vec3(0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void setScreenSize(float w, float h);
    void processScroll(float yOffset);
    void setMouse(GLFWwindow *window, int button, int action, int mods);
    void setCursor(GLFWwindow *window, double xpos, double ypos);
    void reset();
    float getRadius() const;
    glm::vec3 getPosition() const;

private:
    // Orbiting parameters
    glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f); // Earth stays here
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    float radius = 5.0f;
    float yaw = -90.0f;
    float pitch = 0.0f;

    // Viewport and projection
    float screenWidth;
    float screenHeight;
    float fov = 45.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;

    // Interaction
    bool isDragging = false;
    double lastX = 0.0;
    double lastY = 0.0;
    float sensitivity = 0.1f;
};
