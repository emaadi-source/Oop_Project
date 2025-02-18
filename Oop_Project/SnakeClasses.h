#pragma once

//  SnakeClasses which has:
    // -> SnakeSegment
    // -> Point (food)

// The snake segment
class SnakeSegment {
    sf::Sprite sprite;
    float position[2];  // x, y
    float dimensions[2] = { 30,30 };

public:
    SnakeSegment() {};

    SnakeSegment(sf::Texture& texture, const float pos[2]) {
        sprite.setTexture(texture);
        position[0] = pos[0];
        position[1] = pos[1];
        sprite.setPosition(position[0], position[1]);

    }

    void setPosition(float x, float y) {
        position[0] = x;
        position[1] = y;
        sprite.setPosition(position[0], position[1]);
    }

    void setRotation(float angle) {
        sprite.setRotation(angle);
    }

    void setOrigin(int x, int y) {
        sprite.setOrigin(x, y);
    }


    const float* getPosition() const {
        return position;
    }

    sf::Vector2f getPositionAsVector() const {
        return sf::Vector2f(position[0], position[1]);
    }

    const float* getDimensions() {
        return dimensions;
    }
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};


// SNAKE food

class Point {
    sf::CircleShape pointShape;
    float position[2] = { 0, 0 };  // x, y

public:
    Point(int radius) {
        pointShape.setRadius(radius);
        pointShape.setFillColor(sf::Color::Red);
        pointShape.setPosition(position[0], position[1]);
    }

    const float* getPosition() const {
        return position;
    }

    void setPosition(float x, float y) {
        position[0] = x;
        position[1] = y;
        pointShape.setPosition(position[0], position[1]);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(pointShape);
    }
    int getRadius() const
    {
        return pointShape.getRadius();
    }
};
