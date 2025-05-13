#pragma once
#include "Shader.h"
#include <unordered_map>
struct Cursor
{
  size_t line=0;
  size_t column=0;
};



class InputSystem
{
private:
  GLFWwindow*               m_window;
  std::vector<bool>         m_isKeyPressed;
  std::vector<std::string>  m_filePaths;
  std::string               m_fileText;
  glm::mat4                 m_viewMat;
  char*                     m_argv0;
  Shader*                   m_shader;
  double                    m_scrollSens = 8.0;
  double                    m_zoomFactor = 1.0;
  Cursor                    m_cursorPosition;
public:
  InputSystem                 (GLFWwindow* window , Shader& shader, char* argv0);
  std::string GetTextInput    (int index =0);
  double      GetZoomFactor   () const
  {
    return m_zoomFactor;
  }
  std::vector<bool> GetPressedKeys() const
  {
    return m_isKeyPressed;
  }
  size_t getCursorLine()
  {
    return m_cursorPosition.line;
  }
  size_t getCursorColumn()
  {
    return m_cursorPosition.column;
  } 
  ~InputSystem();
private:
  void        OnKey           (int key ,int action);
  void        OnScroll        (double xoffset,double yoffset);
  void        OnType          (unsigned int codepoint);
  static void key_kallback    (GLFWwindow* window, int key, int scancode, int action, int mods);
  static void scroll_kallback (GLFWwindow* window, double xoffset , double yoffset);
  static void char_kallback   (GLFWwindow* window, unsigned int codepoint);
};