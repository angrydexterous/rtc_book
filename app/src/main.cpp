#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "spdlog/spdlog.h"
#include "imgui.h"
#include "imgui-bindings/imgui_impl_glfw.h"
#include "imgui-bindings/imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h> // Must happen after OpenGL init

#include "app/math.h"
#include "app/canvas.h"
#include "app/color.h"

constexpr int canvas_width = 500;
constexpr int canvas_height = 500;

static void glfw_error_callback(int error, const char *description)
{
  spdlog::error("Glfw Error {}: {}\n", error, description);
}

// imgui sample taken from
// https://github.com/conan-io/examples/tree/master/libraries/dear-imgui/basic
int main(int, char **)
{
  spdlog::info("Program Starting!");
  auto canvas = Canvas<float>(canvas_width, canvas_height);
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

    // Decide GL+GLSL versions
#if __APPLE__
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

  // Create window with graphics context
  GLFWwindow *window = glfwCreateWindow(1280, 720, "Dear ImGui - Conan", NULL, NULL);
  if (window == NULL)
    return 1;
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  bool err = glewInit() != GLEW_OK;
  if (err)
  {
    spdlog::error("Failed to initialize OpenGL loader!\n");
    return 1;
  }

  int screen_width, screen_height;
  glfwGetFramebufferSize(window, &screen_width, &screen_height);
  glViewport(0, 0, screen_width, screen_height);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  static float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

  // Init Texture
  GLuint texture_id_;
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);

  ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
  ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
  ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
  ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glClearColor(color[0], color[1], color[2], color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // render your GUI
    ImGui::Begin("Point Position/Color");
    static float translation[] = {0.0, 0.0};
    ImGui::SliderFloat2("position", translation, -1.0, 1.0);
    // color picker
    ImGui::ColorEdit3("color", color);
    ImGui::End();

    // Update Canvas
    auto w = std::clamp((int)((canvas_width / 2) + (translation[0] * (canvas_width / 2))), 0, canvas_width - 1);
    auto h = std::clamp((int)((canvas_height / 2) + (translation[1] * (canvas_height / 2))), 0, canvas_height - 1);
    canvas.writePixel(Color::Color(color[0], color[1], color[2]), w, h);
    // Render Texture - Canvas Window
    ImGui::Begin("Preview");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, canvas.width(), canvas.height(), 0, GL_RGB, GL_FLOAT, canvas.data());
    glGenerateMipmap(GL_TEXTURE_2D);
    ImGui::Image((void *)(intptr_t)texture_id_, ImVec2(canvas.width(), canvas.height()), uv_min, uv_max, tint_col, border_col);
    ImGui::End();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glfwSwapBuffers(window);

    // Clear
    canvas.writePixel(Color::Color(0.f, 0.f, 0.f), w, h);
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}