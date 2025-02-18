#pragma once 
#include <string>
using namespace std;

class Achievement {};  

const int games = 3;

class Player {
private:
    string name;
    Achievement achievement[games];
    int highScores[games] = { 0, 0 ,0 }; // a highscore for each game
    string gameState = "";

public:

    string getName() const { return name; }
    void setName(const string& playerName) { 
        name = playerName;
    }

    int getHighscore(int gameIndex) const {
        if (gameIndex >= 0 && gameIndex < games) {
            return highScores[gameIndex];
        }
        return -1;
    }

    void setHighscore(int gameIndex, int score) {
        if (gameIndex >= 0 && gameIndex < games) {
            highScores[gameIndex] = score;
        }
    }

    void saveProgress();
    void loadProgress();
};
