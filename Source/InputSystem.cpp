#include "InputSystem.h"
#include <glm/ext/matrix_transform.hpp>
#include <algorithm>


InputSystem::InputSystem(GLFWwindow *window, Shader& shader, char* argv0)
{
  m_window = window;
  m_shader = &shader;
  m_viewMat = glm::mat4(1.0f);
  m_argv0 = argv0;
  m_isKeyPressed.resize   (GLFW_KEY_LAST,false);
  // Kallbacks
  glfwSetWindowUserPointer(m_window , this);
  glfwSetKeyCallback      (m_window , key_kallback);
  glfwSetScrollCallback   (m_window , scroll_kallback);
  glfwSetCharCallback     (m_window , char_kallback);

  m_shader->use();
  m_shader->addMat4       ("view",m_viewMat);
  // Reading last opened file 
  std::string SettingsPath =
                   getPath(argv0,PARAMS_PATH)+"lastfile.txt";
  m_filePaths.push_back (ReadFile(SettingsPath));
  m_fileText = ReadFile (m_filePaths[0]);
  ////
}

std::string InputSystem::GetTextInput(int index)
{
  return m_fileText;
}

void InputSystem::key_kallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  InputSystem* instance = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
  if(instance)
  {
    instance->OnKey(key,action);
  }
}

void InputSystem::scroll_kallback(GLFWwindow *window, double xoffset, double yoffset)
{
  InputSystem* instance = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
  if(instance)
  {
    instance->OnScroll(xoffset,yoffset);
  }
}

void InputSystem::char_kallback(GLFWwindow *window, unsigned int codepoint)
{
  InputSystem* instance = static_cast<InputSystem*>(glfwGetWindowUserPointer(window));
  if(instance)
  {
    instance->OnType(codepoint);
  }
}

void InputSystem::OnKey(int key, int action)
{
  if(action==GLFW_PRESS||action==GLFW_REPEAT)
  {
    if(key == GLFW_KEY_BACKSPACE && !m_fileText.empty())
    {
      m_fileText.pop_back();
      if (m_cursorPosition.column > 0) m_cursorPosition.column--;
    }
    if(key == GLFW_KEY_ENTER)
    {
      m_fileText+= '\n';
      m_cursorPosition.line++;
      m_cursorPosition.column = 0; // <- move to new line
    }
    switch (key) {
      case GLFW_KEY_LEFT:  if (m_cursorPosition.column > 0) m_cursorPosition.column--; break;
      case GLFW_KEY_RIGHT: m_cursorPosition.column++; break;
      case GLFW_KEY_UP:    if (m_cursorPosition.line > 0) m_cursorPosition.line--; break;
      case GLFW_KEY_DOWN:  m_cursorPosition.line++; break;
  }
    m_isKeyPressed[key] = true;
  }
  else if(action==GLFW_RELEASE)
  {
    m_isKeyPressed[key]=false;
  }
  // TODO: Shortcuts here
  if(m_isKeyPressed[GLFW_KEY_LEFT_CONTROL]==true)
  {
    if(m_isKeyPressed[GLFW_KEY_O]==true)
    {
      std::string filePath = OpenFileDialog();
      m_filePaths.push_back(filePath);
      m_fileText = ReadFile(filePath);
    }
  }
}

void InputSystem::OnScroll(double xoffset, double yoffset)
{
  if(m_isKeyPressed[GLFW_KEY_LEFT_CONTROL])
  {
    float zoomSpeed = 0.1f;
    float scale = 1.0f - zoomSpeed * (float)yoffset;

    scale = std::clamp(scale, 0.01f, 10.0f);
    
    m_zoomFactor *= scale;
    //m_zoomFactor = std::clamp(m_zoomFactor , 0.01 ,1000.0);
  } else {
    m_viewMat = glm::translate(m_viewMat,glm::vec3(0.0,-yoffset*m_scrollSens,0.0));
  }
  m_shader->use();
  m_shader->addMat4("view",m_viewMat);
}

void InputSystem::OnType(unsigned int codepoint)
{
  m_fileText += (char)codepoint;
  m_cursorPosition.column++; // <- add this
}

InputSystem::~InputSystem()
{
  std::string SettingsPath=getPath(m_argv0,PARAMS_PATH)+"lastfile.txt";
  for(std::string path : m_filePaths)
  {
    Write2File(SettingsPath,path);
  }
}
