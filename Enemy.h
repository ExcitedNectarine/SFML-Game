#pragma once

#include <deque>
#include "EntityManager.h"
#include "AnimatedSprite.h"
#include "TileMap.h"
#include "Player.h"
#include "Tools.h"

class Enemy
    : public FW::EntityManager::BaseEntity
{
public:
  Enemy(FW::Application& app, FW::EntityManager& entities);
  void update(const float delta);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  sf::FloatRect getRect() { return rect; }
  void damage(const int damage);

private:
  float ai_timer = 0.0f;
  float ai_time = 1.0f;
  std::deque<sf::Vector2i> current_path;
  sf::FloatRect node;
  float speed = 100.0f;
  sf::Vector2f velocity;
  sf::FloatRect rect;
  AnimatedSprite run;
  int health = 100;
  bool reverse = false;
  float reverse_time = 3.0f, reverse_timer = 0.0f;
};