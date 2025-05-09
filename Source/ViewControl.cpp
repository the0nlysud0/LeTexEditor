#include "ViewControl.h"
#include <glm/ext/matrix_transform.hpp>


ViewControl::ViewControl(GLFWwindow *window, Shader shader) : m_window(window)
{
  m_viewMat = glm::mat4(1.0f);
  m_shader = &shader;
  glfwSetWindowUserPointer(m_window , this);
  glfwSetScrollCallback(m_window , scroll_kallback);
  m_shader->use();
  m_shader->addMat4("view",m_viewMat);
}

void ViewControl::onScroll(double xoffset, double yoffset)
{
  m_viewMat = glm::translate(m_viewMat , glm::vec3(0.0f,1.0f*yoffset * 2.0,0.0f));
  m_shader->addMat4("view",m_viewMat);
}

ViewControl::~ViewControl()
{
}

void ViewControl::scroll_kallback(GLFWwindow *window, double xoffset, double yoffset)
{
  ViewControl* viewsmth = static_cast<ViewControl*>(glfwGetWindowUserPointer(window));
  if(viewsmth)
  {
    viewsmth->onScroll(xoffset , yoffset);
  }
}
