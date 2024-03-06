#include "include/Background.hpp"
#include "include/Cloud.hpp"

Background::Background(sf::Texture *texture)
{
    Cloud *cloud;

    cloud = new Cloud(100, -40, texture);
    cloud->dirX = -1;
    cloud->velX = 0.08;
    clouds.push_back(cloud);

    cloud = new Cloud(300, 150, texture);
    cloud->dirX = -1;
    cloud->velX = 0.10;
    cloud->spritePosY = 160;
    clouds.push_back(cloud);

    cloud = new Cloud(200, 320, texture);
    cloud->dirX = -1;
    cloud->velX = 0.14;
    clouds.push_back(cloud);

    cloud = new Cloud(400, 480, texture);
    cloud->dirX = -1;
    cloud->velX = 0.12;
    cloud->spritePosY = 320;
    clouds.push_back(cloud);

    cloud = new Cloud(150, 660, texture);
    cloud->dirX = -1;
    cloud->velX = 0.16;
    clouds.push_back(cloud);

    cloud = new Cloud(650, 330, texture);
    cloud->dirX = -1;
    cloud->velX = 0.16;
    cloud->spritePosY = 320;
    clouds.push_back(cloud);
}

void Background::Background::Update()
{
    for (Cloud *cloud : clouds)
    {
        cloud->Update();
    }
}

void Background::Background::MoveUp(float dy)
{
    for (Cloud *cloud : clouds)
    {
        cloud->posY = cloud->posY - (dy / 2);
        if (cloud->posY > 800)
        {
            cloud->posY = -200;
        }
    }
}

void Background::Render(sf::RenderWindow &window)
{
    for (Cloud *cloud : clouds)
    {
        cloud->Render(window);
    }
}

Background::~Background()
{
    for (Cloud *cloud : clouds)
    {
        delete cloud;
    }
    clouds.clear();
}