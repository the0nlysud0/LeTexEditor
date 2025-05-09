#include "UserInterface.h"

UserInterface::UserInterface(GLFWwindow *window ,std::string argv0)
{
  m_TextRenderer = std::make_unique<TextRenderer>(argv0);
}

// 
template<>
void UserInterface::addElement<Triangle>(Triangle triangle)
{
    // m_ElementIDs[DivType::Triangle] = /* your logic */;
    m_ElementIDs[DivType::Triangle] = ++m_Count;
}

// Specialization for RoundedRectangle
template<>
void UserInterface::addElement<RoundedRectangle>(RoundedRectangle round_rect)
{
    // m_ElementIDs[DivType::RoundedRectangle] = /* your logic */;
}

template<>
void UserInterface::addElement<Label>(Label label)
{
  m_TextRenderer->loadFont(label.text.style.FontName);
}

UserInterface::~UserInterface()
{
}
