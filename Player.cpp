#include "Player.h"
#include "Application.h"

Player::Player(FW::Application& app, FW::EntityManager& entities)
    : FW::EntityManager::BaseEntity(app, entities),
    idle(app.getResourceManager().getTexture("Idle.png"), sf::Vector2u(2, 1), 0.4f),
    shooting(app.getResourceManager().getTexture("Shooting.png"), sf::Vector2u(1, 1), 0.0f),
    horizontal_run(app.getResourceManager().getTexture("HorizontalRun.png"), sf::Vector2u(4, 1), 0.2f),
    vertical_run(app.getResourceManager().getTexture("VerticalRun.png"), sf::Vector2u(4, 1), 0.2f),
    vertical_back_run(app.getResourceManager().getTexture("VerticalBackRun.png"), sf::Vector2u(4, 1), 0.2f)
{
    // set the current sprite
    current_sprite = &horizontal_run;
    setOrigin(sf::Vector2f(current_sprite->getFrameSize()) / 2.0f);

    setDrawOrder(2);

    // setup the arm
    arm.setTexture(app.getResourceManager().getTexture("Arm.png"));
    arm.setOrigin(0.0f, arm.getTexture()->getSize().y / 2.0f);
    arm.setPosition(sf::Vector2f(current_sprite->getFrameSize()) / 2.0f);

    // setup the hud
    health_text.setFont(app.getResourceManager().getFont("GameFont.ttf"));
    health_text.setCharacterSize(20);
    health_text.setPosition(20, 20);
    health_text.setString("Health: " + std::to_string(health));

    // set the size of the view (pretty much a camera)
    view.setSize(1280, 720);

    game_time_text.setFont(app.getResourceManager().getFont("GameFont.ttf"));
    game_time_text.setCharacterSize(20);
    game_time_text.setPosition(20, 80);
    game_time_text.setString("Time Left: " + std::to_string(game_time));

    shoot_sound.setBuffer(app.getResourceManager().getSoundBuffer("Handgun.wav"));
}

void Player::update(const float delta)
{
  // if the player is holding the right mouse button
  shoot_mode = sf::Mouse::isButtonPressed(sf::Mouse::Right);
  if (shoot_mode)
  {
      // set the sprite to the shooting sprite
      current_sprite = &shooting;

      // reset player scale and velocity
      setScale(1, 1);
      velocity.x = 0;
      velocity.y = 0;

      // get mouse pos
      sf::Vector2f mouse_pos = app.getWindow().mapPixelToCoords(sf::Mouse::getPosition(app.getWindow()));
      float angle = FW::Tools::Math::angleBetweenPoints(getPosition(), mouse_pos); // get angle between position and mouse position
      arm.setRotation(angle); // turn arm towards mouse pos
      if (angle > 90 && angle < 270) // flip the arm and player depending on where the mouse position is
      {
          setScale(-1, 1);
          arm.setScale(1, -1);
      }
      else
      {
          setScale(1, 1);
          arm.setScale(1, 1);
      }
       
      // if left mouse button is pressed
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !shot)
      {
          shoot_sound.play();

          // position bullet at end of arm
          sf::Vector2f bullet_start = getPosition();
          bullet_start.x += std::cos(FW::Tools::Math::deg2rad(angle)) * arm.getTexture()->getSize().x;
          bullet_start.y += std::sin(FW::Tools::Math::deg2rad(angle)) * arm.getTexture()->getSize().x;

          // create the bullet and get a pointer to it
          auto proj = entities.addEntity<Projectile>();
          proj->setPosition(bullet_start);
          proj->setProjectile("Bullet.png", 250, angle);
          shot = true;
      }
      else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && shot)
          shot = false;

      arm.setPosition(getPosition());
  }
  else
  {
      // movement code for player movement along the X axis
      velocity.x = 0;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
          velocity.x -= SPEED;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
          velocity.x += SPEED;

      // movement code for player movement along the Y axis
      velocity.y = 0;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
          velocity.y -= SPEED;
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
          velocity.y += SPEED;

      // set sprite to idle if player is not moving
      if (!velocity.x && !velocity.y)
          current_sprite = &idle;
      else
      {
          // if player is moving along the Y axis
          if (velocity.y)
          {
              // if player is moving down
              if (velocity.y > 0)
                  current_sprite = &vertical_run; // do vertical movement animation with player facing camera
              else
                  current_sprite = &vertical_back_run; // do vertical movement animation for player not facing camera
          }
          if (velocity.x) // if the player is moving along x axis
          {
              current_sprite = &horizontal_run; // use animation for moving along x axis
              if (velocity.x > 0) // flip player depending on direction
                  setScale(1, 1);
              else
                  setScale(-1, 1);
          }
      }
  }

  // update current animation
  current_sprite->update(delta);

  // get all solids in map
  std::vector<sf::FloatRect> solids = entities.getEntities<TileMap>()[0]->getSurroundingSolids(getPosition());
  for (std::shared_ptr<Enemy> enemy : entities.getEntities<Enemy>()) // get all enemy rects
      solids.push_back(enemy->getRect());

  // set rect for collisions
  rect.left = getPosition().x - getOrigin().x;
  rect.top = getPosition().y - getOrigin().y;
  rect.width = current_sprite->getFrameSize().x;
  rect.height = current_sprite->getFrameSize().y;

  // move rect along the x axis
  rect.left += velocity.x * delta;
  for (sf::FloatRect& solid : solids) // for every solid
  {
      if (rect.intersects(solid)) // if the rect collides with the solid
      {
          if (velocity.x > 0.0f) // if player moving right
              rect.left = solid.left - rect.width; // set rect's right side to solid's left side
          else if (velocity.x < 0.0f) // if player moving left
              rect.left = solid.left + solid.width; // set rect's left side to solid's right side
      }
  }

  // move rect along the y axis
  rect.top += velocity.y * delta;
  for (sf::FloatRect& solid : solids) // for every solid
  {
      // if the rect collides with the solid
      if (rect.intersects(solid))
      {
          if (velocity.y > 0.0f) // if player moving down
              rect.top = solid.top - rect.height; // set rect's bottom to solid's top
          else if (velocity.y < 0.0f) // if player moving up
              rect.top = solid.top + solid.height; // set rect's top to solid's bottom
      }
  }

  // decrement game timer
  game_time -= delta;
  if (game_time <= 0.0f) // if time is up
  {
    // reset game view
    app.getWindow().setView(app.getWindow().getDefaultView());
    app.getSceneManager().getCurrentScene().setAlive(false); // kill the scene, returning to main menu
  }
  // change the on-screen timer text to new time
  game_time_text.setString("Time Left: " + std::to_string(int(game_time)));

  // set position of player to rect's new position
  setPosition(rect.left + getOrigin().x, rect.top + getOrigin().y);
  view.setCenter(getPosition()); // set view on the player
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // set transform to draw by to object's transform
    states.transform *= getTransform();
    target.draw(*current_sprite, states); // draw the current player sprite

    app.getWindow().setView(app.getWindow().getDefaultView()); // reset game view to default to draw hud
    target.draw(health_text); // draw health
    target.draw(game_time_text); // draw game text
    app.getWindow().setView(view); // set view back to the player

    if (shoot_mode) // if we're in shoot mode
    {
        target.draw(arm); // draw the player's arm
    }
}

void Player::damage(const int damage)
{
  health -= damage; // damage player's health
  health_text.setString("Health: " + std::to_string(health)); // update health on hud
  if (health <= 0) // if player is dead
  {
    app.getWindow().setView(app.getWindow().getDefaultView()); // reset the game view
    app.getSceneManager().getCurrentScene().setAlive(false); // kill the scene, return to main menu
  }
}