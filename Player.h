#pragma once

#include "EntityManager.h"
#include "AnimatedSprite.h"
#include "TileMap.h"
#include "Projectile.h"
#include "Enemy.h"
#include <iostream>
#include <algorithm>

class Player
  : public FW::EntityManager::BaseEntity
{
public:
  Player(FW::Application& app, FW::EntityManager& entities);
  void update(const float delta);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  void damage(const int damage);
  sf::FloatRect getRect() { return rect; }

private:
  AnimatedSprite idle; // idle animation
  AnimatedSprite shooting; // shooting animation
  AnimatedSprite horizontal_run; // horizontal running animation
  AnimatedSprite vertical_run; // vertical running down animation
  AnimatedSprite vertical_back_run; // vertical running up animation
  AnimatedSprite* current_sprite; // current animation to dispay
  sf::Sprite arm;
  sf::FloatRect rect;
  sf::View view; // view kept centered on player
  sf::Vector2f velocity; // velocity of the player
  const int SPEED = 150; // movement speed
  bool shoot_mode; // wether the player is shooting or not
  bool shot = false; // if the player has shot
  int health = 100; // the player's health
  sf::Text health_text; // health hud display
  sf::Text game_time_text;
  sf::Sound shoot_sound;
  float game_time = 180.0;
};