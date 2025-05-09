#include <filesystem>
#include "Shader.h"
#include "TextRenderer.h"
#include "ViewControl.h"
#include "InputSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main(int argc, char** argv)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR ,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR ,3);
  // glfwWindowHint(GLFW_DECORATED , GLFW_FALSE);
  GLFWwindow* window = glfwCreateWindow(800,800,"LeTexEditor" , 0,0);
  glfwMakeContextCurrent(window);
  glewExperimental =true;
  glewInit();

  Shader TextShader(argv[0]);
  TextShader.CreateShader("text.vs.glsl", GL_VERTEX_SHADER);
  TextShader.CreateShader("text.fs.glsl", GL_FRAGMENT_SHADER);
  TextShader.LinkProgram();
  glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(800),static_cast<float>(800) ,0.0f);
  TextShader.use();
  glUniformMatrix4fv(glGetUniformLocation(TextShader.getProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

  TextRenderer text(argv[0]);
  ViewControl view(window , TextShader);
  InputSystem input(window, TextShader,argv[0]);
  //text.loadFont("InputSans-Regular.ttf");
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(44.0f/255.0f,49.0f/255.0f,60.0f/255.0f,1.0f);
    // Update viewport and projection
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(width),
                                  0.0f, static_cast<float>(height));
    glUniformMatrix4fv(glGetUniformLocation(TextShader.getProgramID(), "projection"),1, GL_FALSE, glm::value_ptr(projection));

    // Render text once
    TextBlock u_text;
    u_text.content = input.GetTextInput();
    u_text.maxWidth = width;
    u_text.position = glm::vec2(0.0f,0.0f);
    u_text.style.color = glm::vec3(97.0f/255.0f,175.0f/255.0f,239.0f/255.0f);
    u_text.style.FontName = "InputSans-Regular.ttf";
    u_text.style.FontSize = 18/ input.GetZoomFactor();
    u_text.style.scale = 1.25f;
    u_text.align = TextAlign::Left;
    text.renderTextBlock(window, TextShader , u_text);

    glfwPollEvents();
    glfwSwapBuffers(window);
}

  glfwTerminate();
}