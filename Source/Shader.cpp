#include "Shader.h"
#include "Utils.hpp"

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(char *argv0)
{
  m_ShaderID = glCreateProgram();
  m_ExecutablePath = std::filesystem::absolute(argv0).parent_path();
  m_ShaderFolder = m_ExecutablePath / "../../Assets/Shaders/";
}
unsigned int Shader::getProgramID() const
{
  return m_ShaderID;
}
void Shader::LinkProgram() const
{
  glLinkProgram(m_ShaderID);
  int success;
  char infoLog[512];
  glGetProgramiv(m_ShaderID ,GL_LINK_STATUS ,&success);
  if(!success)
  {
    glGetProgramInfoLog(m_ShaderID , 512,nullptr,infoLog);
    LOG("ERROR::PROGRAM::LINKER::LINKING FAILED\n",infoLog);
  }
}
Shader::~Shader()
{
  //glDeleteProgram(m_ShaderID);
}

unsigned int Shader::CreateShader(std::string ShaderPath, GLenum type)
{
  std::string ShaderSource = ReadFile(m_ShaderFolder.lexically_normal().string()+ShaderPath);
  const char* ShaderSourceC = ShaderSource.c_str();
  unsigned int Shader = glCreateShader(type);
  glShaderSource(Shader , 1 , &ShaderSourceC,nullptr);
  glCompileShader(Shader);
  int success;
  char infoLog[512];
  glGetShaderiv(Shader , GL_COMPILE_STATUS , &success);
  if(!success)
  {
    glGetShaderInfoLog(Shader , 512 , nullptr , infoLog);
    LOG("ERROR::SHADER::COMPILATION::FAILED\n" , infoLog);
    return 0;
  }
  LOG("INFO::SHADER::COMPILATION::successfully compiled shader\n");
  glAttachShader(m_ShaderID , Shader);
  glDeleteShader(Shader);
  return Shader;
}

void Shader::addVec3(std::string uniformName ,glm::vec3 &vector)
{
  glUniform3fv(glGetUniformLocation(m_ShaderID, uniformName.c_str()),1, glm::value_ptr(vector));
}

void Shader::addMat4(std::string uniformName,glm::mat4 &matrix)
{
  glUniformMatrix4fv(glGetUniformLocation(m_ShaderID, uniformName.c_str()),1,GL_FALSE, glm::value_ptr(matrix));
}

void Shader::use() const
{
  glUseProgram(m_ShaderID);
}
