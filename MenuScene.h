#pragma once

#include "SceneManager.h"
#include "GameScene.h"
#include "TextButton.h"

class MenuScene
  : public FW::SceneManager::BaseScene
{
public:
  MenuScene(FW::Application& app);
  void update(const float delta);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  std::shared_ptr<TextButton> play;
  std::shared_ptr<TextButton> quit;
  sf::Text instructions;
};