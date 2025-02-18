#pragma once
#include <SFML/Window.hpp>   // For handling keyboard input
#include <iostream>
using namespace std;

const int KEYS = 40;

// InputSystem class. this is used to get user input from keyboard
class InputSystem {
private:
    sf::Keyboard::Key keyMappings[KEYS];           // Array of keys
    bool keyStates[KEYS];
    int mappedKeyCount;                        // Count of currently mapped keys
    string keyLabels[KEYS];
    string userInput = "";

public:
    // hardcode the main keys to be used in the construcor
    InputSystem() : mappedKeyCount(0) {
        // keysmappings and key states
        // Map numbers (1 to 4)
        for (int i = 0; i < 5; ++i) {
            mapKey(static_cast<sf::Keyboard::Key>(sf::Keyboard::Num1 + i), std::string(1, '1' + i), mappedKeyCount++);
        }

        // Map letters (A to Z)
        for (int i = 0; i < 26; ++i) {
            mapKey(static_cast<sf::Keyboard::Key>(sf::Keyboard::A + i), std::string(1, 'a' + i), mappedKeyCount++);
        }

        // Map arrow keys
        mapKey(sf::Keyboard::Up, "UP", mappedKeyCount++);
        mapKey(sf::Keyboard::Down, "DW", mappedKeyCount++);
        mapKey(sf::Keyboard::Left, "LF", mappedKeyCount++);
        mapKey(sf::Keyboard::Right, "RT", mappedKeyCount++);

        //  Escape key
        mapKey(sf::Keyboard::Escape, "ESC", mappedKeyCount++);

        // enter and backspace space and tab

        mapKey(sf::Keyboard::Backspace, "BACK", mappedKeyCount++);
        mapKey(sf::Keyboard::Enter, "ENTER", mappedKeyCount++);
        mapKey(sf::Keyboard::Space, "SPACE", mappedKeyCount++);
        mapKey(sf::Keyboard::Tab, "TAB", mappedKeyCount++);
    }

    // Map a key to the keyMappings array. store the label as well for future use
    void mapKey(sf::Keyboard::Key key, string label, int index) {
        keyMappings[index] = key;
        keyLabels[index] = label;
        keyStates[index] = false;
    }

    // Handle input by checking if one of the mapped keys is pressed by the user
    void handleInput() {
        for (int i = 0; i < mappedKeyCount; i++) {
            if (sf::Keyboard::isKeyPressed(keyMappings[i])) {
                if (!keyStates[i]) {
                    keyStates[i] = true;
                }
            }
            else {
                if (keyStates[i]) {
                    keyStates[i] = false;
                }
            }
        }
    }
    // get the pressed key
    string getCurrentPressedKey() const {
        for (int i = 0; i < KEYS; i++) {
            if (keyStates[i]) {
                return keyLabels[i];  
            }
        }
        return "";
    }

    // check if ANY key is pressed
    bool isAnyKeyPressed()
    {
        for (int k = -1; k < sf::Keyboard::KeyCount; ++k)
        {
            if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
                return true;
        }
        return false;
    }

    // get user name and return it
    string getUserInput(sf::Event& event) {
        if (event.type == sf::Event::TextEntered) {
            if ((event.text.unicode >= 'A' && event.text.unicode <= 'Z') ||
                (event.text.unicode >= 'a' && event.text.unicode <= 'z') ||
                (event.text.unicode >= '1' && event.text.unicode <= '9' || event.text.unicode == ' ')) {
                userInput += static_cast<char>(event.text.unicode);
            }
        }
        return userInput;
    }
    void clearUserInput() {
        userInput = "";  
    }

    // Check if the current key press is an alphabet
    bool isAlphabetPressed() const {
        for (int i = 0; i < KEYS; i++) {
            if (keyStates[i] && (keyLabels[i][0] >= 'a' && keyLabels[i][0] <= 'z')) {
                return true;  // The key pressed is an alphabet
            }
        }
        return false;
    }
};