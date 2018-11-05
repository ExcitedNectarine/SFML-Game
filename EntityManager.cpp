#include "EntityManager.h"

namespace FW
{
    EntityManager::EntityManager(Application& app)
        : app(app)
    {
    }

    void EntityManager::handleEvent(const sf::Event& event)
    {
        // pass the latest event to each entity
        for (auto& entity : entities)
            entity->handleEvent(event);
    }

    void EntityManager::update(const float delta)
    {
        for (std::shared_ptr<BaseEntity>& entity : entities)
            entity->update(delta);

        // remove all entities that died this frame.
        entities.remove_if(
            [](const std::shared_ptr<BaseEntity>& entity) { return !entity->getAlive(); }
        );
    }

    void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // sort all entities so that they are drawn in the correct order.
        entities.sort(
            [](const std::shared_ptr<BaseEntity>& e1, const std::shared_ptr<BaseEntity>& e2) { return e1->getDrawOrder() < e2->getDrawOrder(); }
        );

        // draw each entity.
        for (auto& entity : entities)
            target.draw(*entity);
    }
}