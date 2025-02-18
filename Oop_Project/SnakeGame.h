#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <cstdlib>
#include "SnakeClasses.h"
#include "cmath"

using namespace std;


class SnakeGame : public Game {
    sf::Text text;
    sf::Text scoreText;
    sf::Text levelText;
    sf::Text gameOverText;
    sf::Font font;

    sf::RectangleShape bar;
    sf::Texture headTexture, segmentTexture;
    sf::Sprite gameOverSprite;

    SnakeSegment* snakeBody; 
    int snakeLength;           //  length of the snake
    int capacity;              // max length of snake
    int direction[2] = { 0, 0 };    //  movement direction (+-x or +-y) right and up are 1 and down and left are -1
    int speed;

    Point food;

    bool end = false;
    bool paused = false;
public:
    SnakeGame(Leaderboard* lptr, sf::RenderWindow& window)
        : Game(lptr),
        speed(1),
        snakeLength(3), // initial length = 3
        capacity(30),
        food(8)
    {
        setName("SnakeGame");

        if (!font.loadFromFile("./fonts/arial.ttf")) {
            cerr << "Error loading font!" << endl;
        }
        text.setFont(font);

        // Load textures
        if (!headTexture.loadFromFile("./img/head.png") ||
            !segmentTexture.loadFromFile("./img/body.png")) {
            cerr << "Error loading snake textures!" << endl;
        }

        // initialize
        direction[0] = 1;
        snakeBody = new SnakeSegment[capacity];
        const float headPos[2] = { 100.0, 200.0 };
        snakeBody[0] = SnakeSegment(headTexture, headPos);

        // Initialize the rest of the body
        for (int i = 1; i < snakeLength; i++) {
            float bodyPos[2] = { 100 - (snakeBody[i].getDimensions()[0] * i), 200 };
            snakeBody[i] = SnakeSegment(segmentTexture, bodyPos);

        }

        // initialize food (Point)
        spawnFood(window);
    }

    ~SnakeGame() {
        delete[] snakeBody; 
    }


    void startGame(sf::RenderWindow& window) override{
        Game::startGame(window);
        //reset vars
        snakeLength = 3;  direction[0] = 1; direction[1] = 0; 
        speed = 1; end = false; paused = false; 
        const float headPos[2] = { 100.0, 200.0 };
        snakeBody[0] = SnakeSegment(headTexture, headPos);

        for (int i = 1; i < snakeLength; i++) {
            float bodyPos[2] = { 100 - (snakeBody[i].getDimensions()[0] * i), 200 };
            snakeBody[i] = SnakeSegment(segmentTexture, bodyPos);
        }

        spawnFood(window);

        cout << "Snake game is running..." << endl;
    }

    void handleInput(sf::RenderWindow& window, Player* player, string currentKey, InputSystem* inputSystem)
    {
        Game::handleInput(window, player, currentKey);
        if (currentKey == "ENTER") {
            end = false;
            render(window, player);
            gameLoop(window, player, inputSystem);
        }
    }

    // override render function to render snake specifc graphics
    void render(sf::RenderWindow& window, Player* player) override {
        if (isMenuActive) {
            Game::render(window, player); 
        }
        else {
            window.clear();
            
            // Draw the score and player name in the first row
            sf::Vector2u windowSize = window.getSize();
            float scoreX = windowSize.x - 10.f; 

            bar.setSize(sf::Vector2f(window.getSize().x, 5));
            bar.setFillColor(sf::Color::Green);
            bar.setPosition(0, 45);

            scoreText.setFont(font);
            text.setFont(font);
            levelText.setFont(font);

            text.setString("Name: " + player->getName());
            scoreText.setString("Score: " + to_string(getScore()));  
            levelText.setString("Level " + to_string(int(speed)));

            scoreText.setCharacterSize(24);
            text.setCharacterSize(24);
            levelText.setCharacterSize(24);

            text.setFillColor(sf::Color::White);
            scoreText.setFillColor(sf::Color::White);
            levelText.setFillColor(sf::Color::White);

            text.setPosition(10, 8);
            scoreText.setPosition(scoreX - scoreText.getLocalBounds().width - 10, 8);  // Adjust to the left to fit the score text within the window
            levelText.setPosition(windowSize.x / 2 - levelText.getLocalBounds().width + 25, 8);

            window.draw(bar);
            window.draw(text);
            window.draw(scoreText);
            window.draw(levelText);

            // Draw the snake
            for (int i = 0; i < snakeLength; i++) {
                snakeBody[i].draw(window);
            }

            // Draw the food
            food.draw(window);


            // if game ends, show loosing screen
            if (end)
            {
                window.clear();

                // Load the image into a texture
                sf::Texture gameOverTexture;
                if (!gameOverTexture.loadFromFile("./img/loosing_screen.png"))
                {
                    cerr << "Error loading image!" << endl;
                    return;
                }

                // Create a sprite and set the texture
                gameOverSprite.setTexture(gameOverTexture);

                // Scale the sprite to fit the window size
                sf::Vector2u textureSize = gameOverTexture.getSize();

                float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
                float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
                gameOverSprite.setScale(scaleX, scaleY);

                // Draw the sprite as the background
                window.draw(gameOverSprite);

                gameOverText.setFont(font);
                gameOverText.setString("Score: " + to_string(getScore()));
                gameOverText.setCharacterSize(35);
                gameOverText.setFillColor(sf::Color::White);

                // Center the text on the screen
                sf::FloatRect textBounds = gameOverText.getLocalBounds();
                gameOverText.setOrigin(textBounds.width / 2, textBounds.height / 2);
                gameOverText.setPosition(windowSize.x / 2 + 10, windowSize.y / 2 + 10);

                // Draw the text
                window.draw(gameOverText);
            }


            window.display();
        }
    }

    // local game loop
    void gameLoop(sf::RenderWindow& window, Player* player, InputSystem* inputSystem) {
        sf::Clock clock;
        sf::Time elapsedTime;
        int spacePressCount = 0; // Counter for SPACE presses

        while (window.isOpen()) {
            sf::Event event;
            string currentKey = "";

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }

                inputSystem->handleInput();

                if (inputSystem->isAnyKeyPressed()) {
                    currentKey = inputSystem->getCurrentPressedKey();

                    if (currentKey == "ESC") {
                        end = true;
                    }
                    else if (currentKey == "SPACE") {
                        spacePressCount++;

                        if (spacePressCount % 2 == 0) {
                            paused = !paused; // Toggle paused state
                            if (!paused) {
                                clock.restart();
                            }

                        }
                    }
                    else if (currentKey == "ENTER" && end) {
                        // End the game and return to main menu
                        endGame();
                        return;
                    }
                    if (!paused) { // Allow direction changes only when not paused
                        if (currentKey == "UP" && direction[1] != 1) {
                            direction[0] = 0; direction[1] = -1;
                        }
                        else if (currentKey == "DW" && direction[1] != -1) {
                            direction[0] = 0; direction[1] = 1;
                        }
                        else if (currentKey == "LF" && direction[0] != 1) {
                            direction[0] = -1; direction[1] = 0;
                        }
                        else if (currentKey == "RT" && direction[0] != -1) {
                            direction[0] = 1; direction[1] = 0;
                        }
                    }
                }
                else {
                    currentKey = "";
                }
            }

           
            if (!paused) {
                sf::Time refreshRate = sf::seconds(0.2f - (0.035f * speed));
                elapsedTime += clock.restart();
                if (elapsedTime >= refreshRate) {
                    elapsedTime -= refreshRate;

                    if (!detectCollision(window)) {
                        moveSnake();
                        eatFood(window);
                    }
                    else {
                        end = true; // End the game on collision
                    }
                    render(window, player);
                }
            }
            else {
                sf::sleep(sf::milliseconds(100)); 
            }
        }
    }



    void moveSnake() {
        float prevX = snakeBody[0].getPosition()[0];
        float prevY = snakeBody[0].getPosition()[1];

        float newHeadX = prevX + direction[0] * 30;
        float newHeadY = prevY + direction[1] * 30;
        snakeBody[0].setPosition(newHeadX, newHeadY);

        // also, rotate head in direction of movement

        for (int i = 1; i < snakeLength; i++) {
            float tempX = snakeBody[i].getPosition()[0];
            float tempY = snakeBody[i].getPosition()[1];

            // Set the current segment to the position of the previous one
            snakeBody[i].setPosition(prevX, prevY);
            prevX = tempX;
            prevY = tempY;
        }
    }



    bool detectCollision(sf::RenderWindow& window)
    {
        // calculate exact position of the start of the snake head 
        // make sure to factor in segment size as well
        float headPos[2] = {
            snakeBody[0].getPosition()[0],
            snakeBody[0].getPosition()[1]
        };

        // check all 4 walls

        //right
        if (headPos[0] + snakeBody[0].getDimensions()[0] >= window.getSize().x)
        {
            return true;
        }
        //left
        else if (headPos[0] - snakeBody[0].getDimensions()[0] <= 0)
        {
            return true;
        }
        //top
        else if (headPos[1] - snakeBody[0].getDimensions()[1] <= 40)
        {
            return true;
        }
        //bottom
        else if (headPos[1] + snakeBody[0].getDimensions()[0] >= window.getSize().y)
        {
            return true;
        }

        // SELF collision
        for (int i = 1; i < snakeLength; ++i) {
            if (snakeBody[0].getPosition()[0] == snakeBody[i].getPosition()[0] &&
                snakeBody[0].getPosition()[1] == snakeBody[i].getPosition()[1]) {
                return true; 
            }
        }

        return false;
    }

    // random spawn of food
    void spawnFood(sf::RenderWindow& window) {
        int gridSize = 30; 
        int maxX = window.getSize().x / gridSize;
        int maxY = (window.getSize().y - 40 + gridSize) / gridSize;

        bool validPosition = false;
        float newX, newY;

        while (!validPosition) {
            newX = (rand() % maxX) * gridSize;
            newY = ((rand() % maxY) * gridSize) + 50;

            validPosition = true;
            for (int i = 0; i < snakeLength; i++) {
                if (snakeBody[i].getPosition()[0] == newX &&
                    snakeBody[i].getPosition()[1] == newY || newY < 80 || 
                    newX < 40 || newX > (window.getSize().x -40) || newY >(window.getSize().y - 40)) {
                    validPosition = false; 
                    break;
                }
            }
        }

        food.setPosition(newX, newY);
    }

    // check if snake eats food
    void eatFood(sf::RenderWindow& window) {
        float headX = snakeBody[0].getPosition()[0] + 15;
        float headY = snakeBody[0].getPosition()[1];

        int rad = food.getRadius();

        float foodX = food.getPosition()[0];
        float foodY = food.getPosition()[1];

        float collisionRadius = 15;

        // Calculate distance between snake's head and the food
        float distance = sqrt(pow(headX - foodX, 2) + pow(headY - foodY, 2));

        if (distance <= collisionRadius) {
            spawnFood(window); // Generate new food

            // Add new segment at the tail
            float tailX = snakeBody[snakeLength - 1].getPosition()[0];
            float tailY = snakeBody[snakeLength - 1].getPosition()[1];
            float segmentWidth = snakeBody[snakeLength - 1].getDimensions()[0];
            float segmentHeight = snakeBody[snakeLength - 1].getDimensions()[1];

            // Position the new segment relative to the tail
            float newSegmentX = tailX - direction[0] * segmentWidth;
            float newSegmentY = tailY - direction[1] * segmentHeight;
            const float pos[2] = { newSegmentX, newSegmentY };
            snakeBody[snakeLength] = SnakeSegment(segmentTexture, pos);

            snakeLength++;
            setScore(getScore() + 10);
            // increase level for each 5 eats
            if (getScore() >= 200)
            {
                end = true;
            }

            if (getScore() % 50 == 0)
            {
                speed = speed + 1;
            }
        }
    }


    // end game
    void endGame() override {
        currentPlayer->setHighscore(0, getScore());
        Game::endGame();
    }
};