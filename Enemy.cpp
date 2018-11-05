#include "Enemy.h"
#include "Application.h"

Enemy::Enemy(FW::Application& app, FW::EntityManager& entities)
  : FW::EntityManager::BaseEntity(app, entities),
  run(app.getResourceManager().getTexture("Enemy.png"), sf::Vector2u(4, 1), 0.2)
{
  setOrigin(sf::Vector2f(run.getFrameSize()) / 2.0f);
  setDrawOrder(1);

  // Set the size of the pathfinding node.
  node.width = 10;
  node.height = 10;

  rect.width = run.getFrameSize().x;
  rect.height = run.getFrameSize().y;
}

void Enemy::update(const float delta)
{
  // Get the player and tilemap entities.
  std::shared_ptr<Player> player = entities.getEntities<Player>()[0];
  std::shared_ptr<TileMap> tilemap = entities.getEntities<TileMap>()[0];

  if (!reverse && FW::Tools::Math::distance(getPosition(), player->getPosition()) < 75)
  {
    player->damage(25);
    reverse = true;
  }

  // If there is no path, or it is time to update the path again.
  ai_timer += delta;
  if (ai_timer >= ai_time || current_path.empty())
  {
    // Reset the timer.
    ai_timer = 0.0f;

    // Get the new path to the player.
    current_path = tilemap->getPathToTile(sf::Vector2i(getPosition()) / tilemap->getTileSize());

    // Remove the first element of the path, because its the tile the enemy is already in.
    current_path.pop_back();

    // Set the pathfinding node's position to the next element in the path and center it.
    node.left = (current_path.back().x * tilemap->getTileSize()) + (tilemap->getTileSize() / 2) - (node.width / 2);
    node.top = (current_path.back().y * tilemap->getTileSize()) + (tilemap->getTileSize() / 2) - (node.height / 2);
  }

  // If the enemy has reached the node.
  if (node.contains(getPosition()))
  {
    // Remove that node from the path.
    current_path.pop_back();

    // If there is more path remaining.
    if (!current_path.empty())
    {
      // Set the node to the next element in the path.
      node.left = (current_path.back().x * tilemap->getTileSize()) + (tilemap->getTileSize() / 2) - (node.width / 2);
      node.top = (current_path.back().y * tilemap->getTileSize()) + (tilemap->getTileSize() / 2) - (node.height / 2);
    }
  }

  // If no path, stop for now.
  if (current_path.empty())
  {
    velocity.x = 0.0;
    velocity.y = 0.0;
  }
  else
  {
    // Get the angle between the enemy's position and the path node.
    int angle = FW::Tools::Math::angleBetweenPoints(getPosition(), sf::Vector2f(node.left, node.top));

    // Set the velocity of the enemy towards the node.
    velocity.x = static_cast<float>(std::cos(FW::Tools::Math::deg2rad(angle)));
    velocity.y = static_cast<float>(std::sin(FW::Tools::Math::deg2rad(angle)));
    velocity.x *= speed;
    velocity.y *= speed;
  }

  if (reverse)
  {
    reverse_timer += delta;
    if (reverse_timer >= reverse_time)
    {
      reverse_timer = 0.0f;
      reverse = false;
    }
    velocity = -velocity;
  }

  // Just flip the enemy depending on what way it's facing.
  if (velocity.x > 0)
  {
    setScale(1, 1);
  }
  else
  {
    setScale(-1, 1);
  }

  // Get the solid tiles from the tilemap.
  std::vector<sf::FloatRect> solids = tilemap->getSurroundingSolids(getPosition());
  solids.push_back(player->getRect());

  // add the rects of every enemy except this enemy
  // we wouldn't want the enemy to collide with itself.
  for (std::shared_ptr<Enemy> enemy : entities.getEntities<Enemy>())
  {
    if (enemy->getRect() != rect)
      solids.push_back(enemy->getRect());
  }

  // Set the rect for collision detection.
  rect.left = getPosition().x - getOrigin().x;
  rect.top = getPosition().y - getOrigin().y;

  // Collide against solids on the X axis.
  rect.left += velocity.x * delta;
  for (sf::FloatRect& solid : solids)
  {
    if (rect.intersects(solid))
    {
      if (velocity.x > 0.0f)
        rect.left = solid.left - rect.width;
      else if (velocity.x < 0.0f)
        rect.left = solid.left + solid.width;
    }
  }

  // Collide against solids on the Y axis.
  rect.top += velocity.y * delta;
  for (sf::FloatRect& solid : solids)
  {
    if (rect.intersects(solid))
    {
      if (velocity.y > 0.0f)
        rect.top = solid.top - rect.height;
      else if (velocity.y < 0.0f)
        rect.top = solid.top + solid.height;
    }
  }

  // Set the position of the enemy to the rect used for collision.
  setPosition(rect.left + getOrigin().x, rect.top + getOrigin().y);

  // Update the current animation.
  run.update(delta);
}

void Enemy::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(run, states);
}

void Enemy::damage(const int damage)
{
  health -= damage;
  if (health <= 0)
    setAlive(false);
}