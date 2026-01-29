
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <string>

#include "camera.h"
#include "model.h"
#include "shaders.h"

using namespace std;

int width = 1920, height = 1080;
bool showUI = true;
const char *project_name = "Lab 2 - Transmitance Effects";

// camera at +Z looking toward origin (-Z)
Camera camera(0.0f, 3.0f, 12.0f, 0.0f, 1.0f, 0.0f, -90.0f, -10.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// -- skybox faces ---
const char *faces[6] = {"assets/skybox/right.jpg", "assets/skybox/left.jpg",
                        "assets/skybox/top.jpg",   "assets/skybox/bottom.jpg",
                        "assets/skybox/front.jpg", "assets/skybox/back.jpg"};

// --- globals for input ---
Camera *gCamera = nullptr;
float gLastX = 0.0f;
float gLastY = 0.0f;
bool gFirstMouse = true;

glm::vec3 lightPos(50.0f, 50.0f, 50.0f);
glm::vec3 lightColor(1.0f);
float lightIntensity = 1.0f;
float orenRoughness = 0.5f;

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

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");

  gCamera = &camera;
  gLastX = width / 2.0f;
  gLastY = height / 2.0f;

  cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;

  // Configure OpenGL
  glEnable(GL_DEPTH_TEST);

  // Load shaders
  Shader shader("assets/shaders/main.vert", "assets/shaders/main.frag");
  Shader skyboxShader("assets/shaders/skybox.vert",
                      "assets/shaders/skybox.frag");
  unsigned int cubemapTexture = skyboxShader.loadCubemap(faces);

  // Load Model
  Model ball("assets/models/ball/source/ball_lp_uw.obj");

  // setup skybox
  float skyboxVertices[] = {
      // positions (36 vertices)
      -1, 1,  -1, -1, -1, -1, 1,  -1, -1, 1,  -1, -1, 1,  1,  -1, -1, 1,  -1,
      -1, -1, 1,  -1, -1, -1, -1, 1,  -1, -1, 1,  -1, -1, 1,  1,  -1, -1, 1,
      1,  -1, -1, 1,  -1, 1,  1,  1,  1,  1,  1,  1,  1,  1,  -1, 1,  -1, -1,
      -1, -1, 1,  -1, 1,  1,  1,  1,  1,  1,  1,  1,  1,  -1, 1,  -1, -1, 1,
      -1, 1,  -1, 1,  1,  -1, 1,  1,  1,  1,  1,  1,  -1, 1,  1,  -1, 1,  -1,
      -1, -1, -1, -1, -1, 1,  1,  -1, -1, 1,  -1, -1, -1, -1, 1,  1,  -1, 1};

  unsigned int skyboxVAO, skyboxVBO;
  glGenVertexArrays(1, &skyboxVAO);
  glGenBuffers(1, &skyboxVBO);
  glBindVertexArray(skyboxVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices,
               GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glBindVertexArray(0);

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

    // ImGui UI
    if (showUI) {
      ImGui::Begin("Scene Controls");
      ImGui::Text("OpenGL Starter Template");
      ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
      ImGui::Separator();
      ImGui::Text("Camera Position: (%.1f, %.1f, %.1f)", camera.position.x,
                  camera.position.y, camera.position.z);
      ImGui::Text("Press TAB to toggle UI/Camera control");
      ImGui::End();
    }

    // Set transformations
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.zoom), (float)width / (float)height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    // Use shader and set uniforms
    shader.use();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    ball.Draw(shader);

    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);

    skyboxShader.use();
    skyboxShader.setMat4("projection", projection);
    skyboxShader.setMat4("view", view);
    skyboxShader.setInt("skybox", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);

    if (showUI) {
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
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
