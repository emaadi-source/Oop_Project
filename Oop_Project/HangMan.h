#pragma once

// This contains the following hangman game classes:
// Hangman class which has:
// -> Category
// -> Hangman Figure

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Game.h"
#include "SoundSystem.h"

using namespace std;

class Hangman : public Game {
private:
    sf::Font font;
    sf::Text text;
    sf::Text scoreText;
    sf::Text livesText;
    sf::Text statusText;
    sf::Text gameOverText;
    sf::Text hintText;

    sf::Sprite hangmanSprite;
    sf::Sprite background;
    sf::Sprite gameOverSprite;

    sf::Texture hangmanTexture;
    sf::Texture backgroundTexture;

    SoundSystem* soundSystem;

    const int maxWords = 10;
    const string words[10] = { "PLAIN", "PROGRAM", "BANANA", "CODE", "TRIAD", "WRIST", "HAND", "FACE", "JANUARY", "OCTOPUS" };
    const string hints[10] = {
    "A flat or level surface, often simple in design.",        // PLAIN
    "A set of instructions written to perform a task.",       // PROGRAM
    "A tropical fruit, often yellow, curved, and sweet.",     // BANANA
    "A series of instructions for computers.",               // CODE
    "A group of three closely related things.",              // TRIAD
    "A joint connecting the hand and arm.",                  // WRIST
    "Parts of the body used for grasping or holding.",       // HANDS
    "The front part of the head, often used for expression.", // FACE
    "The first month of the year.",                          // JANUARY
    "A sea creature with eight limbs.",                      // OCTOPUS
    };

    // already guessed words
    bool usedIndices[10] = { false };
    int usedCount = 0;
    string currentWord = "";
    string guessedWord = "";

    int lives;
    int index;
    bool end = false;
    bool showHint = false;

    bool gameOverDelayActive = false; // Delay flag
    sf::Clock gameOverClock;

    bool newGameDelayActive = false; // New game delay flag
    sf::Clock newGameClock;
public:
    Hangman(Leaderboard* lptr) : Game(lptr) {
        setName("Hangman");

        if (!font.loadFromFile("./fonts/arial.ttf")) {
            cerr << "Error loading font!" << endl;
        }
        
        resetGame();
    }

    void startGame(sf::RenderWindow& window) override {
        Game::startGame(window);
        end = false;
        cout << "Hangman game is running..." << endl;
    }

    void handleInput(sf::RenderWindow& window, Player* player, string currentKey, InputSystem* inputSystem, SoundSystem* sptr) {
        Game::handleInput(window, player, currentKey);
        soundSystem = sptr;
        if (currentKey == "ENTER") {
            end = false;
            render(window, player);
            gameLoop(window, player, inputSystem);
        }
    }

    void render(sf::RenderWindow& window, Player* player) override {
        if (isMenuActive) {
            Game::render(window, player); 
        }
        else {
            window.clear();

            // draw background
            if (!backgroundTexture.loadFromFile("./img/hangman_background.png"))
            {

            }
            background.setTexture(backgroundTexture);
            float scaleX = static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x;
            float scaleY = static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y;

            background.setScale(scaleX, scaleY);
            window.draw(background);

            // Display player info
            float scoreX = window.getSize().x - 10.f;
            text.setFont(font);
            text.setString("Player Name: " + player->getName());
            text.setCharacterSize(24);
            text.setFillColor(sf::Color::Black); 
            text.setPosition(10, 10);

            scoreText.setFont(font);
            scoreText.setString("Score: " + to_string(getScore()));
            scoreText.setCharacterSize(24);
            scoreText.setFillColor(sf::Color::Black);
            scoreText.setPosition(scoreX - scoreText.getLocalBounds().width - 10, 10);

            livesText.setFont(font);
            livesText.setString("Lives: " + to_string(lives));
            livesText.setCharacterSize(24);
            livesText.setFillColor(sf::Color::Black);
            livesText.setPosition(window.getSize().x / 2 - livesText.getLocalBounds().width + 30, 10);

            statusText.setFont(font);
            if (statusText.getLocalBounds().width > 50)
                statusText.setCharacterSize(30);
            else
                statusText.setCharacterSize(35);
            statusText.setFillColor(sf::Color::Black);
            statusText.setPosition(50, window.getSize().y - 90);

            hangmanSprite.setPosition(10, 100);

            // draw hint text IF user selcts hint
            if (showHint) {
                string hint = "";
                for (int i = 0; i < hints[index].length(); i++)
                {
                    if (i % 42 == 0 && i != 0)
                        hint += '\n';
                    hint += hints[index][i];
                }
                hintText.setFont(font);
                hintText.setString(hint);
                hintText.setCharacterSize(20);
                hintText.setFillColor(sf::Color::Black);

                hintText.setPosition(window.getSize().x/2 + 10, 155);

                window.draw(hintText);
            }
            // Draw everything
            window.draw(text);
            window.draw(scoreText);
            window.draw(livesText);
            window.draw(statusText);
            window.draw(hangmanSprite);

            // if game ends, show  screen
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

                float scaleX = static_cast<float>(window.getSize().x) / textureSize.x;
                float scaleY = static_cast<float>(window.getSize().y) / textureSize.y;
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
                gameOverText.setPosition(window.getSize().x / 2 + 10, window.getSize().y / 2 + 10);

                // Draw the text
                window.draw(gameOverText);
            }

            window.display();
         }
    }

    // main game loop
    void gameLoop(sf::RenderWindow& window, Player* player, InputSystem* inputSystem) {
        sf::Clock clock;
        sf::Time refreshRate = sf::seconds(0.2f);
        int keyPressCount = 0;
        bool keyPress = false;
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
                    else if (inputSystem->isAlphabetPressed())
                    {
                        keyPressCount++;
                        //cout << currentKey << endl;
                        if (keyPressCount % 2 == 0) {
                            if (lives > 0 && guessedWord != currentWord) {
                                char guessedChar = toupper(currentKey[0]);
                                processGuess(guessedChar);
                                updateStatusText();
                            }
                        }
                    }
                    else if (currentKey == "ENTER" && end) {
                        // End the game and return to main menu
                        endGame();
                        return;
                    }
                    else if (currentKey == "TAB") {
                        // show hint
                        showHint = true;
                    }
                }
            }
            // if all words used up or lives is 0 
            if (lives <= 0 && !gameOverDelayActive) {
                soundSystem->playSound("lose");
                gameOverDelayActive = true;
                gameOverClock.restart();
            }

            // Handle the game-over delay
            if (gameOverDelayActive && gameOverClock.getElapsedTime().asSeconds() >= 2.5) {
                render(window, player);
                end = true;
            }


            // Handle the new game delay

            if (guessedWord == currentWord && !newGameDelayActive) {
                newGameDelayActive = true;
                newGameClock.restart();
            }

            if (newGameDelayActive && newGameClock.getElapsedTime().asSeconds() >= 2.5) {
                render(window, player);
                resetGame();
                newGameDelayActive = false;
            }
            render(window, player);
        }
            
    }

    void resetGame() {
        // Select a random unused word
        do {
            int randIndex = rand() % maxWords;
            if (!usedIndices[randIndex]) {
                usedIndices[randIndex] = true;
                currentWord = words[randIndex];
                index = randIndex;
                usedCount++;
                break;
            }
        } while (true);

        guessedWord = string(currentWord.length(), '_');
        lives = 4;
        showHint = false;
        updateStatusText();
        loadHangmanTexture();
    }
    void processGuess(char guessedChar) {
        bool found = false;
        for (size_t i = 0; i < currentWord.length(); ++i) {
            if (currentWord[i] == guessedChar) {
                guessedWord[i] = guessedChar;
                found = true;
            }
        }
        if (!found) {
            lives--;
            loadHangmanTexture();
        }
    }

    void updateStatusText() {
        if (guessedWord == currentWord) {
            soundSystem->playSound("correct");
            setScore(getScore() + 10);
            statusText.setString("You won! \nThe word was: " + currentWord);
        }
        else if (lives <= 0) {
            statusText.setString("You lost! \nThe word was: " + currentWord);
        }
        else {
            string displayWord = "";
            for (size_t i = 0; i < guessedWord.length(); i++)
            {
                displayWord += guessedWord[i];
                displayWord += ' ';
            }
            statusText.setString("Word: " + displayWord);
        }
    }

    void loadHangmanTexture() {
        std::string texturePath = "./img/mistake_" + std::to_string(4 - lives) + ".png";
        if (!hangmanTexture.loadFromFile(texturePath)) {
            cerr << "Error loading hangman texture!" << endl;
        }
        hangmanSprite.setTexture(hangmanTexture);
    }

    // end game
    void endGame() override {
        currentPlayer->setHighscore(1, getScore());
        Game::endGame();
    }
};
