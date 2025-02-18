#pragma once
#include "Player.h"
#include <iostream>
using namespace std;

struct Score {
    string name;
    int scores[3]; //scores for each game for thi player
};

class Leaderboard {
private:
    Score highScores[3];     // top scores for each player  
    Player* topPlayers[3]; 
    string games[3] = {"SnakeGame", "Hangman", "Wordle"};
    int index = 0;
public:
    // Constructor
    Leaderboard() {
        for (int i = 0; i < 3; i++) {
            highScores[i].name = "";
            highScores[i].scores[i] = 0;
            topPlayers[i] = nullptr; // Initialize pointers to null
        }
    }

    void updateScore(Player* player, int gameIndex, int score) {
        if (gameIndex < 0 || gameIndex >= 3) {
            cout << "Invalid game index." << endl;
            return;
        }

        if (score > highScores[gameIndex].scores[gameIndex]) {
            highScores[gameIndex].scores[gameIndex] = score;
            highScores[gameIndex].name = player->getName();
            topPlayers[gameIndex] = player;
        }
    }

    void addPlayer(Player* player) {
        for (int gameIndex = 0; gameIndex < 3; gameIndex++) {
            // update score for each
            int playerScore = player->getHighscore(gameIndex);
            updateScore(player, gameIndex, playerScore);
        }
    }

    void displayTopScores() {
        cout << "Leaderboard:" << endl;
        for (int i = 0; i < 3; i++) {
            if (topPlayers[i]) {
                cout << games[i] << " - " << highScores[i].name
                    << ": " << highScores[i].scores[i] << endl;
            }
            else {
                cout << games[i] << " - No top player yet." << endl;
            }
        }
    }

    void resetLeaderboard() {
        for (int i = 0; i < 3; i++) {
            highScores[i].scores[i] = 0;
            highScores[i].name = "";
            topPlayers[i] = nullptr;
        }
    }

    void saveLeaderboard() {
        cout << "Saving leaderboard data..." << endl;
        // Implementation of saving to a file would go here
    }

    void loadLeaderboard() {
        cout << "Loading leaderboard data..." << endl;
        // Implementation of loading from a file would go here
    }

    Score* getHighScores() {
        return highScores;
    }
};
