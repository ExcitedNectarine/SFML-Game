#include "GameScene.h"
#include "Application.h"

GameScene::GameScene(FW::Application& app) : FW::SceneManager::BaseScene(app)
{
  tilemap = entities.addEntity<TileMap>();
  tilemap->load("Resources/TileMaps/Level.xml");

  for (auto rect : tilemap->getObjects(1))
  {
    entities.addEntity<ZombieSpawner>()->setPosition(rect.left, rect.top);
  }

  player = entities.addEntity<Player>();
  player->setPosition(tilemap->getObjects(2)[0].left, tilemap->getObjects(2)[0].top);
}

void GameScene::update(const float delta)
{
  // update all the entities in the scene.
  entities.update(delta);

  // if it is time to update the pathfinding grid
  ai_timer += delta;
  if (ai_timer >= ai_time)
  {
    // reset the timer
    ai_timer = 0.0;

    // calculate paths from each tile to the player tile.
    tilemap->breadthFirstSearch(sf::Vector2i(player->getPosition()) / tilemap->getTileSize());
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
  {
    app.getWindow().setView(app.getWindow().getDefaultView()); // reset the window view
    alive = false; // if the escape key is pressed, kill the scene, returning to the menu scene.
  }
}