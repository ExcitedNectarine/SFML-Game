#include "TileMap.h"
#include "Application.h"

TileMap::TileMap(FW::Application& app, FW::EntityManager& entities)
    : FW::EntityManager::BaseEntity(app, entities)
{
}

void TileMap::load(const std::string& filename)
{
    using namespace tinyxml2;

    // create a new XML document
    XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != XML_SUCCESS) // if file failed to load, throw error
        throw FW::ApplicationException("Level file '" + filename + "' could not be opened. " + doc.ErrorName());

    // get metadate about the tilemap, such as the width, height, and the name of the texture to use for tiles
    getTileMapData(doc.FirstChildElement("about"));

    // load the tileset
    texture = &app.getResourceManager().getTexture(tileset);

    vertices.setPrimitiveType(sf::Quads); // set the type of shape the vertex array should draw to quads (tiles are square)
    vertices.resize((width * height * 4) * layers); // set the size of the vertex array

    // parses which tiles are solid and which are not
    auto solid_tiles_split = FW::Tools::splitString(solids, '-');
    std::vector<int> solid_tiles;
    for (auto tile : solid_tiles_split)
        solid_tiles.push_back(std::stoi(tile));

    // set the size of the grid used for checking solids against positions
    solid_grid.resize(height);
    for (auto& row : solid_grid)
        row.resize(width);

    XMLElement* data = doc.FirstChildElement("data");
    XMLElement* layer_element = data->FirstChildElement("layers");

    int layer_counter = 0;
    for (XMLElement* layer = layer_element->FirstChildElement("layer"); layer; layer = layer->NextSiblingElement("layer")) // for each layer of tiles
    {
        // Get the layer and format it for easier parsing.
        std::string layer_text = layer->GetText();
        std::vector<std::string> layer_rows = formatData(layer_text);

        // Convert the layer data to integers.
        std::vector<int> layer_data;
        for (std::string& row : layer_rows)
        {
            std::vector<std::string> numbers = FW::Tools::splitString(row, '-');
            for (std::string& number : numbers)
                layer_data.push_back(std::stoi(number));
        }

        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                int tile_number = layer_data[x + y * width];
                int tile_x = tile_number % (texture->getSize().x / tile_size);
                int tile_y = tile_number / (texture->getSize().x / tile_size);

                // Get the tile's quad.
                sf::Vertex* quad = &vertices[((width * height * 4) * layer_counter) + ((x + y * width) * 4)];

                // Set the position of the quad's vertices.
                quad[0].position = sf::Vector2f(static_cast<float>(x * tile_size), static_cast<float>(y * tile_size));
                quad[1].position = sf::Vector2f(static_cast<float>((x + 1) * tile_size), static_cast<float>(y * tile_size));
                quad[2].position = sf::Vector2f(static_cast<float>((x + 1) * tile_size), static_cast<float>((y + 1) * tile_size));
                quad[3].position = sf::Vector2f(static_cast<float>(x * tile_size), static_cast<float>((y + 1) * tile_size));

                // Set the position of the quad's texture coordinates.
                quad[0].texCoords = sf::Vector2f(static_cast<float>(tile_x * tile_size), static_cast<float>(tile_y * tile_size));
                quad[1].texCoords = sf::Vector2f(static_cast<float>((tile_x + 1) * tile_size), static_cast<float>(tile_y * tile_size));
                quad[2].texCoords = sf::Vector2f(static_cast<float>((tile_x + 1) * tile_size), static_cast<float>((tile_y + 1) * tile_size));
                quad[3].texCoords = sf::Vector2f(static_cast<float>(tile_x * tile_size), static_cast<float>((tile_y + 1) * tile_size));

                // Check if the tile is solid.
                if (std::find(solid_tiles.begin(), solid_tiles.end(), tile_number) != solid_tiles.end() && solid_grid[y][x] != 1)
                    solid_grid[y][x] = 1;
                else
                    solid_grid[y][x] = 0;
            }
        }
        layer_counter++;
    }

    // Parse the object data.
    XMLElement* objects_element = data->FirstChildElement("objects");
    std::string object_text = objects_element->GetText();
    std::vector<std::string> object_rows = formatData(object_text);
    for (std::string& row : object_rows)
    {
        std::vector<int> object_row;
        std::vector<std::string> numbers = FW::Tools::splitString(row, '-');
        for (std::string& number : numbers)
            object_row.push_back(std::stoi(number));
        objects.push_back(object_row);
    }
}

std::vector<sf::FloatRect> TileMap::getObjects(const int object)
{
    // create a vector of rects for where objects should be
    std::vector<sf::FloatRect> object_rects;
    for (int y = 0; y < height; y++) //  for each object tile in the map
        for (int x = 0; x < width; x++)
            if (objects[y][x] == object) // if the object type matches the object we want
                object_rects.emplace_back( // create a new rect based on where the object is, adding it to the vector
                    static_cast<float>(x * tile_size),
                    static_cast<float>(y * tile_size),
                    static_cast<float>(tile_size),
                    static_cast<float>(tile_size)
                );
    return object_rects; // return the object rects
}

std::vector<sf::FloatRect> TileMap::getSurroundingSolids(const sf::Vector2f& pos)
{
    sf::Vector2i position = sf::Vector2i(pos);
    position = position / tile_size; // put position into tile coords

    std::vector<sf::FloatRect> rects;
    sf::FloatRect solid_rect = { 0, 0, static_cast<float>(tile_size), static_cast<float>(tile_size) }; // temp rect

    // get tilemap position of surrounding solids
    sf::Vector2i tiles[8] = {
        { position.x - 1, position.y },
        { position.x + 1, position.y },
        { position.x, position.y - 1 },
        { position.x, position.y + 1 },
        { position.x - 1, position.y - 1 },
        { position.x + 1, position.y + 1 },
        { position.x - 1, position.y + 1 },
        { position.x + 1, position.y - 1 }
    };

    for (const sf::Vector2i& tile : tiles) //  for each position
    {
        if ((tile.x > -1 && tile.x < width) && // if the position is in the map
            (tile.y > -1 && tile.y < height))
        {
            if (solid_grid[tile.y][tile.x] != 0) // if the position's tile is a solid
            {
                solid_rect.left = tile.x * tile_size; // set the temp rect's position to the solid's position
                solid_rect.top = tile.y * tile_size;
                rects.push_back(solid_rect); // add it to the vector of rects
            }
        }
    }

    return rects; // return rects
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = texture;
    target.draw(vertices, states);
}

void TileMap::getTileMapData(tinyxml2::XMLElement* about)
{
    title = about->FirstChildElement("title")->GetText();
    tileset = about->FirstChildElement("tileset")->GetText(); // texture used for drawing tilemap
    solids = about->FirstChildElement("solids")->GetText(); // tiles that are solid
    tile_size = std::stoi(about->FirstChildElement("tilesize")->GetText());
    width = std::stoi(about->FirstChildElement("width")->GetText());
    height = std::stoi(about->FirstChildElement("height")->GetText());
    layers = std::stoi(about->FirstChildElement("layers")->GetText());
}

std::vector<std::string> TileMap::formatData(const std::string& data)
{
    // code here basically removes all useless text from each layer and puts it into a vector
    std::string data_copy = data;
    data_copy.erase(std::remove(data_copy.begin(), data_copy.end(), ' '), data_copy.end()); // erase all spaces
    std::vector<std::string> data_rows = FW::Tools::splitString(data_copy, '\n');
    data_rows.erase(data_rows.begin());
    return data_rows;
}

void TileMap::breadthFirstSearch(const sf::Vector2i& start_tile)
{
    // store the start tile. (this would be the player's position)
    this->start_tile = start_tile;
    std::queue<sf::Vector2i> frontier; // create a queue which stores the frontier of the search
    frontier.push(start_tile); // start with the start tile

    // clear the map used for storing paths, as it's about to be recreated
    if (!came_from.empty())
        came_from.clear();

    // set the start tile's next tile to itself
    came_from[std::make_pair(start_tile.x, start_tile.y)] = std::make_pair(start_tile.x, start_tile.y);

    sf::Vector2i current;
    while (!frontier.empty()) // while the frontier
    {
        current = frontier.front();

        // check tiles surrounding the current tile
        checkTile(frontier, current.x + 1, current.y, current);
        checkTile(frontier, current.x - 1, current.y, current);
        checkTile(frontier, current.x, current.y + 1, current);
        checkTile(frontier, current.x, current.y - 1, current);

        // check diagonal tiles surrounding the current tile
        checkTile(frontier, current.x + 1, current.y - 1, current);
        checkTile(frontier, current.x + 1, current.y + 1, current);
        checkTile(frontier, current.x - 1, current.y - 1, current);
        checkTile(frontier, current.x - 1, current.y + 1, current);

        frontier.pop();
    }
}

std::deque<sf::Vector2i> TileMap::getPathToTile(const sf::Vector2i& end_tile)
{
    std::deque<sf::Vector2i> path; // a deque that stores the path from the start tile to an end tile
    if (!solid_grid[end_tile.y][end_tile.x]) // if the end tile isn't solid, then we can go ahead and construct the path
    {
        std::pair<int, int> current_tile(end_tile.x, end_tile.y); // start with the end tile
        path.push_front({ current_tile.first, current_tile.second }); // put it in the deque (converting it to a vector)

        while (came_from[current_tile] != std::make_pair((int)start_tile.x, (int)start_tile.y)) // while we haven't reached the start tile
        {
            current_tile = came_from[current_tile]; // set the current tile to it's next tile
            path.push_front({ current_tile.first, current_tile.second }); // push the current tile to the path
        }

        path.push_front(start_tile); // finish the path by adding the start tile
    }
    return path;
}

void TileMap::checkTile(std::queue<sf::Vector2i>& frontier, const int x, const int y, const sf::Vector2i& previous)
{
    if (x >= 0 && y >= 0 && x <= solid_grid[0].size() && y <= solid_grid.size()) // check if the tile is in bounds of the map
    {
        if (!solid_grid[y][x]) // if the tile is not solid
        {
            if (came_from.find(std::make_pair(x, y)) == came_from.end()) // if the tile has not been checked yet.
            {
                frontier.push({ x, y }); // put it into the frontier, so it is checked soon
                came_from[std::make_pair(x, y)] = std::make_pair(previous.x, previous.y); // and add it to the came_from map
            }
        }
    }
}