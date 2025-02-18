#pragma once
#include <string>
#include "Screen.h" 
#include "InputSystem.h" 
#include "SoundSystem.h" 
#include "Menu.h"  
#include "SnakeGame.h"  
#include "HangMan.h"
#include "Wordle.h"
#include "Player.h"

using namespace std;
// the main Gameboy class.


class GameBoy {
private:
    Screen* screen;             // Aggregated
    InputSystem* inputSystem;   // Aggregated
    SoundSystem* soundSystem;   // Aggregated
    Menu menu;                  // Composed
    SnakeGame snakeGame;        // Composed
    Hangman hangman;            // Composed
    Wordle wordle;              // Composed        
    Player player;              // Composed 

    Leaderboard* leaderBoard;   // only for display purposes

    bool gameStarted = false; // check if user has selecred a game
    bool gameRunning = false; // check if user playing
    string name = "";
    string currentGame = "";
public:
    GameBoy(Screen* screenPtr, InputSystem* inputPtr, Leaderboard* lptr, SoundSystem* sptr) : screen(screenPtr),
        inputSystem(inputPtr), soundSystem(sptr), snakeGame(lptr, screen->getWindow()), hangman(lptr),
        leaderBoard(lptr), wordle(lptr) {}

    // this is called in Main.cpp
    void start() {
        sf::RenderWindow& window = screen->getWindow();
        sf::Event event_temp;
        screen->render(menu, name);
        soundSystem->playBackgroundMusic();
        // Load and play background music
        if (!soundSystem->loadBackgroundMusic("./aud/music.mp3")) {
            std::cerr << "Failed to load background music." << std::endl;
        }
        else {
            soundSystem->playBackgroundMusic();
        }

        while (window.isOpen()) {
            sf::Event event;

            string currentKey = "";

            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    shutdown(); // Handle window closure
                }

                inputSystem->handleInput();
                // Check if any key is pressed 
                if (inputSystem->isAnyKeyPressed())
                {
                    currentKey = inputSystem->getCurrentPressedKey();
                }
                else {
                    currentKey = "";
                }

                // get user Name if it is the name menu
                if (menu.getMenuState() == "Name" && inputSystem->isAnyKeyPressed()) {
                    name = inputSystem->getUserInput(event);
                    screen->render(menu, name);
                }
            }

            int option = currentKey[0] - '0';
            // handle main navigation keys
            if (currentKey == "ENTER") {
                handleEnterKey(name, window);
            }
            else if (currentKey == "BACK") {
                handleBackKey(name);
            }

            // handle game selection
            else if (currentKey[0] >= '1' && currentKey[0] < '4' && menu.getMenuState() == "Options" && !gameStarted) {
                gameStarted = true;
                switchGame(menu.getGame(option - 1), inputSystem);
            }

            // handle leaderboard and profile show
            else if (currentKey[0] == '4' && menu.getMenuState() == "Options" && !gameStarted) {
                menu.navigate(3); // Navigate to the Profile menu
                currentKey = "";
            }

            else if (currentKey[0] == '5' && menu.getMenuState() == "Options" && !gameStarted) {
                menu.navigate(4); // Navigate to the Leaderboard menu
                currentKey = "";
            }

            // handle game selection and start
            if (gameStarted && !gameRunning)
            {
                if (menu.getGame(option - 1) == "SnakeGame")
                {
                    currentGame = "Snake";
                    snakeGame.startGame(window);
                }
                else if(menu.getGame(option - 1) == "Hangman")
                {
                    currentGame = "Hangman";
                    hangman.startGame(window);
                }
                else if (menu.getGame(option - 1) == "Wordle")
                {
                    currentGame = "Wordle";
                    wordle.startGame(window);
                }

            }
            else if (!gameRunning)
            {
                if (menu.getMenuState() == "Leaderboard")
                {
                    screen->render(menu, leaderBoard);
                }
            }

            if (currentKey == "ESC") {
                
                shutdown();
            }
        }
    }

    void handleEnterKey(string name, sf::RenderWindow& window) {
        string currentState = menu.getMenuState();
        if (!gameStarted)
        {
            if (currentState == "Start") {
                menu.navigate(1); // Move to the Name state
                this->name = "";
                inputSystem->clearUserInput();
                screen->render(menu, name);
            }
            else if (currentState == "Name" && name != "") {
                menu.navigate(2); // Move to the Options state
                screen->render(menu, name);
            }
        }
        else {
            gameRunning = true;
            if (currentGame == "Snake")
            {
                snakeGame.handleInput(window, &player, "ENTER", inputSystem);
                soundSystem->stopBackgroundMusic();
            }
            else if (currentGame == "Hangman")
            {
                hangman.handleInput(window, &player, "ENTER", inputSystem, soundSystem);
                soundSystem->stopBackgroundMusic();
            }
            else if (currentGame == "Wordle")
            {
                wordle.handleInput(window, &player, "ENTER", inputSystem);
                soundSystem->stopBackgroundMusic();
            }
            // return back to options menu
            gameStarted = false; gameRunning = false;
            screen->render(menu, name);
            leaderBoard->displayTopScores();
            soundSystem->playBackgroundMusic();
        }
    }

    void handleBackKey(string name) {
        string currentState = menu.getMenuState();
        inputSystem->clearUserInput();
        if (gameStarted)
        {
            menu.navigate(0);
            gameStarted = false;
            screen->render(menu, name);
        }
        else if (currentState == "Options" || currentState == "Name" && !gameRunning) {
            if (currentState != "Start") {
                menu.navigate(0);
                this->name = "";
                screen->render(menu, name);
            }
        }
        else if (currentState == "Leaderboard" || currentState == "Options")
        {
            menu.navigate(2);
            screen->render(menu, name);
        }
    }

    // choose a game based on user input
    void switchGame(const string& gameName, InputSystem* inputSystem) {
        player.setName(name);
        soundSystem->stopBackgroundMusic();
        if (gameName == "SnakeGame") {
            cout << "Launching SnakeGame..." << endl;
            screen->render(snakeGame, &player);
        }
        else if (gameName == "Hangman") {
            cout << "Launching Hangman..." << endl;
            screen->render(hangman, &player);
        }
        else if (gameName == "Wordle") {
            cout << "Launching Wordle..." << endl;
            screen->render(wordle, &player);
        }
    }


    void shutdown() {
        cout << "Shutting down GameBoy..." << endl;
        sf::RenderWindow& window = screen->getWindow();
        if (window.isOpen()) {
            window.close();
        }
    }
};
