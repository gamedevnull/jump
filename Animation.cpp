#include "include/Animation.hpp"
#include <SFML/Graphics.hpp>
using namespace sf;

Animation::Animation(Texture *texture, int frameWidth, int frameHeight, int framesCount, int firstFrameX, int firstFrameY, float animationDelay) : texture(texture), frameWidth(frameWidth), frameHeight(frameHeight), framesCount(framesCount), firstFrameX(firstFrameX), firstFrameY(firstFrameY), delay(animationDelay)
{
    sprite.setTexture(*texture);
    frameSelection = IntRect(firstFrameX, firstFrameY, frameWidth, frameHeight);
    sprite.setTextureRect(frameSelection);
    timer = 0;
    delay = animationDelay;
}

void Animation::Update()
{
    timer++;
    if (timer > delay)
    {
        if (frameSelection.left == firstFrameX + (frameWidth * (framesCount - 1)))
        {
            frameSelection.left = firstFrameX;
        }
        else
        {
            frameSelection.left += frameWidth;
        }
        sprite.setTextureRect(frameSelection);
        timer = 0;
    }
}

Sprite &Animation::getSprite()
{
    return sprite;
}