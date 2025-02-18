#pragma once
#include <SFML/Graphics.hpp>   // For rendering shapes, sprites, text, etc.
#include <SFML/Window.hpp>     // For managing window and user input.
#include <SFML/System.hpp>     // For timing and other utilities.
#include <SFML/Audio.hpp>      // For music and sound effects.

#include <iostream>            // For console output.
#include <string>              // For handling strings.
#include <ctime>               // For random seed generation.
#include <cstdlib>             // For generating random numbers.

class Engine {

public:

	void StartGame();
	void HandleEvent();
	void AddLettertoWord(unsigned int key);
	bool wordfound();

private:
	sf::RenderWindow window;
	sf::Event ev;
	sf::Sprite mistake_sprite;
	sf::Texture mistake_texture;

	sf::Text word;
	sf::Font arial;

	sf::String word_string;
	sf::Texture cross_texture;
	sf::Sprite crosses[4]; 
	sf::Text cross_numbers[4];
	sf::Text totalLivesText;

	sf::Clock animationClock;  // For tracking animation time
	sf::Text animatedText;     // Text that will animate
	sf::Vector2f animationStartPos;  // Starting position of animation
	sf::Vector2f animationEndPos;    // End position for final text
	float animationRadius = 100.f;  // Radius of circular movement (adjustable)
	float animationSpeed = 200.f;   // Speed of the animation (adjustable)
	bool isAnimating = false;       // Whether the animation is active


	//for_sound 
	sf::SoundBuffer correctBuffer, wrongBuffer, successBuffer, looseBuffer;
	sf::Sound correctSound, wrongSound, successSound, looseSound;

	//truimph effect 
	sf::RectangleShape ribbons[20];   // Static array for ribbons
	sf::Vector2f ribbonVelocities[20]; // Velocity for each ribbon
	bool showTriumph = false;         // Flag for ribbon animation
	sf::Clock triumphTimer;           // Timer for the animation
	int ribbonCount = 20;             // Number of ribbons




	const char words[10][100] = {"PLAIN", "PROGRAM", "SFML", "CODE", "TRIAD", "WRIST", "HANDS", "FACE", "JANUARY", "OCTOPUS"};
	const char mistakes_img[5][20] = { "mistakes_zero.png", "mistake_one.png", "mistake_two.png", "mistake_three.png", "mistake_four.png" };
	char _word[100];
	unsigned short word_index;
	unsigned short word_len;
	unsigned short mistakes = 0;


};

