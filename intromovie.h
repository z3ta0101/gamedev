#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class Button {
public:
    Button(float x, float y, float width, float height, const std::string& text, sf::Font& font, const std::string& texturePath, std::function<void()> onClick) 
        : onClick(onClick) {
        // Set up button size and position
        button.setSize(sf::Vector2f(width, height));
        button.setPosition(x, y);

        // Load texture and apply it to the button
        if (!buttonTexture.loadFromFile(texturePath)) {
            std::cerr << "Error loading button texture from " << texturePath << std::endl;
        } else {
            button.setTexture(&buttonTexture);
        }

        // Set up button text
        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(42);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(
            x + (width - buttonText.getLocalBounds().width) / 2 - 4,
            y + (height - buttonText.getLocalBounds().height) / 2 - 12
        );
    }

    void draw(sf::RenderWindow& window) {
        window.draw(button);     // Draw the button texture
        window.draw(buttonText); // Draw the button text
    }

    void checkClick(sf::Event& event, sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed) {
            // Get the mouse position in window coordinates
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

            // Check if the mouse is within the button's bounds
            if (button.getGlobalBounds().contains(worldMousePos)) {
                onClick();  // Trigger the onClick function if clicked
            }
        }
    }

    void checkHover(sf::RenderWindow& window) {
        // Get the mouse position in window coordinates
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

        // Check if the mouse is within the button's bounds
        if (button.getGlobalBounds().contains(worldMousePos)) {
            // Change button texture or appearance when hovered
            button.setFillColor(sf::Color(100, 100, 255));  // Example: change color to blue when hovered
        } else {
            button.setFillColor(sf::Color(255, 255, 255));  // Reset to original color when not hovered
        }
    }

private:
    sf::RectangleShape button;
    sf::Text buttonText;
    sf::Texture buttonTexture;
    std::function<void()> onClick;
};

// Main loop for intro movie
void playIntroMovie(sf::RenderWindow& window) {
    // Clock for time handling
    sf::Clock clock;

    // Load the background texture and font
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("/home/z3ta/c++/SoV/images/castle1.png")) {
        std::cerr << "Error loading background texture" << std::endl;
        return;
    }

    sf::Font font;
    if (!font.loadFromFile("/usr/share/texmf/fonts/opentype/public/tex-gyre/texgyrechorus-mediumitalic.otf")) {
        std::cerr << "Error loading font" << std::endl;
        return;
    }

    // Set up scrolling text
    sf::Text scrollingText("Once upon a time, at the peak of the tallest mountain, in the northern-most part of the world,\nthere ruled a great king.\nThe people of Icingr, in the harsh, cold environment of their land, have managed to prosper;\nthanks to their great kings unwavering rule over many decades.", font, 42);
    scrollingText.setFillColor(sf::Color::White);
    scrollingText.setPosition(0, window.getSize().y);

    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setPosition(200, 200);

    // Create the skip button
    Button skipButton(
        window.getSize().x * 0.8f - 75,  // Center the button horizontally
        window.getSize().y * 0.8f,        // Place the button lower on the screen
        150, 75,                          // Button size
        "Continue", font, "/home/z3ta/c++/SoV/images/sprites/buttons/button1.png", 
        [&]() {
            hasPlayed = true; // Mark that the intro has been skipped
        }
    );

    // Load and play audio
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("/home/z3ta/c++/SoV/audio/introtracks/intro1.ogg")) {
        std::cerr << "Error loading audio file" << std::endl;
        return;
    }
    sf::Sound sound(buffer);
    sound.play();  // Play the audio when the intro starts

    bool textScrolled = false;

    // Main loop for the intro movie
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check for button clicks
            skipButton.checkClick(event, window);

            // If the skip button was clicked, stop the audio
            if (hasPlayed) {
                sound.stop();
                break;  // Exit the loop if the intro has been skipped
            }
        }

        // Handle text scrolling
        if (!textScrolled) {
            float deltaTime = clock.restart().asSeconds();
            scrollingText.move(0, -15 * deltaTime);

            if (scrollingText.getPosition().y + scrollingText.getLocalBounds().height < 0) {
                textScrolled = true;
            }
        }

        // Check if the mouse is hovering over the skip button
        skipButton.checkHover(window);

        // Draw everything
        window.clear();
        window.draw(backgroundSprite); // Draw the background
        window.draw(scrollingText);     // Draw the scrolling text
        skipButton.draw(window);        // Draw the skip button
        window.display();

        // If hasPlayed is true, break the loop to stop the intro without closing the game
        if (hasPlayed) {
            break;  // Exit the loop when the intro has been skipped or completed
        }
    }
}

#endif // BUTTON_H