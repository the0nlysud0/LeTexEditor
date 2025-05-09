#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

#include "Utils.hpp"

class Shader
{
private:
  uint32_t m_ShaderID=0;
  std::filesystem::path m_ExecutablePath;
  std::filesystem::path m_ShaderFolder;
public:
  Shader(char* argv0);
  unsigned int getProgramID() const;
  void LinkProgram() const;
  unsigned int CreateShader(std::string ShaderPath, GLenum type);
  void addVec3(std::string uniformName,glm::vec3& vector);
  void addMat4(std::string uniformName,glm::mat4& matrix);
  void use() const;
  ~Shader();
};