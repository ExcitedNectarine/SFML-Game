#include "ZombieSpawner.h"
#include "Application.h"

sf::Sound ZombieSpawner::zombie; // must set static member variable

ZombieSpawner::ZombieSpawner(FW::Application& app, FW::EntityManager& entities)
  : FW::EntityManager::BaseEntity(app, entities)
{
  sprite.setTexture(app.getResourceManager().getTexture("Gravestone.png"));
  zombie.setBuffer(app.getResourceManager().getSoundBuffer("Zombie.wav"));
}

void ZombieSpawner::update(const float delta)
{
  spawn_timer += delta;
  if (spawn_timer >= spawn_time)
  {
    zombie.play(); // play zombie spawn sound
    spawn_timer = 0.0f;
    entities.addEntity<Enemy>()->setPosition(getPosition()); // spawn enemy and set it's position to the spawner
  }
}

void ZombieSpawner::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(sprite, states);
}