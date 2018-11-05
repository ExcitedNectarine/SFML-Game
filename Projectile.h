#pragma once

#include "EntityManager.h"
#include "TileMap.h"
#include "Enemy.h"

class Projectile
    : public FW::EntityManager::BaseEntity
{
public:
  Projectile(FW::Application& app, FW::EntityManager& entities);
  void setProjectile(const std::string& texture, const float speed, const int angle);
  void update(const float delta);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  sf::Sprite sprite;
  sf::FloatRect rect;
  sf::Vector2f velocity;
  int speed = 0;
};