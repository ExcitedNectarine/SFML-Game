#pragma once

#include <string>
#include <map>
#include <memory>
#include <experimental/filesystem>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ApplicationException.h"
#include "Output.h"
#include "Tools.h"

namespace FW
{
    class ResourceManager
    {
    public:
        ResourceManager(
            const std::string& texture_directory,
            const std::string& font_directory,
            const std::string& sound_directory,
            const std::string& music_directory
        );

        // takes a resource type (anything but sf::Music) and a directory for where those
        // resources are stored. loads in every file in that directory
        template <typename ResourceType>
        std::map<std::string, ResourceType> loadAllResource(const std::string& resource_directory)
        {
            namespace fs = std::experimental::filesystem; // shorten namespace

            std::map<std::string, ResourceType> resources; // create map for resource
            if (fs::exists(resource_directory)) // if the resource directory exists
            {
                for (auto& path : fs::directory_iterator(resource_directory)) // get the name of every file in the directory
                {
                    std::string path_text = path.path().string();
                    std::string file = Tools::splitString(path_text, '\\').back();

                    OUTPUT("Loading file '" << file << "'.");

                    ResourceType resource; // create a new resource
                    if (!resource.loadFromFile(path_text)) // load the file
                        throw ApplicationException("Resource file '" + file + "' could not be loaded.");

                    resources[file] = resource; // copy it to the map
                }
            }

            return resources; // return the map of resource
        }

        // have to use a seperate function to load in music due to music using a different function name for loading
        void loadAllMusic(const std::string& music_directory);

        // a function for getting each kind of resource.
        sf::Texture& getTexture(const std::string& filename);
        sf::Font& getFont(const std::string& filename);
        sf::SoundBuffer& getSoundBuffer(const std::string& filename);
        sf::Music& getMusic(const std::string& filename);

    private:
        // a map for each kind of resource in the game
        std::map<std::string, sf::Texture> textures;
        std::map<std::string, sf::Font> fonts;
        std::map<std::string, sf::SoundBuffer> sound_buffers;
        std::map<std::string, std::unique_ptr<sf::Music>> music; // have to use pointers to music because they're non-copyable objects.
    };
}