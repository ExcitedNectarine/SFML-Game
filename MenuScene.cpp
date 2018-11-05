#include "MenuScene.h"
#include "Application.h"

MenuScene::MenuScene(FW::Application& app)
  : FW::SceneManager::BaseScene(app)
{
  // start playing the music
  app.getResourceManager().getMusic("Chiptune.wav").play();
  app.getResourceManager().getMusic("Chiptune.wav").setVolume(25);

  // create the play and exit buttons;
  play = entities.addEntity<TextButton>();
  play->setup("GameFont.ttf", "Play", 20);
  play->setPosition(20, 20);
  quit = entities.addEntity<TextButton>();
  quit->setup("GameFont.ttf", "Quit", 20);
  quit->setPosition(20, 80);

  // setup the instructions
  instructions.setFont(app.getResourceManager().getFont("GameFont.ttf"));
  instructions.setCharacterSize(12);
  instructions.setPosition(150, 20);
  instructions.setString("Use WASD to move. Hold the right mouse button to aim, and press the left mouse button to shoot.");
}

void MenuScene::update(const float delta)
{
  // update all entities in scene
  entities.update(delta);

  // if the play button is pressed
  if (play->isPressed())
  {
    // add the game scene
    app.getSceneManager().addScene<GameScene>();
  }
  // if the quit button is pressed
  if (quit->isPressed())
  {
    // close the window and end the game
    app.getWindow().close();
  }
}

void MenuScene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(entities);
  target.draw(instructions);
}