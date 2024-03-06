#pragma once
#include "GameObject.hpp"

class Platform : public GameObject
{
public:
    sf::Texture *texture;
    int sizeId, colorId;

    Platform(int x, int y, sf::Texture *texture, int sizeId, int colorId);
    void Update();
    void Render(sf::RenderWindow &window);
};