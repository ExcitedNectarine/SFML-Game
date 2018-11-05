#pragma once

#include <queue>
#include <deque>
#include <tinyxml2.h>
#include "EntityManager.h"
#include "ResourceManager.h"

class TileMap : public FW::EntityManager::BaseEntity
{
public:
    TileMap(FW::Application& app, FW::EntityManager& entities);
    void load(const std::string& filename); // loads in a new map
    std::vector<sf::FloatRect> getObjects(const int object); // gets all objects of a certain type in the map
    std::vector<sf::FloatRect> getSurroundingSolids(const sf::Vector2f& position); // gets all solids surrounding a position, faster than checking all solids
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    // pathfinding methods
    void breadthFirstSearch(const sf::Vector2i& start_tile);
    std::deque<sf::Vector2i> getPathToTile(const sf::Vector2i& end_tile);
    void checkTile(std::queue<sf::Vector2i>& frontier, const int x, const int y, const sf::Vector2i& previous);

    int getTileSize() { return tile_size; }

private:
    // methods used internally
    void getTileMapData(tinyxml2::XMLElement* about);
    std::vector<std::string> formatData(const std::string& data);

private:
    int tile_size;
    int width;
    int height;
    int layers;
    std::string title;
    std::string tileset;
    std::string solids;
    std::vector<std::vector<int>> objects;
    std::vector<std::vector<int>> solid_grid;
    sf::Texture* texture;
    sf::VertexArray vertices; // used for drawing the map in one draw call
    
    // variables used for pathfinding
    sf::Vector2i start_tile;
    std::map<std::pair<int, int>, std::pair<int, int>> came_from; // stores a tile, and then the tile that comes after it on the way to a goal
};