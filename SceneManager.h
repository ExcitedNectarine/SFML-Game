#pragma once

#include <stack>
#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Window/Event.hpp>
#include "EntityManager.h"
#include "Output.h"

namespace FW
{
    class Application;

    class SceneManager
    {
    public:
        class BaseScene // base scene in which all other scenes inherit from
            : public sf::Drawable
        {
        public:
            BaseScene(Application& app) : app(app), entities(app) {}
            virtual void handleEvent(const sf::Event& event) { entities.handleEvent(event); } // default function hands event to entities
            virtual void update(const float delta) { entities.update(delta); } // default function updates entities
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(entities); } // default function draws entities
            void setAlive(const bool alive) { this->alive = alive; } // used to kill scene
            bool getAlive() { return alive; }

        protected:
            Application & app; // stores reference to main application object
            EntityManager entities; // and an entity manager
            bool alive = true;
        };

    public:
        SceneManager(Application& app);
        void popScene();
        BaseScene& getCurrentScene(); // gets current scene
        void update() {
          // remove all scenes that have died this frame
          scenes.erase(std::remove_if(scenes.begin(), scenes.end(),
            [](const std::unique_ptr<BaseScene>& scene) { return !scene->getAlive(); }
          ), scenes.end());
        }

        template <typename SceneType>
        void addScene()
        {
            // add a new scene to the vector, must inherit from BaseScene
            static_assert(std::is_base_of<BaseScene, SceneType>::value, "'SceneType' must inherit 'BaseScene'.");
            scenes.push_back(std::make_unique<SceneType>(app)); // uses a smart pointer to the scene
        }

    private:
        Application & app; // stores reference to main application object
        std::vector<std::unique_ptr<BaseScene>> scenes; // vector of scenes
    };
}