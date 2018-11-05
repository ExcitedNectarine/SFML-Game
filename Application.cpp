#include "Application.h"

namespace FW
{
    Application::Application(const std::string& filename)
        : scenes(*this),
        configuration(filename),
        resources("Resources/Textures", "Resources/Fonts", "Resources/Sounds", "Resources/Music")
    {
        // Get the configuration map.
        ConfigurationManager::ConfigurationMap& config = configuration.getConfigurationMap();
        int width = std::stoi(config["screen"]["width"]); // get the screen width
        int height = std::stoi(config["screen"]["height"]); // get the screen height
        int framerate = std::stoi(config["screen"]["framerate"]); // get the framerate

        window.create(sf::VideoMode(width, height), "SFML Game"); // create the window
        window.setFramerateLimit(framerate); // set the framerate limit
    }

    void Application::run()
    {
        while (window.isOpen())
        {
            // get the time since last frame
            delta = clock.restart().asSeconds();

            // event loop
            while (window.pollEvent(event))
            {
                // if the X button is pressed
                if (event.type == sf::Event::Closed)
                    window.close(); // close the window and end the game
                scenes.getCurrentScene().handleEvent(event); // pass the event to the current scene
            }

            // update the current scene, passing the delta time
            scenes.getCurrentScene().update(delta);
            scenes.update();

            // clear previous frame from screen
            window.clear();
            window.draw(scenes.getCurrentScene()); // draw current scene
            window.display(); // display frame on window
        }
    }

    sf::RenderWindow& Application::getWindow()
    {
        return window;
    }

    SceneManager& Application::getSceneManager()
    {
        return scenes;
    }

    ConfigurationManager& Application::getConfiguration()
    {
        return configuration;
    }

    ResourceManager& Application::getResourceManager()
    {
        return resources;
    }
}