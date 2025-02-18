#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Game.h"
#include "WordleClasses.h"

class Wordle : public Game {
    static const int WORD_LENGTH = 5;
    static const int NUMBER_OF_GUESSES = 5;
    static const int NEW_WORD_LIMIT = 3; // words per player

    const string WORD_LIST[20] = {
    "apple", "berry", "charm", "dream", "eagle",
    "flame", "grape", "heart", "ivory", "joker",
    "knack", "lemon", "magic", "noble", "oasis",
    "pearl", "queen", "raven", "storm", "tiger"
    };

    sf::Font font;
    sf::Text feedbackText;
    sf::Text text;
    sf::Text scoreText;
    sf::Text gameOverText;

    sf::RectangleShape bar;
    sf::Sprite gameOverSprite;

    Boxes* boxRows[NUMBER_OF_GUESSES];
    string targetWord;
    string tries[NUMBER_OF_GUESSES];

    int matches[NUMBER_OF_GUESSES][WORD_LENGTH];
    int currentTry = 0;
    int newWordCount = 0;
    int index = 0;
    bool end = false;
public:
    Wordle(Leaderboard* leaderboard)
        : Game(leaderboard), currentTry(0) {
        setName("Wordle");
        if (!font.loadFromFile("./fonts/arial.ttf")) {
            cerr << "Error loading font!" << endl;
        }
        feedbackText.setFont(font);

        for (int i = 0; i < NUMBER_OF_GUESSES; i++) {
            boxRows[i] = new Boxes(150.0f + i * 80, font);
        }
        toUpperCase(targetWord);
    }

    ~Wordle() {
        for (int i = 0; i < NUMBER_OF_GUESSES; i++) {
            delete boxRows[i];
        }
    }

    void startGame(sf::RenderWindow& window) override {
        Game::startGame(window);
        currentTry = 0;
        targetWord = getRandomWord();
        cout << "Target Word: " << targetWord << endl;
        toUpperCase(targetWord);
        index = 0; currentTry = 0;
        for (int i = 0; i < NUMBER_OF_GUESSES; i++) {
            boxRows[i] = new Boxes(150.0f + i * 80, font);
        }
    }

    void handleInput(sf::RenderWindow& window, Player* player, string currentKey, InputSystem* inputSystem) {
        Game::handleInput(window, player, currentKey);
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

            // Draw the score and player name in the first row
            sf::Vector2u windowSize = window.getSize();
            float scoreX = windowSize.x - 10.f;

            bar.setSize(sf::Vector2f(window.getSize().x, 5));
            bar.setFillColor(sf::Color::Green);
            bar.setPosition(0, 45);

            scoreText.setFont(font);
            text.setFont(font);

            text.setString("Name: " + player->getName());
            scoreText.setString("Score: " + to_string(getScore()));

            scoreText.setCharacterSize(24);
            text.setCharacterSize(24);

            text.setFillColor(sf::Color::White);
            scoreText.setFillColor(sf::Color::White);

            text.setPosition(10, 8);
            scoreText.setPosition(scoreX - scoreText.getLocalBounds().width - 10, 8);  // Adjust to the left to fit the score text within the window

            window.draw(bar);
            window.draw(text);
            window.draw(scoreText);

            // Display feedback
            feedbackText.setString("Enter your guess:");
            feedbackText.setCharacterSize(24);
            feedbackText.setFillColor(sf::Color::White);
            feedbackText.setPosition(200, 100);
            window.draw(feedbackText);

            // Draw boxes
            for (int i = 0; i < NUMBER_OF_GUESSES; i++) {
                boxRows[i]->draw(window);
            }

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


    void gameLoop(sf::RenderWindow& window, Player* player, InputSystem* inputSystem) {
        sf::Clock clock;
        int keyPressCount = 0;

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
                        if (keyPressCount % 2 == 0) {
                            if (currentTry <= 4) {
                                char guessedChar = toupper(currentKey[0]);
                                
                                // Check if the row is complete
                                if (index < WORD_LENGTH ) {
                                    boxRows[currentTry]->setLetter(index, guessedChar);
                                    tries[currentTry] += guessedChar;
                                    index++;
                                }
                            }
                        }
                    }
                    else if (currentKey == "ENTER" && index == WORD_LENGTH)
                    {
                        keyPressCount++;
                        if (keyPressCount % 2 == 0) {
                            checkWord(tries[currentTry]);
                            index = 0;

                            // Check for win or loss
                            if (tries[currentTry] == targetWord) {
                                feedbackText.setString("You Win!");
                                setScore(getScore() + 10);
                                render(window, player);
                                sf::sleep(sf::seconds(1.5));

                                if (canGetNewWord()) {
                                    newWordCount++;
                                    resetGameState();
                                }
                                else {
                                    feedbackText.setString("Word Limit Reached. Try Again Later.");
                                    end = true;
                                }
                            }
                            else if (++currentTry == NUMBER_OF_GUESSES) {
                                feedbackText.setString("You Lose! Word: " + targetWord);
                                render(window, player);
                                sf::sleep(sf::seconds(1.5));
                                end = true;
                            }
                        }
                    }
                    else if (currentKey == "ENTER" && end) {
                        endGame();
                        return;
                    }
                }
            }
            render(window, player);
        }

    }


    string getRandomWord()  {
        srand(static_cast<unsigned>(time(0)));
        int index = rand() % 20;              // Random index between 0 and 19
        return WORD_LIST[index];
    }
    void toUpperCase(string& input) {
        for (char& c : input) c = toupper(c);
    }

    void checkWord(const string& input) {
        Boxes* currentRow = boxRows[currentTry];
        for (int i = 0; i < WORD_LENGTH; i++) {
            // check for exact match
            if (currentRow->letters2[i] == targetWord[i]) {
                boxRows[currentTry]->squares[i].setOutlineColor(sf::Color::Green);
            }
            // check if partial match
            else if (letterExists(currentRow->letters2[i])) {
                boxRows[currentTry]->squares[i].setOutlineColor(sf::Color::Yellow);
            }
            else {
                sf::Color darkGrey(105, 105, 105);
                boxRows[currentTry]->squares[i].setOutlineColor(darkGrey);
            }
            
        }

    }
    bool letterExists(char n)
    {
        for (int i = 0; i < 5; i++)
        {
            if (targetWord[i] == n)
                return true;
        }
        return false;
    }

    void resetGameState() {
        targetWord = getRandomWord();
        toUpperCase(targetWord);
        cout << "New Target Word: " << targetWord << endl;

        currentTry = 0;
        index = 0;

        for (int i = 0; i < NUMBER_OF_GUESSES; i++) {
            delete boxRows[i];
            boxRows[i] = new Boxes(150.0f + i * 80, font);
            tries[i] = "";
        }
        end = false;
    }

    bool canGetNewWord() {
        return newWordCount < NEW_WORD_LIMIT - 1;
    }

    // end game
    void endGame() override {
        currentPlayer->setHighscore(2, getScore());
        Game::endGame();
    }
};
