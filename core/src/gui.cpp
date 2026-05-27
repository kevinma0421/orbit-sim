#include "gui.h"

Gui::Gui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImFontConfig font_cfg;
    font_cfg.OversampleH = 4;    // Horizontal oversampling
    font_cfg.OversampleV = 4;    // Vertical oversampling
    font_cfg.PixelSnapH = false; // Smooth horizontal positioning

    ImFont *font = io.Fonts->AddFontFromFileTTF("textures/static/Roboto-Black.ttf", 12.0f, &font_cfg);
    io.FontDefault = font;
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    speedOptions[0] = 60;
    speedOptions[1] = 0;
    // speedOptions[2] = 30000;

    currIndex = 0;

    GLuint texPause = LoadTexture("textures/pause.png");
    GLuint texPlay = LoadTexture("textures/play.png");
    GLuint texFast = LoadTexture("textures/forward.png");

    speedButtons[0] = texPause;
    speedButtons[1] = texPlay;
    // speedButtons[2] = texFast;
}
Gui::~Gui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void Gui::frame(Camera &camera, float &rotationSpeed)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // --- Overlay buttons (floating, transparent background) ---
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 10.0f, 10.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));

    ImGui::SetNextWindowBgAlpha(0.0f); // Fully transparent

    ImGui::Begin("SpeedControlOverlay", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav |
                     ImGuiWindowFlags_NoBackground);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));

    ImTextureID texID = static_cast<ImTextureID>(speedButtons[currIndex]);
    if (ImGui::ImageButton("speed_button", texID, ImVec2(16, 16)))
    {
        currIndex = (currIndex + 1) % 2;
    }
    ImGui::PopStyleColor(3);
    ImGui::End();

    // --- FPS overlay (top-right corner) ---
    ImGui::SetNextWindowPos(ImVec2(10.0f, 10.0f), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("FPS Overlay", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoFocusOnAppearing |
                     ImGuiWindowFlags_NoNav |
                     ImGuiWindowFlags_NoMove);

    ImGui::Text("FPS: %d", fps());
    ImGui::End();

    // Update speed based on selected index
    rotationSpeed = speedOptions[currIndex] * 2.0f * glm::pi<float>() / 86164.0f;
}

void Gui::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
int Gui::fps()
{
    static double lastTime = glfwGetTime();
    static int frameCount = 0;
    static int lastFPS = 0;

    double currentTime = glfwGetTime();
    frameCount++;

    if (currentTime - lastTime >= 1.0)
    {
        lastFPS = frameCount;
        frameCount = 0;
        lastTime = currentTime;
    }

    return lastFPS;
}
GLuint Gui::LoadTexture(const char *filename)
{
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 4);
    if (!data)
        return 0;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}