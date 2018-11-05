#pragma once

#include "EntityManager.h"
#include "Enemy.h"

class ZombieSpawner
  : public FW::EntityManager::BaseEntity
{
public:
  ZombieSpawner(FW::Application& app, FW::EntityManager& entities);
  void update(const float delta);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  float spawn_time = 10.0f, spawn_timer = 0.0f;
  sf::Sprite sprite;
  static sf::Sound zombie;
};