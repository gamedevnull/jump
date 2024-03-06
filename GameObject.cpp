#include "include/GameObject.hpp"

GameObject::GameObject()
{
    dirY = 1;
    dirX = 1;
    velX = 0;
    velY = 0;
}

void GameObject::Init(int x, int y, int width, int height)
{
    posX = x;
    posY = y;
    sizeX = width;
    sizeY = height;
}

void GameObject::Update()
{
    posX += velX * dirX;
    posY += velY * dirY;
}

void GameObject::Render(sf::RenderWindow &window)
{
    sf::RectangleShape tile;
    tile.setSize(sf::Vector2f(sizeX, sizeY));
    tile.setFillColor(sf::Color(0, 0, 255));
    tile.setPosition(posX, posY);
    window.draw(tile);
}

float GameObject::distanceBetweenObjects(GameObject *otherObject)
{
    float dx = static_cast<float>(posX - otherObject->posX);
    float dy = static_cast<float>(posY - otherObject->posY);
    return sqrt(dx * dx + dy * dy);
}

bool GameObject::Collided(GameObject *otherObject)
{
    if (posX < otherObject->posX + otherObject->sizeX &&
        posX + sizeX > otherObject->posX &&
        posY < otherObject->posY + otherObject->sizeY &&
        posY + sizeY > otherObject->posY)
    {
        return true;
    }
    return false;
}