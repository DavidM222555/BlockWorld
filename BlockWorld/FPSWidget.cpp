#include "FPSWidget.h"

FPSWidget::FPSWidget(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

}

void FPSWidget::render(unsigned int fps, glm::vec3 pos, glm::vec3 playerChunk) 
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Debug");

    ImGui::Text("FPS: %d", fps);
    ImGui::Text("Position: x: %f, y: %f, z: %f", pos.x, pos.y, pos.z);
    ImGui::Text("Chunk: x: %g, y: %g, z: %g", playerChunk.x, playerChunk.y, playerChunk.z);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}