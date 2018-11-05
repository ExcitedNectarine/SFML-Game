#include "TextButton.h"
#include "Application.h"

TextButton::TextButton(FW::Application& app, FW::EntityManager& entities)
  : FW::EntityManager::BaseEntity(app, entities)
{
}

void TextButton::setup(const std::string& font, const std::string& str, const int size)
{
  text.setFont(app.getResourceManager().getFont(font));
  text.setString(str);
  text.setCharacterSize(size);
}

void TextButton::update(const float delta)
{
  // reset the pressed variable if it was set to true
  // can only be pressed once if not reset.
  if (pressed)
  {
    pressed = false;
  }

  rect = text.getGlobalBounds();
  rect.left = getPosition().x;
  rect.top = getPosition().y;

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
  {
    sf::Vector2f mouse_pos = sf::Vector2f(sf::Mouse::getPosition(app.getWindow()));
    if (rect.contains(mouse_pos))
    {
      pressed = true;
    }
  }
}

void TextButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(text, states);
}