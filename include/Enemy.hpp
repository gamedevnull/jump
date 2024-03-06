#pragma once
#include "Animation.hpp"
#include "GameObject.hpp"

class Enemy : public GameObject
{
public:
 int state;
    enum stateID {
        Falling,
        Stopped
    };
    Animation *animation;
    
    Enemy(Texture *texture);
    void Update();
    void Render(sf::RenderWindow &window);
    ~Enemy();
};