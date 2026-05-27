#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"
#include "utils.h"
#include "camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

Window::Window(int width, int height, const std::string &title)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window)
    {
        logMessage("GLFW Window creation failed!");
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(2); // vsync
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glEnable(GL_MULTISAMPLE);
    glViewport(0, 0, width, height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // render window in center of screen
    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
    int screenWidth = videoMode->width;
    int screenHeight = videoMode->height;
    int xpos = (screenWidth - width) / 2;
    int ypos = (screenHeight - height) / 2;
    glfwSetWindowPos(window, xpos, ypos);
}

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::swapBuffers() { glfwSwapBuffers(window); }
void Window::pollEvents() { glfwPollEvents(); }
bool Window::shouldClose() const { return glfwWindowShouldClose(window); }
GLFWwindow *Window::getGLFWwindow() const { return window; }

void Window::framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    cam->setScreenSize(static_cast<float>(width), static_cast<float>(height));
}
void Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    cam->processScroll(static_cast<float>(yoffset));
}
void Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    cam->setMouse(window, button, action, mods);
}
void Window::cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    if (ImGui::GetIO().WantCaptureMouse)
        return;

    Camera *cam = static_cast<Camera *>(glfwGetWindowUserPointer(window));
    cam->setCursor(window, xpos, ypos);
}