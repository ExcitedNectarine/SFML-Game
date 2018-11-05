#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite(const sf::Texture& texture, const sf::Vector2u& frames, const float time_between_frames)
    : sf::Sprite(texture),
    frames(frames),
    time_between_frames(time_between_frames)
{
    calculateFrames();
}

AnimatedSprite::AnimatedSprite() {}

void AnimatedSprite::update(const float delta)
{
    if (playing)
    {
        timer += delta;
        if (timer >= time_between_frames)
        {
            timer = 0.0f;

            if (++frame.x > frames.x)
            {
                frame.x = 0;
                if (++frame.y > frames.y)
                    frame.y = 0;
            }

            setTextureRect(
                sf::IntRect(
                    frame.x * frame_size.x,
                    frame.y * frame_size.y,
                    frame_size.x,
                    frame_size.y
                )
            );
        }
    }
}

void AnimatedSprite::setFrames(const sf::Vector2u& frames)
{
    this->frames = frames;
    calculateFrames();
}

sf::Vector2u AnimatedSprite::getFrames()
{
    return frames;
}

void AnimatedSprite::setTimeBetweenFrames(const float time_between_frames)
{
    this->time_between_frames = time_between_frames;
}

float AnimatedSprite::getTimeBetweenFrames()
{
    return time_between_frames;
}

void AnimatedSprite::setPlaying(const bool playing)
{
    this->playing = playing;
}

bool AnimatedSprite::getPlaying()
{
    return playing;
}

sf::Vector2u AnimatedSprite::getFrameSize()
{
    return frame_size;
}

void AnimatedSprite::calculateFrames()
{
    frame_size = getTexture()->getSize();
    if (frames.x)
        frame_size.x /= frames.x;
    if (frames.y)
        frame_size.y /= frames.y;

    frames.x--;
    frames.y--;

    setTextureRect(
        sf::IntRect(
            frame.x * frame_size.x,
            frame.y * frame_size.y,
            frame_size.x,
            frame_size.y
        )
    );
}