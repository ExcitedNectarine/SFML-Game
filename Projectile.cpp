#include "Projectile.h"
#include "Application.h"

Projectile::Projectile(FW::Application& app, FW::EntityManager& entities)
    : FW::EntityManager::BaseEntity(app, entities)
{
}

void Projectile::setProjectile(const std::string& texture, const float speed, const int angle)
{
  sprite.setTexture(app.getResourceManager().getTexture(texture));
  setOrigin(sf::Vector2f(sprite.getTexture()->getSize()) / 2.0f);
  rect.width = sprite.getTexture()->getSize().x;
  rect.height = sprite.getTexture()->getSize().y;

  // set the rotation and velocity of the projectile.
  setRotation(angle);
  velocity.x = std::cos(FW::Tools::Math::deg2rad(angle));
  velocity.y = std::sin(FW::Tools::Math::deg2rad(angle));
  velocity *= speed;
}

void Projectile::update(const float delta)
{
  rect.left = getPosition().x - getOrigin().x;
  rect.top = getPosition().y - getOrigin().y;

  // for each solid in the map
  std::vector<sf::FloatRect> solids = entities.getEntities<TileMap>()[0]->getSurroundingSolids(getPosition());
  for (auto& solid : solids)
  {
    // if the projectile collides with the solid
    if (rect.intersects(solid))
        setAlive(false); // destroy the projectile
  }

  // for each enemy in the game
  std::vector<std::shared_ptr<Enemy>> enemies = entities.getEntities<Enemy>();
  for (auto& enemy : enemies)
  {
    // if the projectile collides with the enemy
    if (rect.intersects(enemy->getRect()))
    {
      // damage the enemy
      enemy->damage(25);

      // destroy the projectile
      setAlive(false);
    }
  }

  // move the projectile
  move(velocity * delta);
}

void Projectile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(sprite, states);
}