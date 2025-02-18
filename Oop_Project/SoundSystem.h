#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>

class SoundSystem {
private:
    sf::Music backgroundMusic;
    struct SoundEntry {
        sf::SoundBuffer* buffer;
        sf::Sound* sound;
        std::string name;
    };

    SoundEntry* sounds;
    int capacity;
    int count;

public:
    SoundSystem(int maxSounds)
        : capacity(maxSounds), count(0) {
        sounds = new SoundEntry[capacity];
        for (int i = 0; i < capacity; i++) {
            sounds[i].buffer = nullptr;
            sounds[i].sound = nullptr;
            sounds[i].name = "";
        }
    }

    ~SoundSystem() {
        for (int i = 0; i < count; i++) {
            delete sounds[i].buffer;
            delete sounds[i].sound;
        }
        delete[] sounds;
    }

    bool loadBackgroundMusic(const std::string& filePath) {
        if (!backgroundMusic.openFromFile(filePath)) {
            std::cerr << "Error loading background music: " << filePath << std::endl;
            return false;
        }
        backgroundMusic.setLoop(true);  // loop
        backgroundMusic.setVolume(10);
        return true;
    }

    bool loadSound(const std::string& name, const std::string& filePath) {
        if (count >= capacity) {
            std::cerr << "SoundSystem capacity reached, cannot load more sounds." << std::endl;
            return false;
        }

        sf::SoundBuffer* buffer = new sf::SoundBuffer();
        if (!buffer->loadFromFile(filePath)) {
            std::cerr << "Error loading sound: " << filePath << std::endl;
            delete buffer;
            return false;
        }

        sf::Sound* sound = new sf::Sound();
        sound->setBuffer(*buffer);

        sounds[count].buffer = buffer;
        sounds[count].sound = sound;
        sounds[count].name = name;

        count++;
        return true;
    }

    void playBackgroundMusic() {
        backgroundMusic.setLoop(true);
        backgroundMusic.play();
    }

    void stopBackgroundMusic() {
        backgroundMusic.stop();
    }

    void playSound(const std::string& name) {
        for (int i = 0; i < count; i++) {
            if (sounds[i].name == name) {
                sounds[i].sound->play();
                return;
            }
        }
        std::cerr << "Sound not found: " << name << std::endl;
    }

    // Stop a specific sound
    void stopSound(const std::string& name) {
        for (int i = 0; i < count; i++) {
            if (sounds[i].name == name) {
                sounds[i].sound->stop();
                return;
            }
        }
        std::cerr << "Sound not found: " << name << std::endl;
    }

    // Stop all sounds
    void stopAllSounds() {
        for (int i = 0; i < count; i++) {
            if (sounds[i].sound) {
                sounds[i].sound->stop();
            }
        }
    }
};
