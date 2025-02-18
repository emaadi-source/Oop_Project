/*

#include<iostream>
#include<cstring>
#include"Engine.h"
using namespace std;



#include <SFML/Graphics.hpp>   // For rendering shapes, sprites, text, etc.
#include <SFML/Window.hpp>     // For managing window and user input.
#include <SFML/System.hpp>     // For timing and other utilities.
#include <SFML/Audio.hpp>      // For music and sound effects.

void Engine::StartGame() {
	window.create(sf::VideoMode(800, 600), "Hangman");

	arial.loadFromFile("D:/Oop_Project/Oop_Project/fonts/arial.ttf");
	mistake_texture.loadFromFile("D:/Oop_Project/Oop_Project/img/mistake_zero.png");
	mistake_sprite.setTexture(mistake_texture);
	mistake_sprite.setScale(sf::Vector2f(1.45, 1.45));

	word_index = rand() % 10;
	word_len = strlen(words[word_index]) - 1;

	word.setFont(arial);
	word.setCharacterSize(70);
	word.setPosition(365, 430);
	word.setFillColor(sf::Color::Black);

	// Load sound files
	if (!correctBuffer.loadFromFile("D:/Oop_Project/Oop_Project/aud/correct.mp3") ||
		!wrongBuffer.loadFromFile("D:/Oop_Project/Oop_Project/aud/wrong.mp3") ||
		!successBuffer.loadFromFile("D:/Oop_Project/Oop_Project/aud/success.mp3") ||
		!looseBuffer.loadFromFile("D:/Oop_Project/Oop_Project/aud/loose.mp3")) {
		std::cerr << "Error loading sound files!" << std::endl;
		return;
	}

	// Set sound buffers to sound objects
	correctSound.setBuffer(correctBuffer);
	wrongSound.setBuffer(wrongBuffer);
	successSound.setBuffer(successBuffer);
	looseSound.setBuffer(looseBuffer);

	std::string displayWord = "";
	for (unsigned short i = 0; i < word_len + 1; i++) {
		if (words[word_index][i] == words[word_index][0] || words[word_index][i] == words[word_index][word_len]) {
			_word[i] = words[word_index][i]; // Pre-fill first and last letters
		}
		else {
			_word[i] = '_'; // Underscores for unguessed letters
		}

		if (_word[i] != '_') {
			displayWord += _word[i];
		}
		else {
			displayWord += "_";
		}

		if (i != word_len) {
			displayWord += " ";
		}
	}
	_word[word_len + 1] = '\0'; // Null-terminate the array
	word_string = displayWord;
	word.setString(word_string);

	// Initialize cross textures and number texts
	cross_texture.loadFromFile("D:/Oop_Project/Oop_Project/img/cross.png");
	for (int i = 0; i < 4; ++i) {
		crosses[i].setTexture(cross_texture);
		crosses[i].setScale(0.15f, 0.15f);  // Tiny size for crosses
		crosses[i].setPosition(720.f, 50.f + (i * 70)); // Position crosses

		cross_numbers[i].setFont(arial); // Same font for numbers
		cross_numbers[i].setCharacterSize(20); // Smaller size for numbers
		cross_numbers[i].setFillColor(sf::Color::Black); // Color of numbers
		cross_numbers[i].setString("-" + std::to_string(i + 1)); // Set numbers
		cross_numbers[i].setPosition(680.f, 55.f + (i * 70)); // Position left of crosses
	}

	// Initialize "Total Lives" text
	totalLivesText.setFont(arial);
	totalLivesText.setCharacterSize(20);  // Set character size for the text
	totalLivesText.setFillColor(sf::Color::Black);  // Set text color
	totalLivesText.setString("Total Lives: 4");  // Set initial total lives text
	totalLivesText.setPosition(600.f, 10.f);  // Set position at the top right

	while (window.isOpen()) {
		HandleEvent();

		window.clear(sf::Color::White);

		window.draw(mistake_sprite);
		window.draw(word);

		// Draw crosses and numbers based on number of mistakes
		for (int i = 0; i < mistakes; ++i) {
			window.draw(crosses[i]);
			window.draw(cross_numbers[i]);
		}

		// Draw the "Total Lives" text
		window.draw(totalLivesText);

		window.display();
	}
}







bool Engine::wordfound() {

	for (int i = 0; i < word_len + 1; i++) {

		if (_word[i] != words[word_index][i])
			return false;
	}
	return true;
}

void Engine::AddLettertoWord(unsigned int key) {
	bool letter_found = false;
	char key_char = key + 65; // Convert key to uppercase letter

	// Update the guessed word (_word) array
	for (int i = 0; i < word_len + 1; i++) {
		if (words[word_index][i] == key_char && _word[i] == '_') { // Check if letter matches and is not already guessed
			_word[i] = key_char;
			letter_found = true;
		}
	}

	// Rebuild the word string with spaces between characters
	std::string updatedWord = "";
	for (int i = 0; i < word_len + 1; i++) {
		if (_word[i] != '_') {
			updatedWord += _word[i]; // Add the correctly guessed letter
		}
		else {
			updatedWord += "_"; // Keep underscores for unguessed letters
		}

		if (i != word_len) {
			updatedWord += " "; // Add space between characters
		}
	}
	word_string = updatedWord; // Update the displayed word

	if (letter_found) {
		correctSound.play();  // Play sound for correct letter

		if (wordfound()) { // Check if the entire word is guessed
			word.setCharacterSize(30);
			word_string = "You've found the word!";
			word.setString(word_string);
			successSound.play();  // Play success sound only after winning
		}
		else {
			word.setString(word_string);
		}
	}
	else {
		wrongSound.play();  // Play sound for wrong letter

		// Handle incorrect guess
		mistake_texture.loadFromFile(sf::String("D:/Oop_Project/Oop_Project/img/") + sf::String(mistakes_img[++mistakes]));

		// Update total lives text
		int remainingLives = 4 - mistakes;
		totalLivesText.setString("Total Lives: " + std::to_string(remainingLives));

		// Check if maximum mistakes are reached
		if (mistakes == 4) {
			looseSound.play();  // Play losing sound
			word_string = "The word was " + sf::String(words[word_index]);
			word.setCharacterSize(30);
			word.setString(word_string);
		}
	}
}



void Engine::HandleEvent()
{
	while (window.pollEvent(ev)) {

		switch (ev.type)
		{
		case sf::Event::Closed:
		{
			window.close();
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			if (ev.mouseButton.button == sf::Mouse::Left) {

				int x = sf::Mouse::getPosition(window).x;
				int y = sf::Mouse::getPosition(window).y;
				cout << x << ' ' << y << '\n';

				word.setPosition(sf::Vector2f(x, y));

			}

		}

		case sf::Event::KeyPressed:
		{
			const sf::Keyboard::Key key_code = ev.key.code;

			if (key_code >= sf::Keyboard::A && key_code <= sf::Keyboard::Z)
			{
				AddLettertoWord(key_code);
			}
		}

		default:
			break;

		}
	}
}
*/