#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Menu.h"
#include "Game.h"
#include "InputSystem.h"

using namespace std;

// in this class, the rendering is mostly handled. render functions are called for the menu or a game

class Screen {
private:
    int resolutionX, resolutionY;
    sf::RenderWindow window;       // SFML window for rendering
    sf::Font font;                 // Font for rendering text
    sf::Text displayText;          // Text to display
    sf::Clock cursorTimer;

    sf::Sprite start;
    string games[3] = { "Snake", "Hangman", "Wordle" };
public:
    Screen(int x, int y) : resolutionX(x), resolutionY(y), window(sf::VideoMode(x, y), "GameBoy Screen") {
        // Load font for text rendering
        if (!font.loadFromFile("./fonts/arial.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
        }

        displayText.setFont(font);
        displayText.setCharacterSize(24);
        displayText.setFillColor(sf::Color::White);
        displayText.setPosition(10, 10);
    }

    // Render the menu. there are different menu states
    void render(Menu menu, string name) {
        window.clear(sf::Color::Black);

        // Render based on menu state
        string state = menu.getMenuState();

        if (state == "Start") {
            renderStartMenu();
        }
        else if (state == "Name") {
            renderNameInput(name);
        }
        else if (state == "Options") {
            renderOptionsMenu(name);
        }

        window.display();
    }

    void render(Menu menu, Leaderboard* leaderBoard) {
        window.clear(sf::Color::Black);

        // Render based on menu state
        string state = menu.getMenuState();
        if (state == "Leaderboard") {
            renderLeaderboard(leaderBoard);
        }

        window.display();
    }

    // Render the game
    void render(Game& game, Player* player) {
        window.clear();
        game.render(window, player); // pass the window to the game render function
    }

    sf::RenderWindow& getWindow() {
        return window;
    }

    // the different menus
    void renderStartMenu() {
        sf::Texture startTexture;
        if (!startTexture.loadFromFile("./img/start.png"))
        {
            cerr << "Error loading image!" << endl;
            return;
        }

        start.setTexture(startTexture);
        sf::Vector2u textureSize = startTexture.getSize();

        float scaleX = static_cast<float>(window.getSize().x) / textureSize.x;
        float scaleY = static_cast<float>(window.getSize().y) / textureSize.y;
        start.setScale(scaleX, scaleY);

        window.draw(start);

    }

    void renderNameInput(string name) {
        displayText.setString("Enter Player Name: ");
        displayText.setCharacterSize(28);
        displayText.setPosition(11, 50);
        window.draw(displayText);
        
        // get player name
        sf::Text userInputText("", font, 24);
        userInputText.setFillColor(sf::Color::White);
        userInputText.setPosition(11, 100);
        userInputText.setString(name);

        // Draw the user input text
        window.draw(userInputText);

        // Cursor and bar to indicate typing position
        sf::RectangleShape cursor(sf::Vector2f(2, userInputText.getCharacterSize() + 2));
        sf::RectangleShape bar(sf::Vector2f(250, 2));
        bar.setFillColor(sf::Color::White);
        cursor.setFillColor(sf::Color::Yellow);

        float cursorX = userInputText.getPosition().x + userInputText.getLocalBounds().width + 5;
        float cursorY = userInputText.getPosition().y + 2;
        cursor.setPosition(cursorX, cursorY);
        bar.setPosition(11, 108 + userInputText.getCharacterSize());

        window.draw(bar);
        window.draw(cursor);
    }

    void renderOptionsMenu(string name) {
        window.clear();
        displayText.setString("Welcome " + name + "!\nPlease Select an Option : \n");
        displayText.setPosition(10, 10);
        window.draw(displayText);

        displayText.setString("1. SnakeGame\n2. Hangman\n3. Wordle\n4. Profile\n5.Leaderboard\nEsc. Exit");
        displayText.setPosition(10, 100);
        window.draw(displayText);
    }

    // Function to render the leaderboard
    void renderLeaderboard(Leaderboard* leaderboard) {
        window.clear();

        displayText.setString("Leaderboard:");
        displayText.setCharacterSize(28);
        displayText.setPosition(10, 10);
        window.draw(displayText);

        Score* highScores = leaderboard->getHighScores();

        for (int i = 0; i < 3; i++) {
            displayText.setString(games[i] + " - " + highScores[i].name + ": " + to_string(highScores[i].scores[i]));
            displayText.setPosition(10, 50 + i * 40);
            window.draw(displayText);
        }

    }
};
