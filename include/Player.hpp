#pragma once
#include "Animation.hpp"
#include "GameObject.hpp"

class Player : public GameObject
{
public:
    int state;
    enum stateID {
        Idle,
        JumpingLeft,
        JumpingRight,
        UsingJetPack
    };
    int fuel;
    int jetpackFireLen;
    Animation *animation;

    Player(Texture *texture);
    void Update();
    void Render(sf::RenderWindow &window);
    ~Player();
};