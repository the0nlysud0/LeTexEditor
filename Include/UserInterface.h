#pragma once
#include "TextRenderer.h"
#include <map>

enum class DivType
{
  Triangle,
  TriangleOutline,
  RoundedRectangle,
  RoundedRectangleOutline,
  Label,
  Paragraph,
  TextBox,
};

struct Triangle     {};
struct TriangleOutline{};
struct RoundedRectangle{};
struct RoundedRectangleOutline{};
struct Label{
  TextBlock text;
};
struct Paragraph{};
struct TextBox{};

class UserInterface
{
private:
  std::unique_ptr<TextRenderer> m_TextRenderer;
  std::map<DivType ,unsigned int> m_ElementIDs;
  unsigned int m_Count=0;
public:
  UserInterface(GLFWwindow* window , std::string argv0);
  template<typename Element>
  void UserInterface::addElement(Element element);
  void removeElement();
  ~UserInterface();
};

template<typename Element>
void UserInterface::addElement(Element element)
{
    constexpr DivType type = DivTypeTraits<Shape>::type;
    m_ElementIDs[type] = 42; // just a placeholder logic
}