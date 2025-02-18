#pragma once

#include <string>
#include <iostream>
using namespace std;

class Menu {
private:
    string menuOptions[5] = {"Start", "Name", "Options", "Profile", "Leaderboard"};
    string games[3] = { "SnakeGame", "Hangman", "Wordle" };
    // current main menu state
    string menuState;
    int index = 0;

public:
    Menu():menuState(menuOptions[0]) {};
    Menu(int n){
        menuState = n;
    };
    // chose options based on keys pressed
    void navigate(int option) {
        if (option >= 0 && option < 5) {
            menuState = menuOptions[option];
            index = option;
        }
    }

    void transtion(string option)
    {

    };

    string getMenuState() const
    {
        return menuState;
    }
    int getIndex() const {
        return index;
    }

    string getGame(int index) {
        if (index >= 0 && index < 3) {
            return games[index];
        }
        return "";
    }
};
