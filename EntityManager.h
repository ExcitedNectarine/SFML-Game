#pragma once

#include <vector>
#include <list>
#include <memory>
#include <algorithm>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>

namespace FW
{
    class Application;

    class EntityManager
        : public sf::Drawable
    {
    public:
        // a base entity type
        // each entity in the game inherits from this
        class BaseEntity
            : public sf::Drawable,
            public sf::Transformable
        {
        public:
            BaseEntity(Application& app, EntityManager& entities) : app(app), entities(entities) {}
            virtual void handleEvent(const sf::Event& event) {}
            virtual void update(const float delta) {}
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {}

            void setAlive(const bool alive) { this->alive = alive; }
            bool getAlive() { return alive; }

            void setDrawOrder(const int draw_order) { this->draw_order = draw_order; }
            int getDrawOrder() { return draw_order; }

        protected:
            Application& app;
            EntityManager& entities;
            bool alive = true;
            int draw_order = 0;
        };

    public:
        EntityManager(Application& app);
        void handleEvent(const sf::Event& event);
        void update(const float delta);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        template <typename EntityType>
        std::shared_ptr<EntityType> addEntity()
        {
            // make sure the entity type inherits from base entity
            static_assert(std::is_base_of<BaseEntity, EntityType>::value, "'EntityType' must inherit 'BaseEntity'.");

            // add the entity to the list, passing the app and the entity manager to its constructor
            entities.push_back(std::make_shared<EntityType>(app, *this));

            // return a casted pointer
            return std::dynamic_pointer_cast<EntityType>(entities.back());
        }

        template <typename EntityType>
        std::vector<std::shared_ptr<EntityType>> getEntities()
        {
            // make sure the entity type inherits from base entity
            static_assert(std::is_base_of<BaseEntity, EntityType>::value, "'EntityType' must inherit 'BaseEntity'.");
            std::vector<std::shared_ptr<EntityType>> entity_pointers;
            for (std::shared_ptr<BaseEntity> entity : entities)
                if (typeid(*entity) == typeid(EntityType))
                    entity_pointers.push_back(std::dynamic_pointer_cast<EntityType>(entity));
            return entity_pointers;
        }

    private:
        Application & app;
        mutable std::list<std::shared_ptr<BaseEntity>> entities;
    };
}