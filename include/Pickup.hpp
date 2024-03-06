#pragma once
#include "Animation.hpp"
#include "GameObject.hpp"

class Pickup : public GameObject
{
public:
    int state;
    enum stateID {
        Falling,
        Stopped
    };
    sf::Texture *texture;

    Pickup(Texture *texture);
    void Update();
    void Render(sf::RenderWindow &window);
    ~Pickup();
};