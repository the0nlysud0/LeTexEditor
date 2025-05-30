#include "TextRenderer.h"

#include <ft2build.h>
#include <Utils.hpp>
#include <algorithm>
#include FT_FREETYPE_H

float getWordWidth(const std::string& word, const Font& font, float scale) {
  float width = 0.0f;
  for (char c : word) {
      const Character& ch = font.Characters.at(c);
      width += (ch.Advance >> 6) * scale;
  }
  return width;
}

float getCharWidth(const char c, const Font& font, float scale) {
  float width = 0.0f;
  const Character& ch = font.Characters.at(c);
  width += (ch.Advance >> 6) * scale;
  return width;
}

TextRenderer::TextRenderer(char* argv0)
{
  m_Argv0 = argv0;
  glGenVertexArrays(1, &m_TextVAO);
  glGenBuffers(1, &m_TextVBO);
  m_CurrentTime = glfwGetTime();
  glBindVertexArray(m_TextVAO);
  glBindBuffer(GL_ARRAY_BUFFER , m_TextVBO);
  glBufferData(GL_ARRAY_BUFFER , sizeof(float)*6*4 , NULL , GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  glGenVertexArrays(1, &m_CursorVAO);
  glGenBuffers(1, &m_CursorVBO);

  glBindVertexArray(m_CursorVAO);
  glBindBuffer(GL_ARRAY_BUFFER , m_CursorVBO);
  glBufferData(GL_ARRAY_BUFFER , sizeof(float)*2*2 , NULL , GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TextRenderer::attachInputSystem(InputSystem &p_input_sys)
{
  m_InputSystem = &p_input_sys;
}

void TextRenderer::loadFont(std::string& fontName ,int size)
{
  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
      LOG("ERROR::FREETYPE: Could not init FreeType Library\n");
      return;
  }
  FT_Face face;
  std::string fullPath = getPath(m_Argv0, FONT_PATH) + fontName;
  if (FT_New_Face(ft, fullPath.c_str(), 0, &face)) {
    LOG("ERROR::FREETYPE: Failed to load font\n");
    return;
  }

  FT_Set_Pixel_Sizes(face, 0, size);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  int MaxGlyphBearing=0;
  for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
        LOG("ERROR::FREETYTPE: Failed to load Glyph\n");
        continue;
    }
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int glyphBearingHeight = face->glyph->bitmap_top;
    // MaxGlyphBearing = std::max(MaxGlyphBearing , glyphBearingHeight);

    Character character = {
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        static_cast<unsigned int>(face->glyph->advance.x)
    };
    m_Fonts[fontName][size].FontFamily = fontName;
    m_Fonts[fontName][size].Characters.insert(std::pair<char, Character>(c, character));
    m_Fonts[fontName][size].line_height = face->size->metrics.height >> 6;
  }
  glBindTexture(GL_TEXTURE_2D, 0);
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

vector<std::string> TextRenderer::toLine(std::vector<std::string> &tokens , TextBlock& text)
{
  std::vector<std::string> lines;
  std::string currentLine;
  float currentLineWidth = 0.0f; 
  float MaxLineWidth = text.maxWidth - text.position.x;
  for (const std::string& token : tokens) {
    if (token == "\n") {
      lines.push_back(currentLine);
      currentLine.clear();
      currentLineWidth = 0.0f;
      continue;
    }
  
    float tokenWidth = getWordWidth(token, m_Fonts[text.style.FontName][text.style.FontSize], text.style.scale);
  
    if (currentLineWidth + tokenWidth > MaxLineWidth && !currentLine.empty()) {
      lines.push_back(currentLine);
      currentLine = "";
      currentLineWidth = 0.0f;
    }
  
    currentLine += token;
    currentLineWidth += tokenWidth;
  }
  
  if (!currentLine.empty()) {
    lines.push_back(currentLine);
  }

  return lines;
}

vector<std::string> TextRenderer::toLine(std::string &fullText , TextBlock& text)
{
  std::vector<std::string> tokens = toToken(fullText);
  std::vector<std::string> lines;
  std::string currentLine;
  float currentLineWidth = 0.0f; 
  float MaxLineWidth = text.maxWidth - text.position.x;
  for (const std::string& token : tokens) {
    if (token == "\n") {
      lines.push_back(currentLine);
      currentLine.clear();
      currentLineWidth = 0.0f;
      continue;
    }
  
    float tokenWidth = getWordWidth(token, m_Fonts[text.style.FontName][text.style.FontSize], text.style.scale);
  
    if (currentLineWidth + tokenWidth > MaxLineWidth && !currentLine.empty()) {
      lines.push_back(currentLine);
      currentLine = "";
      currentLineWidth = 0.0f;
    }
  
    currentLine += token;
    currentLineWidth += tokenWidth;
  }
  
  if (!currentLine.empty()) {
    lines.push_back(currentLine);
  }
  return lines;
}

vector<std::string> TextRenderer::toToken(std::string &fullText)
{
  std::vector<std::string> tokens;
  std::string current;
  for (char c : fullText) {
    if (c == ' ' || c == '\n') {
      if (!current.empty()) {
        tokens.push_back(current);
        current.clear();
      }
      std::string s(1, c); // capture the space or newline as its own "word"
      tokens.push_back(s);
    } else {
      current += c;
    }
  }
  if (!current.empty())
    tokens.push_back(current);
  return tokens;
}

void TextRenderer::renderTextBlock(GLFWwindow *window, Shader &shader, TextBlock& text)
{
  if(m_Fonts.find(text.style.FontName)==m_Fonts.end()||m_Fonts[text.style.FontName].find(text.style.FontSize)==m_Fonts[text.style.FontName].end())
  {
    loadFont(text.style.FontName , text.style.FontSize);
    LOG("INFO::RENDERING::TEXT::Font ", text.style.FontName , " was loaded\n");
  }
  shader.use();
  glUniform3f(glGetUniformLocation(shader.getProgramID(), "textColor"), text.style.color.x, text.style.color.y,text.style.color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(m_TextVAO);
  int width , height;
  glfwGetWindowSize(window , &width , &height);
  text.position.y = height - text.position.y - m_Fonts[text.style.FontName][text.style.FontSize].line_height;
  float baseX = text.position.x;
  float baseY = text.position.y;

  std::vector<std::string> tokens;
  std::string current;
  for (char c : text.content) {
    if (c == ' ' || c == '\n') {
      if (!current.empty()) {
        tokens.push_back(current);
        current.clear();
      }
      std::string s(1, c); // capture the space or newline as its own "word"
      tokens.push_back(s);
    } else {
      current += c;
    }
  }
  if (!current.empty())
    tokens.push_back(current);

  std::vector<std::string> lines;
  std::string currentLine;
  float currentLineWidth = 0.0f; 
  float MaxLineWidth = text.maxWidth - text.position.x;
  for (const std::string& token : tokens) {
    if (token == "\n") {
      lines.push_back(currentLine);
      currentLine.clear();
      currentLineWidth = 0.0f;
      continue;
    }
  
    float tokenWidth = getWordWidth(token, m_Fonts[text.style.FontName][text.style.FontSize], text.style.scale);
  
    if (currentLineWidth + tokenWidth > MaxLineWidth && !currentLine.empty()) {
      lines.push_back(currentLine);
      currentLine = "";
      currentLineWidth = 0.0f;
    }
  
    currentLine += token;
    currentLineWidth += tokenWidth;
  }
  
  if (!currentLine.empty()) {
    lines.push_back(currentLine);
  }

  float lineHeight = m_Fonts[text.style.FontName][text.style.FontSize].line_height * text.style.scale + 0.3f;
  switch (text.align)
  {
  case TextAlign::Left:
    break;
  case TextAlign::Center:
    baseX = width / 2.0f - MaxLineWidth / 2.0f;
    break;
  case TextAlign::Right:
    baseX = width -(MaxLineWidth + text.position.x);
    break;
  default:
    break;
  }
  for(std::string& line : lines) {
    text.position.x = baseX;
    renderSingleLine(line, text);
    text.position.y -= lineHeight;
  }
    // now draw the caret
  if (lines.empty()) {
    lines.push_back(""); // prevent crash
  }
    m_Cursor.line = lines.size() - 1;
    m_Cursor.column = lines.back().size();
    m_Cursor.column = m_InputSystem->getCursorColumn();
    m_Cursor.line = m_InputSystem->getCursorLine();

    // clamping the the line and column
    // m_Cursor.line = std::clamp(m_Cursor.line, size_t(0), lines.size() - 1);
    // m_Cursor.column = std::clamp(m_Cursor.column, size_t(0), lines[m_Cursor.line].size());

    float cx = baseX;
    for (size_t i = 0; i < m_Cursor.column && i < lines[m_Cursor.line].size(); ++i) {
        cx += getCharWidth(lines[m_Cursor.line][i], m_Fonts[text.style.FontName][text.style.FontSize], text.style.scale);
    }
    float cy = baseY - m_Cursor.line * lineHeight + lineHeight;
      // m_CurrentTime = glfwGetTime();

    shader.use();
    shader.addVec3("textColor" , glm::vec3(1.0f,1.0f,1.0f));
    glDisable(GL_DEPTH_TEST); 
    glDisable(GL_CULL_FACE);
    renderCursor(cx, cy, lineHeight);

    LOG("col:",m_Cursor.column," line: ", m_Cursor.line ,'\n');
  

  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::renderSingleLine(std::string line, TextBlock& properties)
{
  float baseX = properties.position.x;
  for(char c : line) {
    if(c == '\n')
    {
      continue;
    }
    const auto& ch = m_Fonts[properties.style.FontName][properties.style.FontSize].Characters[c];
    float lineHeight = m_Fonts[properties.style.FontName][properties.style.FontSize].line_height * properties.style.scale + 0.3f;
    
    float xpos = properties.position.x + ch.Bearing.x * properties.style.scale;
    float ypos = properties.position.y + ch.Bearing.y * properties.style.scale - ch.Size.y * properties.style.scale;

    float w = ch.Size.x * properties.style.scale;
    float h = ch.Size.y * properties.style.scale;


    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 0.0f },            
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f }           
    };

    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, m_TextVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    properties.position.x += (ch.Advance >> 6) * properties.style.scale;
  }
}

void TextRenderer::renderCursor(float x, float y, float height) {
  m_CurrentTime = glfwGetTime();
  if(m_CurrentTime - m_LastBlinkTime >= m_BlinkInterval)
  {
    m_CursorVisible = !m_CursorVisible;
    m_LastBlinkTime = m_CurrentTime;
  }
  // if(!m_CursorVisible) return;
  float caretWidth = 2.0f; // pixels
    float A[6][2] = {
        {x, y},
        {x + caretWidth, y},
        {x + caretWidth, y - height - 5.0f},

        {x, y},
        {x + caretWidth, y - height-5.0f},
        {x, y - height-5.0f}
    };

    glBindVertexArray(m_CursorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_CursorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(A), A, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer() {}

Character &TextRenderer::getGlyph(const std::string &fontName, int fontSize, char c)
{
  return m_Fonts[fontName][fontSize].Characters.at(c);
}
