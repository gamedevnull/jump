#include "include/Pickup.hpp"
#include "include/Animation.hpp"
#include "include/globals.hpp"

Pickup::Pickup(Texture *texture) : texture(texture)
{
    GameObject::Init(100, 100, 24, 24);
    state = Falling;
    velY = 0;
    objectType = PlayerId;
}

void Pickup::Update()
{
    if (state == Falling)
    {
        velY += 0.1;
    }

    GameObject::Update();

    if (posY > SCREEN_HEIGHT)
    {
        velY = 0;
        posX = rand() % 530;
        posY = -300;
        state = Falling;
    }
}

void Pickup::Render(sf::RenderWindow &window)
{
    // GameObject::Render(window);
    // sf::RectangleShape tile;
    // tile.setSize(sf::Vector2f(sizeX, sizeY));
    // tile.setFillColor(sf::Color(255, 0, 255));
    // tile.setPosition(posX, posY);
    // window.draw(tile);

    sf::Sprite sprite;
    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(48, 637, 16, 16));
    sprite.setPosition(posX + 2, posY + 2);

    sprite.setScale(1.5f, 1.5f);
    window.draw(sprite);
}

Pickup::~Pickup()
{
}