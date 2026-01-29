
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_style.h"

#include <string>

#include "camera.h"
#include "model.h"
#include "shaders.h"

using namespace std;

int width = 1920, height = 1080;
bool showUI = true;
const char *project_name = "Lab 1 - Reflectance Models";

// camera at +Z looking toward origin (-Z)
Camera camera(0.0f, 3.0f, 12.0f, 0.0f, 1.0f, 0.0f, -90.0f, -10.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// --- globals for input ---
Camera *gCamera = nullptr;
float gLastX = 0.0f;
float gLastY = 0.0f;
bool gFirstMouse = true;

float spacing = 4.0f; // spacing between models

glm::vec3 lightPos(50.0f, 50.0f, 50.0f);
glm::vec3 lightColor(1.0f);
float lightIntensity = 1.0f;
float orenRoughness = 0.5f;
float toonBands = 4.0f;
float toonMinShade = 0.1f;
float phongShininess = 32.0f;
float phongSpecularStrength = 0.5f;

glm::vec3 globalObjectColor(1.0f);

glm::vec3 modelColors[3] = {
    {1.0f, 0.3f, 0.3f}, {0.3f, 1.0f, 0.3f}, {0.3f, 0.3f, 1.0f}};

bool linkModelColors = true;

void framebuffer_size_callback(GLFWwindow *window, int w, int h) {
  glViewport(0, 0, w, h);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {

  if (showUI)
    return;

  if (!gCamera)
    return;

  if (gFirstMouse) {
    gLastX = (float)xpos;
    gLastY = (float)ypos;
    gFirstMouse = false;
  }

  float xoffset = (float)xpos - gLastX;
  float yoffset = gLastY - (float)ypos; // reversed: y ranges bottom->top
  gLastX = (float)xpos;
  gLastY = (float)ypos;

  gCamera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  if (!gCamera)
    return;
  if (ImGui::GetIO().WantCaptureMouse)
    return;
  gCamera->ProcessMouseScroll((float)yoffset);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
    showUI = !showUI;
    glfwSetInputMode(window, GLFW_CURSOR,
                     showUI ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    gFirstMouse = true;
  }
}

void processInput(GLFWwindow *window) {

  if (showUI)
    return;

  if (!gCamera)
    return;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    gCamera->ProcessKeyboard(FRONT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    gCamera->ProcessKeyboard(BACK, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    gCamera->ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    gCamera->ProcessKeyboard(RIGHT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    gCamera->ProcessKeyboard(UP, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    gCamera->ProcessKeyboard(DOWN, deltaTime);
}

int main() {

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window =
      glfwCreateWindow(width, height, project_name, nullptr, nullptr);

  if (!window) {
    cerr << "Failed to create window\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetKeyCallback(window, key_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cerr << "Failed to initialize GLAD\n";
    return -1;
  }

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ApplyCustomStyle();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  gCamera = &camera;
  gLastX = width / 2.0f;
  gLastY = height / 2.0f;

  cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

  // Configure OpenGL
  glEnable(GL_DEPTH_TEST);

  // Load shaders
  Shader phongShader("assets/shaders/phong.vert", "assets/shaders/phong.frag");

  Shader toonShader("assets/shaders/toon.vert", "assets/shaders/toon.frag");

  Shader orenNayer("assets/shaders/oren_nayer.vert",
                   "assets/shaders/oren_nayer.frag");

  // Load model
  Model kolobok("assets/models/kolobok/source/Kolobok.fbx");
  // Model kolobok("assets/models/utah_teapot.obj");

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Clear buffers
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui::Begin("Scene Controls");

    // ----- LIGHT -----
    ImGui::Text("Light");
    ImGui::DragFloat3("Light Position", &lightPos.x, 0.5f);
    ImGui::ColorEdit3("Light Color", &lightColor.x);
    ImGui::SliderFloat("Light Intensity", &lightIntensity, 0.0f, 5.0f);

    ImGui::Separator();

    // ----- MODELS -----
    ImGui::Checkbox("Link Model Colors", &linkModelColors);
    ImGui::SliderFloat("Model Spacing", &spacing, 1.0f, 20.0f);

    if (linkModelColors) {
      ImGui::ColorEdit3("Model Color", &globalObjectColor.x);
    } else {
      ImGui::ColorEdit3("Phong Model", &modelColors[0].x);
      ImGui::ColorEdit3("Toon Model", &modelColors[1].x);
      ImGui::ColorEdit3("Oren-Nayar Model", &modelColors[2].x);
    }

    ImGui::Separator();

    // ----- OREN–NAYAR -----
    ImGui::SliderFloat("Oren Roughness", &orenRoughness, 0.0f, 1.0f);

    ImGui::Separator();

    // ----- TOON -----
    ImGui::SliderFloat("Toon Bands", &toonBands, 1.0f, 10.0f);
    ImGui::SliderFloat("Toon Min Shade", &toonMinShade, 0.0f, 1.0f);
    ImGui::Separator();

    // ----- PHONG -----
    ImGui::SliderFloat("Phong Shininess", &phongShininess, 1.0f, 256.0f);
    ImGui::SliderFloat("Phong Specular Strength", &phongSpecularStrength, 0.0f,
                       1.0f);
    ImGui::Separator();

    ImGui::End();

    // Set transformations
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.zoom), (float)width / (float)height, 0.1f, 1000.0f);
    glm::mat4 view = camera.GetViewMatrix();
    float angle = glfwGetTime();

    // Put three copies along X
    // spacing for teapot = 15.0f
    glm::vec3 positions[3] = {glm::vec3(-spacing, 0.0f, 0.0f),
                              glm::vec3(0.0f, 0.0f, 0.0f),
                              glm::vec3(spacing, 0.0f, 0.0f)};

    Shader *shaders[3] = {&phongShader, &toonShader, &orenNayer};

    for (int i = 0; i < 3; i++) {

      glm::vec3 colorToUse =
          linkModelColors ? globalObjectColor : modelColors[i];

      shaders[i]->use();
      shaders[i]->setMat4("projection", projection);
      shaders[i]->setMat4("view", view);

      shaders[i]->setVec3("lightPos", lightPos);
      shaders[i]->setVec3("lightColor", lightColor * lightIntensity);
      shaders[i]->setVec3("objectColor", colorToUse);
      shaders[i]->setVec3("viewPos", camera.position);

      if (shaders[i] == &toonShader) {
        shaders[i]->setFloat("bands", toonBands);
        shaders[i]->setFloat("minShade", toonMinShade);
      }

      if (shaders[i] == &phongShader) {
        shaders[i]->setFloat("shininess", phongShininess);
        shaders[i]->setFloat("ks", phongSpecularStrength);
      }

      if (shaders[i] == &orenNayer) {
        shaders[i]->setFloat("roughness", orenRoughness);
      }

      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, positions[i]);
      model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::scale(model, glm::vec3(2.0f));
      shaders[i]->setMat4("model", model);

      kolobok.Draw(*shaders[i]);
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();

  return 0;
}
