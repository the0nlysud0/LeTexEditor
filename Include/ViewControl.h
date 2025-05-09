#pragma once

#include "Shader.h"


class ViewControl
{
private:
  glm::mat4 m_viewMat;
  GLFWwindow* m_window;
  static void scroll_kallback(GLFWwindow* window, double xoffset , double yoffset);
  Shader* m_shader;
public:
  ViewControl(GLFWwindow* window ,Shader shader);
  void onScroll(double xoffset, double yoffset);
  // void update();
  ~ViewControl();
};