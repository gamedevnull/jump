#pragma once
#include "GameObject.hpp"

class Cloud : public GameObject
{
public:
    sf::Texture *texture;
    int spritePosX, spritePosY, spriteWidth, spriteHeight;
    
    Cloud(int x, int y, sf::Texture *texture);
    void Update();
    void Render(sf::RenderWindow &window);
};