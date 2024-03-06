#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class FxBlock
{
public:
    int x, y;
    float startVelocity, startAngle;
    float vx0, vy0, timer;

    FxBlock(int x, int y) : x(x), y(y)
    {
        timer = 0;
        startVelocity = rand() % 50 + 40;
        startAngle = rand() % 180 + 1;
        vx0 = startVelocity * cos(startAngle * (3.14159265 / 180));
        vy0 = startVelocity * sin(startAngle * (3.14159265 / 180));
    }

    void Update(float elapsedTime)
    {
        timer += elapsedTime;
        float g = 9.81;
        x += vx0 * timer;
        y -= vy0 * timer - 0.5 * g * timer * timer;
        vy0 -= g * timer;
    }

};

class SpecialEffects
{
public:
    std::vector<FxBlock *> sparks;

    SpecialEffects();
    void Update();
    void Render(sf::RenderWindow &window);
    void SpawnEffect(int x, int y, int number);
    ~SpecialEffects();
};