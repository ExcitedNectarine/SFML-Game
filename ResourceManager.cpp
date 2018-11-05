#include "ResourceManager.h"

namespace FW
{
    ResourceManager::ResourceManager(
        const std::string& texture_directory,
        const std::string& font_directory,
        const std::string& sound_directory,
        const std::string& music_directory
    )
    {
        // load in all textures, fonts, sounds and music
        textures = loadAllResource<sf::Texture>(texture_directory);
        fonts = loadAllResource<sf::Font>(font_directory);
        sound_buffers = loadAllResource<sf::SoundBuffer>(sound_directory);
        loadAllMusic(music_directory);
    }

    // almost identical to template function in header, but has to be different because music
    // is "opened" instead of loaded
    void ResourceManager::loadAllMusic(const std::string& music_directory)
    {
        namespace fs = std::experimental::filesystem;

        if (fs::exists(music_directory))
        {
            for (auto& path : fs::directory_iterator(music_directory))
            {
                std::string path_text = path.path().string();
                std::string filename = Tools::splitString(path_text, '\\').back();

                OUTPUT("Loading file '" << filename << "'.");

                std::unique_ptr<sf::Music> track = std::make_unique<sf::Music>(); // create a new pointer to music object
                if (!track->openFromFile(path_text))
                    throw ApplicationException("Resource file '" + filename + "' could not be loaded.");

                music[filename] = std::move(track); // move pointer to the map
            }
        }
    }

    sf::Texture& ResourceManager::getTexture(const std::string& filename)
    {
        auto it = textures.find(filename); // check if texture exists
        if (it == textures.end()) // if it doesn't, throw error
            throw ApplicationException("Texture '" + filename + "' could not be found.");
        return it->second; // return texture
    }

    sf::Font& ResourceManager::getFont(const std::string& filename)
    {
        auto it = fonts.find(filename); // check if font exists
        if (it == fonts.end()) // if it doesnt, throw error
            throw ApplicationException("Font '" + filename + "' could not be found.");
        return it->second; // return font
    }

    sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& filename)
    {
        auto it = sound_buffers.find(filename); // check if sound buffer exists
        if (it == sound_buffers.end()) // if it doesn't, throw error
            throw ApplicationException("Sound buffer '" + filename + "' could not be found.");
        return it->second; // return sound buffer
    }

    sf::Music& ResourceManager::getMusic(const std::string& filename)
    {
        auto it = music.find(filename); // check if track exists
        if (it == music.end()) // if it doesn't, throw error
            throw ApplicationException("Track '" + filename + "' could not be found.");
        return *it->second; // return track from pointer
    }
}