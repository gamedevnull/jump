#include "include/Player.hpp"
#include "include/Animation.hpp"
#include "include/globals.hpp"

Player::Player(Texture *texture)
{
    GameObject::Init(200, 200, 30, 44);
    animation = new Animation(texture, 24, 24, 4, 0, 550, 5);
    objectType = PlayerId;
    fuel = 20;
    jetpackFireLen = 0;
}

void Player::Update()
{
    animation->Update();

    velY += 0.2;

    if (posY + sizeY > SCREEN_HEIGHT)
    {
        velY = 0;
    }

    if (posX - sizeX >= SCREEN_WIDTH and velX > 0)
    {
        posX = 0;
    }

    if (posX + sizeX <= 0 and velX < 0)
    {
        posX = SCREEN_WIDTH;
    }

    int oldX = posX;

    GameObject::Update();

    int newX = posX;

    if (state != UsingJetPack)
    {
        if (velX < -1 or velX > 1)
        {
            if (newX - oldX >= 1)
            {
                state = JumpingRight;
            }
            else
            {
                state = JumpingLeft;
            }
        }
    }
}

void Player::Render(sf::RenderWindow &window)
{
    // GameObject::Render(window);

    Sprite sprite = animation->getSprite();

    if (state == UsingJetPack)
    {
        jetpackFireLen++;
        if (jetpackFireLen > 10)
        {
            jetpackFireLen = 5;
        }
        sprite.setScale(2.0f, 2.0f);
        sprite.setPosition(posX - 8, posY);

        RectangleShape tile;

        tile.setSize(Vector2f(3, jetpackFireLen));
        tile.setFillColor(Color(255, 0, 0));
        tile.setPosition(posX + (sizeX / 2) - 6, posY + sizeY - 4);
        window.draw(tile);

        tile.setSize(Vector2f(3, jetpackFireLen));
        tile.setFillColor(Color(255, 0, 0));
        tile.setPosition(posX + (sizeX / 2) + 3, posY + sizeY - 4);
        window.draw(tile);
    }
    else
    {
        if (state == JumpingLeft)
        {
            sprite.setScale(-2.0f, 2.0f);
            sprite.setPosition(posX + 40, posY);
        }
        else
        {
            sprite.setScale(2.0f, 2.0f);
            sprite.setPosition(posX - 8, posY);
        }
    }

    window.draw(sprite);
}

Player::~Player()
{
    delete animation;
}