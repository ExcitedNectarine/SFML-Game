#include "Application.h"
#include "MenuScene.h"

int main()
{
  try
  {
    // Create an application object.
    FW::Application app("Resources/Configuration.xml");

    // Add the menu scene to the scene manager.
    app.getSceneManager().addScene<MenuScene>();

    // Run the game.
    app.run();
  }
  // If there was an error, catch it and display it on the console.
  catch (const FW::ApplicationException& error)
  {
    OUTPUT_ERROR(error.what());
    std::cin.get();
  }

  return 0;
}