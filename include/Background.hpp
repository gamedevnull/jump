#pragma once
#include <SFML/Graphics.hpp>
#include "Cloud.hpp"
using namespace sf;

class Background
{
public:
    std::vector<Cloud *> clouds;
    
    Background(sf::Texture *texture);
    void Update();
    void MoveUp(float dy);
    void Render(sf::RenderWindow &window);
    ~Background();
};