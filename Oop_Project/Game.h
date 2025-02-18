#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
using namespace std;

#include "Leaderboard.h"
#include "Player.h"

// the main game class, from whicht he other games inherit

class Game {
private:
    string name;         
    int score = 0;  
    Leaderboard* leaderboard; // Aggregated
    bool playing = false;
    sf::Font font;       
    sf::Text text;     

    sf::Sprite img;

protected:
    bool isMenuActive = true; 
    Player* currentPlayer = nullptr;

public:
    Game(Leaderboard* lptr) : leaderboard(lptr) {
        currentPlayer = nullptr;
        if (!font.loadFromFile("./fonts/arial.ttf")) {
            cerr << "Error loading font!" << endl;
        }
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
    }

    virtual ~Game() = default;

    void setName(const string& gameName) { name = gameName; }
    string getName() const { return name; }

    int getScore() const { return score; }
    void setScore(int n) { score = n; }

    // Render the shared menu
    virtual void render(sf::RenderWindow& window, Player* player) {
        currentPlayer = player;
        window.clear();

        text.setString("Welcome to \"" + name + "\"");
        text.setPosition(10, 100);
        window.draw(text);

        text.setString("Press Enter to Start");
        text.setPosition(10, 150);
        window.draw(text);

        text.setString("Press Backspace to Return to Main Menu");
        text.setPosition(10, 200);
        window.draw(text);

        text.setString("Press ESC to Exit Gameboy");
        text.setPosition(10, 250);
        window.draw(text);

        window.display();
    }

    // Start the game
    virtual void startGame(sf::RenderWindow& window) {
        score = 0;
        window.clear();
        sf::Texture imgTexture;
        if (name == "SnakeGame" && !imgTexture.loadFromFile("./img/snake_start.png"))
        {
            cerr << "Error loading image!" << endl;
            return;
        }
        else if (name == "Hangman" && !imgTexture.loadFromFile("./img/hangman_start.png"))
        {
            cerr << "Error loading image!" << endl;
            return;
        }
        else if (name == "Wordle" && !imgTexture.loadFromFile("./img/wordle_start.png"))
        {
            cerr << "Error loading image!" << endl;
            return;
        }
        img.setTexture(imgTexture);
        sf::Vector2u textureSize = imgTexture.getSize();

        float scaleX = static_cast<float>(window.getSize().x) / textureSize.x;
        float scaleY = static_cast<float>(window.getSize().y) / textureSize.y;
        img.setScale(scaleX, scaleY);
        window.draw(img);
        window.display();
    }

    // inputs are passed from Gameboy to here
    virtual void handleInput(sf::RenderWindow& window, Player* player, string currentKey)
    {
        if (currentKey == "ENTER") {
            isMenuActive = false;
        }
    }

    // End the game
    virtual void endGame() {
        cout << "Ending game: " << name << endl;
        leaderboard->addPlayer(currentPlayer);
        score = 0;
        isMenuActive = true;
        //leaderboard->displayTopScores();
    }


};
