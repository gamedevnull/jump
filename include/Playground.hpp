#pragma once
#include <SFML/Graphics.hpp>
#include "Platform.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Pickup.hpp"
using namespace sf;

class Playground
{
public:
    sf::Texture *texture;
    std::vector<Platform *> platforms;

    Playground(Texture *texture);

    void CheckPlayerCollisions(Player *player);
    void CheckEnemyCollisions(std::vector<Enemy *> bombs);
    void CheckPickupCollisions(Pickup *pickup);

    void Update();
    void Render(sf::RenderWindow &window);

    void MoveDown(float dy);

    void GeneratePlatforms();
    void generateRandomGameObjectsWithFixingOnGivenArea(int startX, int startY, int endX, int endY, int n, int offsetY);
    void generateMore(int num);

    bool isValidPosition(GameObject* newObj);
    int randomWithStep(int min, int max, int step);
    void fixPos(GameObject *newObj, GameObject *obj);
    GameObject *getClosestObjInRange(GameObject *newObj, int range);

    void removePlatform(Platform *block);
    ~Playground();
};