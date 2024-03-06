#pragma once
#include <SFML/Graphics.hpp>

class GameObject
{
public:
    float posX, posY, velX, velY, dirX, dirY;
    int sizeX, sizeY, objectType;

    GameObject();
    void Init(int x, int y, int width, int height);
    void Update();
    void Render(sf::RenderWindow &window);
    bool Collided(GameObject *otherObject); // AABB test
    float distanceBetweenObjects(GameObject* otherObject);
};