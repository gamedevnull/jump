#include <iostream>
#include <fstream>
#include <ostream>
#include <SFML/Graphics.hpp>
#include "include/globals.hpp"
#include "include/Platform.hpp"
#include "include/Background.hpp"
#include "include/Playground.hpp"
#include "include/Player.hpp"
#include "include/Enemy.hpp"
#include "include/Pickup.hpp"
#include "include/SpecialEffects.hpp"
using namespace sf;

class GameInput
{
public:
    int moveX;
    bool spaceBar, keyP;
    GameInput() : moveX(0), spaceBar(0), keyP(0) {}
};

class GameState
{
public:
    enum States
    {
        Title,
        Playing,
        Paused,
        GameOver
    };
    int currentState, currentScore, highestScore;
    const char *highscoreFilename;

    GameState()
    {
        highestScore = 0;
        highscoreFilename = "hiscore";
    }

    void loadHighScore()
    {
        std::ifstream inputFile(highscoreFilename);
        inputFile >> highestScore;
        inputFile.close();
    }

    void saveHighScore()
    {
        std::ofstream outputFile(highscoreFilename);
        outputFile << highestScore;
        outputFile.close();
    }

    bool onTitle()
    {
        return currentState == Title;
    }

    bool onGameOver()
    {
        return currentState == GameOver;
    }

    bool onPause()
    {
        return currentState == Paused;
    }

    bool onPlaying()
    {
        return currentState == Playing;
    }

    void set(int newState)
    {
        currentState = newState;
        if (newState == GameOver)
        {
            if (currentScore > highestScore)
            {
                highestScore = currentScore;
                saveHighScore();
            }
        }
    }
};

class JumpGame
{
public:
    RenderWindow window;
    Font font;
    GameInput input;
    GameState state;
    Texture *gameTextures;
    Background *background;
    Playground *playground;
    Player *player;
    Platform *platform;
    Enemy *enemy;
    Pickup *pickup;
    SpecialEffects *sparks;
    std::vector<Enemy *> bombs;
    int maxBombs;

    JumpGame() : window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Jump!")
    {
        window.setFramerateLimit(60);

        font.loadFromFile("font.ttf");

        state.currentState = GameState::Title;
        state.loadHighScore();

        /*
        Free assets:
        - platforms: https://opengameart.org/content/cute-platformer-sisters
        - clouds: https://opengameart.org/content/old-frogatto-clouds
        - creeper: https://www.spriters-resource.com/custom_edited/minecraftcustoms/sheet/91008 
        */
        gameTextures = loadTexture("textures.png");
        
        background = new Background(gameTextures);
        player = new Player(gameTextures);
        playground = new Playground(gameTextures);

        enemy = new Enemy(gameTextures);
        bombs.push_back(enemy);

        pickup = new Pickup(gameTextures);
        sparks = new SpecialEffects();

        maxBombs = 1;
    }

    void restartGame()
    {
        state.currentScore = 0;

        player->posX = rand() % 550;
        player->posY = 400;
        player->velX = 0;
        player->velY = 0;
        player->fuel = 10;

        playground->GeneratePlatforms();

        pickup->posX = rand() % 550;
        pickup->posY = -300;
        pickup->velY = 0;
        pickup->state = Pickup::Falling;

        clearIteratorAndElements(&bombs);
        enemy = new Enemy(gameTextures);
        enemy->posY = -300;
        enemy->posX = rand() % 550;
        enemy->velY = 0;
        enemy->state = Enemy::Falling;
        bombs.push_back(enemy);
    }

    void Update()
    {
        background->Update();

        sparks->Update();

        if (state.onTitle())
        {
            if (input.spaceBar)
            {
                state.set(GameState::Playing);
                input.spaceBar = 0;
                restartGame();
            }
        }
        else if (state.onPlaying())
        {
            if (input.moveX > 0) // JUMP RIGHT
            {
                if (player->velX < 5)
                {
                    player->velX += 0.2;
                }
            }
            else if (input.moveX < 0) // JUMP LEFT
            {
                if (player->velX > -5)
                {
                    player->velX -= 0.2;
                }
            }

            // Move the player higher and the platforms lower
            if (player->posY < 200)
            {
                player->posY = 200;
                state.currentScore++;

                background->MoveUp(player->velY);
                playground->MoveDown(player->velY);

                // adjust the position of the collectible pickup
                if (pickup->state == Pickup::Stopped)
                {
                    pickup->state = Pickup::Falling;
                }
                pickup->posY = pickup->posY - player->velY;

                // adjust bombs positions
                for (std::vector<Enemy *>::iterator it = bombs.begin(); it != bombs.end(); ++it)
                {
                    if ((*it)->state == Pickup::Stopped)
                    {
                        (*it)->state = Pickup::Falling;
                    }
                    (*it)->posY = (*it)->posY - player->velY;
                }
            }

            if (input.spaceBar) // JETPACK USAGE
            {
                if (player->fuel > 0)
                {
                    player->velY = -8;
                    player->fuel--;
                    player->state = Player::UsingJetPack;
                }
                else
                {
                    player->state = Player::Idle;
                }
            }
            else
            {
                player->state = Player::Idle;
            }

            // DETECT COLLISIONS
            playground->CheckPlayerCollisions(player);
            playground->CheckEnemyCollisions(bombs);
            playground->CheckPickupCollisions(pickup);

            // reuse the same enemy if it falls off the screen
            for (std::vector<Enemy *>::iterator it = bombs.begin(); it != bombs.end(); ++it)
            {
                if ((*it)->posY > 800)
                {
                    (*it)->posY = -300;
                    (*it)->posX = rand() % 570;
                    (*it)->velY = 0;
                    (*it)->state = Enemy::Falling;
                }
            }

            // update final positions and animations
            player->Update();
            playground->Update();
            pickup->Update();
            for (std::vector<Enemy *>::iterator it = bombs.begin(); it != bombs.end(); ++it)
            {
                (*it)->Update();
            }

            // check if it's GAME OVER when the player collides with an enemy
            for (std::vector<Enemy *>::iterator it = bombs.begin(); it != bombs.end(); ++it)
            {
                if (player->Collided(*it))
                {
                    sparks->SpawnEffect(player->posX, player->posY, 30);
                    state.set(GameState::GameOver);
                    return;
                }
            }

            // fuel up when the player collides with an pickup
            if (player->Collided(pickup))
            {
                if (player->fuel < 250)
                {
                    player->fuel += 50;
                    if (player->fuel > 250)
                    {
                        player->fuel = 250;
                    }
                }
                // reuse the same pickup if it falls off the screen
                pickup->posX = rand() % 550;
                pickup->posY = -300;
                pickup->velY = 0;
                pickup->state = Pickup::Falling;
            }

            spawnBombs();

            // its also game over when the player falls off the screen
            if (player->posY + player->sizeY >= 800)
            {
                state.set(GameState::GameOver);
                maxBombs = 1;
                return;
            }

            if (input.keyP) // PAUSE
            {
                state.set(GameState::Paused);
                input.keyP = 0;
            }
        }
        else if (state.onPause())
        {
            if (input.keyP)
            {
                state.set(GameState::Playing);
                input.keyP = 0;
            }
        }
        else if (state.onGameOver())
        {
            if (input.spaceBar)
            {
                state.set(GameState::Title);
                input.spaceBar = 0;
            }
        }
    }

    void spawnBombs()
    {
        if (state.currentScore > 200 and state.currentScore < 400)
        {
            maxBombs = 2;
        }
        else if (state.currentScore > 400 and state.currentScore < 600)
        {
            maxBombs = 3;
        }
        else if (state.currentScore > 600 and state.currentScore < 700)
        {
            maxBombs = 4;
        }
        else if (state.currentScore > 700 and state.currentScore < 900)
        {
            maxBombs = 5;
        }
        else if (state.currentScore > 1000 and state.currentScore < 1800)
        {
            maxBombs = 6;
        }
        else if (state.currentScore > 1800)
        {
            maxBombs = 10; // good luck! ;)
        }
        else
        {
            maxBombs = 1;
        }

        if (bombs.size() < maxBombs)
        {
            for (int i = 0; i < (maxBombs - bombs.size()); i++)
            {
                enemy = new Enemy(gameTextures);
                enemy->posY = -1 * (rand() % 400 + 200);
                enemy->posX = rand() % 550;
                enemy->velY = 0;
                enemy->state = Enemy::Falling;
                bombs.push_back(enemy);
            }
        }
    }

    void Render()
    {
        window.clear(Color(76, 180, 255));

        background->Render(window);

        sparks->Render(window);

        if (state.onTitle())
        {
            RenderText("Jump!", 185, 260, 64);
        }
        else if (state.onPlaying() or state.onPause())
        {
            playground->Render(window);

            pickup->Render(window);

            for (std::vector<Enemy *>::iterator it = bombs.begin(); it != bombs.end(); ++it)
            {
                (*it)->Render(window);
            }

            player->Render(window);

            RenderText("score: ", 10, 10, 22);
            RenderNumber(state.currentScore, 110, 10, 22);

            RenderText("hi: ", 200, 10, 22);
            RenderNumber(state.highestScore, 240, 10, 22);

            RenderText("fuel: ", 480, 10, 22);
            RenderNumber(player->fuel, 550, 10, 22);
        }
        else if (state.onGameOver())
        {
            RenderText("Game Over", 100, 260, 60);
            RenderText("score: ", 155, 350, 32);
            RenderNumber(state.currentScore, 310, 350, 32);
        }
        window.display();
    }

    void RenderText(const char *string, int x, int y, int size)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(string);
        text.setFillColor(Color(255, 255, 255));
        text.setCharacterSize(size);
        text.setPosition(x, y);
        window.draw(text);
    }

    void RenderNumber(int number, int x, int y, int size)
    {
        sf::Text text;
        text.setFont(font);
        text.setString(std::to_string(number));
        text.setFillColor(Color(255, 255, 255));
        text.setCharacterSize(size);
        text.setPosition(x, y);
        window.draw(text);
    }

    void run()
    {
        // const sf::Vector2u windowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        // window.setSize(windowSize);

        while (window.isOpen())
        {
            Event e;
            while (window.pollEvent(e))
            {
                switch (e.type)
                {
                case Event::Closed:
                    window.close();
                    break;
                    // case Event::Resized:
                    //     window.setSize(windowSize);
                    //     break;

                case sf::Event::KeyPressed:
                    switch (e.key.code)
                    {
                    case sf::Keyboard::Left:
                        input.moveX = -1;
                        break;
                    case sf::Keyboard::Right:
                        input.moveX = 1;
                        break;
                    case sf::Keyboard::Space:
                        input.spaceBar = 1;
                        break;
                    case sf::Keyboard::P:
                        input.keyP = 1;
                        break;
                    default:
                        break;
                    }
                    break;

                case sf::Event::KeyReleased:
                    switch (e.key.code)
                    {
                    case sf::Keyboard::Left:
                        input.moveX = 0;
                        break;
                    case sf::Keyboard::Right:
                        input.moveX = 0;
                        break;
                    case sf::Keyboard::Space:
                        input.spaceBar = 0;
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
                }
            }

            Update();
            Render();
        }
    }

    ~JumpGame()
    {
        delete background;
        delete playground;
        delete player;
        delete pickup;
        delete sparks;
        clearIteratorAndElements(&bombs);
        delete gameTextures;
    }

    template <typename Container>
    void clearIteratorAndElements(Container *container)
    {
        for (typename Container::iterator it = container->begin(); it != container->end(); ++it)
        {
            delete *it;
        }
        container->clear();
    }

    Texture *loadTexture(const char *filename, sf::Color transparencyColor = sf::Color::Transparent)
    {
        Texture *tex = new Texture();
        Image img;
        if (img.loadFromFile(filename))
        {
            img.createMaskFromColor(transparencyColor);
            tex->loadFromImage(img);
        }
        return tex;
    }
};

int main()
{
    JumpGame app;
    app.run();
    return 0;
}