#include "GameBoy.h"
#include "Leaderboard.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "Screen.h"

int main() {
	Screen screen(800, 650);
	Leaderboard leaderBoard;
	InputSystem inputSystem;

	SoundSystem soundSystem(10);
	// Load sounds
	if (!soundSystem.loadBackgroundMusic("./aud/music.mp3")) {
		std::cerr << "Failed to load background music." << std::endl;
		return -1;
	}
	// Load sounds
	if (!soundSystem.loadSound("correct", "./aud/correct.mp3")) {
		std::cerr << "Failed to load 'correct' sound." << std::endl;
	}
	if (!soundSystem.loadSound("lose", "./aud/loose.mp3")) {
		std::cerr << "Failed to load 'lose' sound." << std::endl;
	}
	if (!soundSystem.loadSound("success", "./aud/success.mp3")) {
		std::cerr << "Failed to load 'success' sound." << std::endl;
	}
	if (!soundSystem.loadSound("wrong", "./aud/wrong.mp3")) {
		std::cerr << "Failed to load 'wrong' sound." << std::endl;
	}
	// these are all aggregated into gameboy
	GameBoy gameboy(&screen, &inputSystem, &leaderBoard, &soundSystem);

	gameboy.start();
}
