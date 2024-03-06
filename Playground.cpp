#include "include/Playground.hpp"
#include "include/Platform.hpp"
#include <iostream>
#include "include/globals.hpp"

Playground::Playground(Texture *texture) : texture(texture)
{
}

void Playground::CheckPlayerCollisions(Player *player)
{
    for (std::vector<Platform *>::iterator it = platforms.begin(); it != platforms.end(); ++it)
    {
        if (player->Collided(*it) and player->velY > 0)
        {
            player->velY = -10;
        }
    }
}

void Playground::CheckEnemyCollisions(std::vector<Enemy *> bombs)
{
    for (std::vector<Enemy *>::iterator enemy = bombs.begin(); enemy != bombs.end(); ++enemy)
    {
        for (std::vector<Platform *>::iterator platform = platforms.begin(); platform != platforms.end(); ++platform)
        {
            if ((*enemy)->state == Enemy::Falling and (*enemy)->Collided(*platform))
            {
                (*enemy)->velY = 0;
                (*enemy)->state = Enemy::Stopped;

                // fix bomb position to be in line with platform
                if ((*enemy)->posY + (*enemy)->sizeY > (*platform)->posY)
                {
                    (*enemy)->posY = (*platform)->posY - (*enemy)->sizeY;
                }
                break;
            }
        }
    }
}

void Playground::CheckPickupCollisions(Pickup *pickup)
{
    for (std::vector<Platform *>::iterator platform = platforms.begin(); platform != platforms.end(); ++platform)
    {
        if (pickup->state == Pickup::Falling and pickup->Collided(*platform))
        {
            pickup->velY = 0;
            pickup->state = Pickup::Stopped;

            // fix pickup position to be in line with platform
            if (pickup->posY + pickup->sizeY > (*platform)->posY)
            {
                pickup->posY = (*platform)->posY - pickup->sizeY;
            }
            break;
        }
    }
}

void Playground::Playground::Update()
{
    for (std::vector<Platform *>::iterator platform = platforms.begin(); platform != platforms.end(); ++platform)
    {
        (*platform)->Update();
    }
}

void Playground::Render(sf::RenderWindow &window)
{
    for (std::vector<Platform *>::iterator platform = platforms.begin(); platform != platforms.end(); ++platform)
    {
        (*platform)->Render(window);
    }
}

void Playground::MoveDown(float dy)
{
    for (std::vector<Platform *>::iterator platform = platforms.begin(); platform != platforms.end(); ++platform)
    {
        (*platform)->posY = (*platform)->posY - dy;
    }

    for (std::vector<Platform *>::iterator platform = platforms.begin(); platform != platforms.end(); ++platform)
    {
        if ((*platform)->posY > 800)
        {
            removePlatform((*platform));
            generateMore(1);
            break;
        }
    }
}

void Playground::GeneratePlatforms()
{
    // Let's split the screen into 12 boxes, each 200x200
    int boxW = 200, boxH = 200;
    int rows = 4, cols = 3;
    bool doGenerate = 0;
    do
    {
        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                doGenerate = rand() % 2;
                // Let's try to generate a platform in given box
                if (doGenerate and platforms.size() < 10)
                {  
                    generateRandomGameObjectsWithFixingOnGivenArea(i * boxW, j * boxH, i * boxW + boxW, j * boxH + boxH, 1, 0);
                }
            }
        }
    } while (platforms.size() < 10);

    generateMore(1); // oh, well, let's try creating one more platform if there's any space
}

void Playground::generateRandomGameObjectsWithFixingOnGivenArea(int startX, int startY, int endX, int endY, int n, int offsetY)
{
    int randX, randY, tryFixCnt;
    GameObject *closestObj;
    Platform *platform;
    closestObj = nullptr;

    for (int i = 0; i < n; i++)
    {
        // move on random position in given area and put platform
        randX = randomWithStep(startX, endX - 100, 5);
        randY = randomWithStep(startY, endY - 20, 5);
        randY += offsetY;

        platform = new Platform(randX, randY, texture, 2, 1);

        closestObj = getClosestObjInRange(platform, 160);
        if (closestObj == nullptr)
        {
            // if there is no platform in close range => great, we are done!
            platforms.push_back(platform);
        }
        else
        {
            // if there's another platform in close range ...
            tryFixCnt = 0;
            do
            {
                // ... well, let's try moving the new platform slightly and check if it's ok then
                fixPos(platform, closestObj);
                closestObj = getClosestObjInRange(platform, 160);
                tryFixCnt++;

                // oh, and let's do it 50 times!
                if (tryFixCnt > 50)
                {
                    break;
                }

            } while (closestObj != nullptr);

            if (closestObj == nullptr)
            {
                // if there is no platform in close range after fixing => thats what we wanted and we will use it in the game!
                platforms.push_back(platform);
            }
        }
    }
}

int Playground::randomWithStep(int min, int max, int step)
{
    int range = (max - min) / step;
    return min + (rand() % range) * step;
}

bool Playground::isValidPosition(GameObject *newObj)
{
    if (newObj->posX > SCREEN_WIDTH - 100)
    {
        return false; // too close to the right
    }
    if (newObj->posY > SCREEN_HEIGHT - 2 * 32)
    {
        return false; // too close to the bottom
    }

    if (newObj->posY < 2 * 32)
    {
        return false; // too close to the top
    }

    for (std::vector<Platform *>::iterator it = platforms.begin(); it != platforms.end(); ++it) 
    {
        float dist = newObj->distanceBetweenObjects(*it);
        if (dist < 150)
        {
            return false; // too close to other platform
        }
    }
    return true;
}

GameObject *Playground::getClosestObjInRange(GameObject *newObj, int range)
{
    int dist;
    int closestDistance = 1000;

    GameObject *closestObj = nullptr;

    // just iterate platforms and find closest one
    for (std::vector<Platform *>::iterator it = platforms.begin(); it != platforms.end(); ++it)
    {
        dist = newObj->distanceBetweenObjects(*it);

        if (dist < range)
        {
            if (dist < closestDistance)
            {
                closestObj = *it;
                closestDistance = dist;
            }
        }
    }
    return closestObj;
}

void Playground::fixPos(GameObject *newObj, GameObject *obj)
{
    // Depending on the situation, let's try moving the new platform slightly to see if it fits better
    if (obj->posX > newObj->posX)
    {
        // on the right side
        if (obj->posY > newObj->posY)
        {
            // on the top
            newObj->posY -= 1;
            newObj->posX -= 1;
        }
        else
        {
            // must be bottom
            newObj->posY += 1;
            newObj->posX -= 1;
        }
    }
    else
    {
        // must be on the left side
        if (obj->posY > newObj->posY)
        {
            // on the top
            newObj->posY -= 1;
            newObj->posX += 1;
        }
        else
        {
            // must be bottom
            newObj->posY -= 1;
            newObj->posX += 1;
        }
    }

    // fit to the screen goes next

    if (newObj->posX < 0)
    {
        newObj->posX = 0;
    }

    if (newObj->posX + 100 > SCREEN_WIDTH)
    {
        newObj->posX = SCREEN_WIDTH - 100;
    }

    if (newObj->posY + 20 > SCREEN_HEIGHT)
    {
        newObj->posY = SCREEN_HEIGHT - 20;
    }
}

void Playground::generateMore(int num)
{

    // This one is very similar to GeneratePlatforms but it operates only on the top part of the game screen
    // TODO probably should be merged with GeneratePlatforms

    int boxW = 200;
    int cols = 4;
    bool doGenerate = 0;

    generateRandomGameObjectsWithFixingOnGivenArea(200, 0, 400, 30, 1, -30);

    if (platforms.size() < 10)
    {
        do
        {
            for (int i = 0; i < cols; i++)
            {
                doGenerate = rand() % 2;

                if (doGenerate and platforms.size() < 10)
                {
                    generateRandomGameObjectsWithFixingOnGivenArea(i * boxW, 0, i * boxW + boxW, 200, 1, -200);
                }
            }

        } while (platforms.size() < 10);
    }
}

void Playground::removePlatform(Platform *block)
{
    for (std::vector<Platform *>::iterator it = platforms.begin(); it != platforms.end(); ++it)
    {
        if (*it == block)
        {
            delete *it;
            platforms.erase(it);
            break;
        }
    }
}

Playground::~Playground()
{
    for (std::vector<Platform *>::iterator it = platforms.begin(); it != platforms.end(); ++it)
    {
        delete *it;
    }
    platforms.clear();
}