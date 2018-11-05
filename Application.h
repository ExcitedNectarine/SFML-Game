#pragma once

#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include "ConfigurationManager.h"
#include "ResourceManager.h"

namespace FW
{
    class Application
    {
    public:
        Application(const std::string& filename);
        void run();

        sf::RenderWindow& getWindow();

        SceneManager& getSceneManager();
        ConfigurationManager& getConfiguration();
        ResourceManager& getResourceManager();

    private:
        sf::RenderWindow window;
        sf::Event event;
        sf::Clock clock;
        float delta;

        SceneManager scenes;
        ConfigurationManager configuration;
        ResourceManager resources;
    };
}