class Boxes {
public:
    sf::Text letters[5];
    char letters2[5];
    static const int BOX_SIZE = 60;
    static const int BOX_PADDING = 10;
    sf::RectangleShape squares[5];

    Boxes(float yOffset, sf::Font& font) {
        float xOffset = 200.0f;
        for (int i = 0; i < 5; i++) {
            squares[i].setSize(sf::Vector2f(BOX_SIZE, BOX_SIZE));
            squares[i].setPosition(xOffset, yOffset);
            squares[i].setOutlineThickness(3);
            squares[i].setOutlineColor(sf::Color::White);
            squares[i].setFillColor(sf::Color::Black);

            // Initialize text for each box
            letters[i].setFont(font);
            letters[i].setCharacterSize(40);
            letters[i].setFillColor(sf::Color::White);
            letters[i].setString("");
            letters[i].setPosition(xOffset + 15, yOffset + 5);

            xOffset += BOX_SIZE + BOX_PADDING;
        }
    }

    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < 5; i++) {
            window.draw(squares[i]);
            window.draw(letters[i]);
        }
    }

    void setLetter(int index, char letter) {
        if (index >= 0 && index < 5) {
            letters[index].setString(string(1, letter));
            letters2[index] = letter;
        }
    }
};
