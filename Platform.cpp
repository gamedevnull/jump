
#include "include/Platform.hpp"
#include "include/globals.hpp"

Platform::Platform(int x, int y, sf::Texture *texture, int sizeId, int colorId) : texture(texture), sizeId(sizeId), colorId(colorId)
{
    int width = 60;
    if (sizeId == 2)
    {
        width = 100;
    }
    GameObject::Init(x, y, width, 20);
    objectType = PlatformId;
}

void Platform::Update()
{
    GameObject::Update();
}

void Platform::Render(sf::RenderWindow &window)
{
    // GameObject::Render(window);
    sf::Sprite sprite;
    sprite.setTexture(*texture);

    int spriteX = posX;

    // top

    sprite.setTextureRect(sf::IntRect(0, 475, 20, 16));
    sprite.setPosition(spriteX, posY);
    window.draw(sprite);

    spriteX += 20;
    sprite.setTextureRect(sf::IntRect(3, 475, 20, 16));
    sprite.setPosition(spriteX, posY);
    window.draw(sprite);

    if (sizeId == 2)
    {

        spriteX += 20;
        sprite.setTextureRect(sf::IntRect(3, 475, 20, 16));
        sprite.setPosition(spriteX, posY);
        window.draw(sprite);

        spriteX += 20;
        sprite.setTextureRect(sf::IntRect(3, 475, 20, 16));
        sprite.setPosition(spriteX, posY);
        window.draw(sprite);
    }

    spriteX += 20;
    sprite.setTextureRect(sf::IntRect(12, 475, 20, 16));
    sprite.setPosition(spriteX, posY);
    window.draw(sprite);

    // bottom

    spriteX = posX;
    int spriteY = posY + 16;

    sprite.setTextureRect(sf::IntRect(0, 525, 20, 16));
    sprite.setPosition(spriteX, spriteY);
    window.draw(sprite);

    spriteX += 20;
    sprite.setTextureRect(sf::IntRect(3, 525, 20, 16));
    sprite.setPosition(spriteX, spriteY);
    window.draw(sprite);

    if (sizeId == 2)
    {

        spriteX += 20;
        sprite.setTextureRect(sf::IntRect(3, 525, 20, 16));
        sprite.setPosition(spriteX, spriteY);
        window.draw(sprite);

        spriteX += 20;
        sprite.setTextureRect(sf::IntRect(3, 525, 20, 16));
        sprite.setPosition(spriteX, spriteY);
        window.draw(sprite);
    }

    spriteX += 20;
    sprite.setTextureRect(sf::IntRect(12, 525, 20, 16));
    sprite.setPosition(spriteX, spriteY);
    window.draw(sprite);
}