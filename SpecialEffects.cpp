#include "include/globals.hpp"
#include "include/SpecialEffects.hpp"

SpecialEffects::SpecialEffects()
{
}

void SpecialEffects::Update()
{
    for(std::vector<FxBlock *>::iterator it = sparks.begin(); it != sparks.end(); ++it)
    {
        (*it)->Update(0.01);
    }

    for(std::vector<FxBlock *>::iterator it = sparks.begin(); it != sparks.end(); ++it)
    {
        if ((*it)->y > SCREEN_HEIGHT)
        {
            delete *it;
            sparks.erase(it);
            break;
        }
    }
}

void SpecialEffects::Render(sf::RenderWindow &window)
{
    sf::RectangleShape tile;
    tile.setSize(sf::Vector2f(6, 6));

    for(std::vector<FxBlock *>::iterator it = sparks.begin(); it != sparks.end(); ++it)
    {
        if (rand() % 2)
        {
            tile.setFillColor(Color(35, 181, 69));
        }
        else
        {
            tile.setFillColor(Color(38, 38, 38));
        }
        tile.setPosition((*it)->x, (*it)->y);
        window.draw(tile);
    }
}

void SpecialEffects::SpawnEffect(int x, int y, int number)
{
    if (sparks.size() <= 30)
    {
        for (int i = 0; i < number; i++)
        {
            FxBlock *newBlock = new FxBlock(x, y);
            sparks.push_back(newBlock);
        }
    }
}

SpecialEffects::~SpecialEffects()
{
}