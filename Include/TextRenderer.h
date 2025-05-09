#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

#include "Shader.h"

struct Character {
  unsigned int TextureID; // ID handle of the glyph texture
  glm::ivec2   Size;      // Size of glyph
  glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
  unsigned int Advance;   // Horizontal offset to advance to next glyph
};

struct Font {
  std::string FontFamily;
  std::map<GLchar, Character> Characters;
  float line_height;
};

enum class TextAlign{
  Left , Center , Right
};

struct TextStyle{
  bool isFontLoaded=0;
  std::string FontName;
  int FontSize;
  glm::vec3 color;
  float scale;
  TextStyle() : FontSize(48) , color(glm::vec3(1.0f,1.0f,1.0f)) ,  scale(1.0f) {}
};

struct TextBlock{
  std::string content;
  TextStyle style;
  glm::vec2 position;
  float maxWidth;
  TextAlign align;
};

// TODO: improve the text rendering with instancing :)

class TextRenderer
{
private:
  std::vector<std::string> m_TextLines;
  std::map<std::string, std::map<int, Font>> m_Fonts;

  std::string m_CurrentFont;
  char* m_Argv0;
  unsigned int m_TextVAO;
  unsigned int m_TextVBO;
public:
  TextRenderer(char* argv0);
  void loadFont(std::string fontPath , int size =48);
  void loadFontInstanced(std::string fontPath);
  void renderText(GLFWwindow* window ,Shader& shader , std::string Text , TextStyle Settings);
  void renderTextBlock(GLFWwindow* window, Shader& shader , TextBlock text);
  void renderSingleLine(std::string line , TextBlock properties);
  ~TextRenderer();
};