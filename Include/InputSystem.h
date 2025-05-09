#pragma once
#include "Shader.h"
#include "TextRenderer.h"
#include <unordered_map>
class InputSystem
{
private:
  GLFWwindow* m_window;
  std::vector<bool> m_isKeyPressed;
  std::vector<std::string> m_filePaths;
  std::string m_fileText;
  glm::mat4 m_viewMat;
  char* m_argv0;
  Shader* m_shader;
  double m_scrollSens = 8.0;
  double m_zoomFactor =1.0;
public:
  InputSystem(GLFWwindow* window , Shader& shader, char* argv0);
  std::string GetTextInput(int index =0);
  double GetZoomFactor() const
  {
    return m_zoomFactor;
  }
  ~InputSystem();
private:
  void OnKey(int key ,int action);
  void OnScroll(double xoffset,double yoffset);
  static void key_kallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void scroll_kallback(GLFWwindow* window, double xoffset , double yoffset);
};