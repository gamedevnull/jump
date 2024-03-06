
#include "include/Cloud.hpp"

Cloud::Cloud(int x, int y, sf::Texture *texture) : texture(texture)
{
    GameObject::Init(x, y, 280, 170);
    spritePosX = 0;
    spritePosY = 0;
    spriteWidth = 272;
    spriteHeight = 155;
}

void Cloud::Update()
{
    GameObject::Update();
    if (posX < -200) {
        posX = 700;
    }
}

void Cloud::Render(sf::RenderWindow &window)
{
    sf::Sprite sprite;

    sprite.setTexture(*texture);
    sprite.setTextureRect(sf::IntRect(spritePosX, spritePosY, spriteWidth, spriteHeight));
    sprite.setPosition(posX, posY);

    window.draw(sprite);
}