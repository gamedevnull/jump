#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Animation
{
public:
    Texture *texture;
    Sprite sprite;
    IntRect frameSelection;
    int frameWidth, frameHeight, framesCount, firstFrameX, firstFrameY;
    float timer, delay;
    bool repeat, rewind;

    Animation(Texture *texture, int frameWidth, int frameHeight, int framesCount, int firstFrameX, int firstFrameY, float animationDelay);
    void Update();
    Sprite &getSprite();
};