#pragma once

#include <SFML/Graphics.hpp>

class AnimatedSprite : public sf::Sprite
{
public:
    AnimatedSprite(const sf::Texture& texture, const sf::Vector2u& frames, const float time_between_frames);
    AnimatedSprite();
    void update(const float delta);
    void setFrames(const sf::Vector2u& frames);
    sf::Vector2u getFrames();
    void setTimeBetweenFrames(const float time_between_frames);
    float getTimeBetweenFrames();
    void setPlaying(const bool playing);
    bool getPlaying();
    sf::Vector2u getFrameSize();

private:
    void calculateFrames();

private:
    sf::Vector2u frames;
    sf::Vector2u frame_size;
    sf::Vector2u frame;
    float time_between_frames = 0.0f;
    float timer = 0.0f;
    bool playing = true;
};