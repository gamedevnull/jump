#include "include/Enemy.hpp"
#include "include/Animation.hpp"
#include "include/globals.hpp"

Enemy::Enemy(Texture *texture)
{
    GameObject::Init(300, 200, 28, 36);
    animation = new Animation(texture, 28, 36, 3, 0, 579, 10);
    objectType = EnemyId;
    state = Falling;
}

void Enemy::Update()
{
    animation->Update();

    if (state == Falling)
    {
        velY += 0.1;
    }
    GameObject::Update();
}

void Enemy::Render(sf::RenderWindow &window)
{
    // GameObject::Render(window);
    Sprite sprite = animation->getSprite();
    sprite.setPosition(posX, posY);
    window.draw(sprite);
}

Enemy::~Enemy()
{
}