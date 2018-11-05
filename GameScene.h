#pragma once

#include "SceneManager.h"
#include "TileMap.h"
#include "Player.h"
#include "Enemy.h"
#include "ZombieSpawner.h"

class GameScene
  : public FW::SceneManager::BaseScene
{
public:
  GameScene(FW::Application& app);
  void update(const float delta);

private:
  float ai_timer = 0.0, ai_time = 0.5;
  float game_time = 180.0;
  std::shared_ptr<TileMap> tilemap;
  std::shared_ptr<Player> player;
};