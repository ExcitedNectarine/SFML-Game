#pragma once

#include "EntityManager.h"
#include <SFML/Graphics.hpp>

class TextButton
  : public FW::EntityManager::BaseEntity
{
public:
  TextButton(
    FW::Application& app,
    FW::EntityManager& entities
  );
  void setup(const std::string& font, const std::string& str, const int size);
  void update(const float delta);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  bool isPressed() { return pressed; }

private:
  sf::Text text;
  bool pressed = false;
  sf::FloatRect rect;
};