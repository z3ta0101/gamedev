#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <tinyxml2.h>
#include <limits>
#include <random>
#include <functional>
#include <memory>

using namespace sf;
int area = 2;
//Player Coordinates
sf::Vector2f playerPosition(1900, -3000);

sf::Vector2i mousePosx;

sf::Sprite player;

class Door {
public:
    Door(const sf::Vector2f& position, const std::string& texturePath)
        : doorOpen(false), isClicked(false) {

        // Load the door texture and initialize the sprite
        if (!doorTexture.loadFromFile(texturePath)) {
            std::cerr << "Error loading door texture!" << std::endl;
        }
        doorSprite.setTexture(doorTexture);
        doorSprite.setPosition(position);
        doorSprite.setTextureRect(sf::IntRect(0, 0, 338, 281));  // Initial door state (closed)

        // Set up the clickable polygon (assuming door is a rectangle)
        doorPolygon.setPointCount(4);
        doorPolygon.setPoint(0, sf::Vector2f(1052, -3228));
        doorPolygon.setPoint(1, sf::Vector2f(1052, -3410));
        doorPolygon.setPoint(2, sf::Vector2f(1159, -3429));
        doorPolygon.setPoint(3, sf::Vector2f(1159, -3255));

        // Set a transparent fill and visible outline color for debugging
        doorPolygon.setFillColor(sf::Color::Transparent);
        doorPolygon.setOutlineThickness(2);
    }

    void update(const sf::Vector2i& mousePos, sf::RenderWindow& window, const sf::Vector2f& playerPos) {
        // Convert mouse position to world coordinates
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

        // Calculate the distance between the player and the door
        float distanceToDoor = std::sqrt(std::pow(playerPos.x - doorSprite.getPosition().x, 2) +
                                         std::pow(playerPos.y - doorSprite.getPosition().y, 2));

        // Set a threshold distance for interaction
        float interactionThreshold = 200.0f;

        if (distanceToDoor <= interactionThreshold) {
            bool isHovering = pointInPolygon(doorPolygon, worldMousePos);

            if (isHovering) {
                doorPolygon.setOutlineColor(sf::Color::Blue);

                // Handle click detection
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                    toggleDoorState(interactionThreshold);  // Pass global 'area'
                    isClicked = true;
                }
            } else {
                doorPolygon.setOutlineColor(sf::Color::Transparent);
            }
        } else {
            doorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        // Reset the isClicked flag when the mouse button is released
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorSprite);  // Draw the door sprite (with current animation)
        window.draw(doorPolygon); // Draw the clickable polygon (for visual testing)
    }

    bool pointInPolygon(const sf::ConvexShape& polygon, const sf::Vector2f& point) {
        bool inside = false;
        int n = polygon.getPointCount();
        int x1 = polygon.getPoint(0).x, y1 = polygon.getPoint(0).y;

        for (int i = 1; i <= n; i++) {
            int x2 = polygon.getPoint(i % n).x, y2 = polygon.getPoint(i % n).y;

            if (point.y > std::min(y1, y2)) {
                if (point.y <= std::max(y1, y2)) {
                    if (point.x <= std::max(x1, x2)) {
                        if (y1 != y2) {
                            int xinters = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                            if (x1 == x2 || point.x <= xinters) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
            x1 = x2;
            y1 = y2;
        }
        return inside;
    }

    void toggleDoorState(float interactionThreshold) {
        if (doorOpen) {
            doorSprite.setTextureRect(sf::IntRect(0, 0, 338, 281));  // Close door
        } else {
            doorSprite.setTextureRect(sf::IntRect(338, 0, 338, 281));  // Open door
        }
        doorOpen = !doorOpen;

        if (!doorOpen && interactionThreshold <= 200.0f) {
            area = 4;  // Update the area to 4 when the door opens and interaction is valid
            player.setPosition(2255.0f, 4110.0f);  // Move the player to the new position
        }
    }

private:
    sf::Texture doorTexture;
    sf::Sprite doorSprite;
    sf::ConvexShape doorPolygon;
    bool doorOpen;
    bool isClicked;
};

class Door2 {
public:
    // Updated constructor without sf::View& parameter
    Door2(const sf::Vector2f& position, const std::string& texturePath)
        : doorOpen(false), isClicked(false) {
        
        // Load the door texture and initialize the sprite
        if (!doorTexture.loadFromFile(texturePath)) {
            std::cerr << "Error loading door texture!" << std::endl;
        }
        doorSprite.setTexture(doorTexture);
        doorSprite.setPosition(position);
        doorSprite.setTextureRect(sf::IntRect(0, 0, 570, 1732)); // Initial door state (closed)

        // Set up the clickable polygon (assuming door is a rectangle)
        doorPolygon.setPointCount(4);
        doorPolygon.setPoint(0, sf::Vector2f(782, 1863));
        doorPolygon.setPoint(1, sf::Vector2f(613, 2154));
        doorPolygon.setPoint(2, sf::Vector2f(613, 2672));
        doorPolygon.setPoint(3, sf::Vector2f(782, 2377));

        // Set a transparent fill and visible outline color for debugging
        doorPolygon.setFillColor(sf::Color::Transparent);
        doorPolygon.setOutlineThickness(2);
    }

    // Updated update function with sf::RenderWindow& window parameter
    void update(const sf::Vector2i& mousePos, sf::RenderWindow& window, const sf::Vector2f& playerPos) {
        // Convert mouse position to world coordinates
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

        // Calculate the distance between the player and the door
        float distanceToDoor = std::sqrt(std::pow(playerPos.x - doorSprite.getPosition().x - 170, 1.8) +
                                         std::pow(playerPos.y - doorSprite.getPosition().y - 10, 1.8));

        // Set a threshold distance for interaction (you can adjust this value as needed)
        float interactionThreshold = 400.0f;  // You can change this value based on your needs

        // If the player is close enough to the door, allow interaction
        if (distanceToDoor <= interactionThreshold) {
            // Check if the mouse is hovering over the clickable polygon (use worldMousePos)
            bool isHovering = pointInPolygon(doorPolygon, worldMousePos);
        
            if (isHovering) {
                // Set the outline to blue when the mouse is over the polygon
                doorPolygon.setOutlineColor(sf::Color::Blue);

                // If the mouse is over the clickable polygon, handle click detection
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                    // Toggle door state on click
                    toggleDoorState();
                    isClicked = true;  // Prevent multiple toggles while holding down the mouse
                }
            } else {
                // Make the outline transparent when the mouse is not over the polygon
                doorPolygon.setOutlineColor(sf::Color::Transparent);
            }
        } else {
            // Make the outline transparent if the player is too far from the door
            doorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        // If the mouse button is released, reset the isClicked flag
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            std::cout << "World Mouse Position: (" 
              << worldMousePos.x << ", " 
              << worldMousePos.y << ")" << std::endl;
        }
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorSprite);     // Draw the door sprite (with current animation)
        window.draw(doorPolygon);    // Draw the clickable polygon (for visual testing)
    }

    // Function to check if a point is inside the polygon (click detection)
    bool pointInPolygon(const sf::ConvexShape& polygon, const sf::Vector2f& point) {
        bool inside = false;
        int n = polygon.getPointCount();
        int x1 = polygon.getPoint(0).x, y1 = polygon.getPoint(0).y;
    
        for (int i = 1; i <= n; i++) {
            int x2 = polygon.getPoint(i % n).x, y2 = polygon.getPoint(i % n).y;
        
            if (point.y > std::min(y1, y2)) {
                if (point.y <= std::max(y1, y2)) {
                    if (point.x <= std::max(x1, x2)) {
                        if (y1 != y2) {
                            int xinters = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                            if (x1 == x2 || point.x <= xinters) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
            x1 = x2; y1 = y2;
        }
        return inside;
    }

    // Function to toggle the door animation (open/closed)
    void toggleDoorState() {
        if (doorOpen) {
            // Set door to closed (left = 0)
            doorSprite.setTextureRect(sf::IntRect(0, 0, 570, 1732));
        } else {
            // Set door to open (left = 338 for example)
            doorSprite.setTextureRect(sf::IntRect(570, 0, 570, 1732));
        }
        doorOpen = !doorOpen;  // Toggle the door state
    }

private:
    sf::Texture doorTexture;
    sf::Sprite doorSprite;
    sf::ConvexShape doorPolygon;
    bool doorOpen;  // Track if the door is open or closed
    bool isClicked; // To prevent multiple toggles from a single click
};

class Door3 {
public:
    // Updated constructor without sf::View& parameter
    Door3(const sf::Vector2f& position, const std::string& texturePath)
        : doorOpen(false), isClicked(false) {
        
        // Load the door texture and initialize the sprite
        if (!doorTexture.loadFromFile(texturePath)) {
            std::cerr << "Error loading door texture!" << std::endl;
        }
        doorSprite.setTexture(doorTexture);
        doorSprite.setPosition(position);
        doorSprite.setTextureRect(sf::IntRect(0, 0, 184, 537)); // Initial door state (closed)

        // Set up the clickable polygon (assuming door is a rectangle)
        doorPolygon.setPointCount(4);
        doorPolygon.setPoint(0, sf::Vector2f(8010, -3034));
        doorPolygon.setPoint(1, sf::Vector2f(8010, -2905));
        doorPolygon.setPoint(2, sf::Vector2f(7939, -3029));
        doorPolygon.setPoint(3, sf::Vector2f(7939, -3157));

        // Set a transparent fill and visible outline color for debugging
        doorPolygon.setFillColor(sf::Color::Transparent);
        doorPolygon.setOutlineThickness(2);
    }

    // Updated update function with sf::RenderWindow& window parameter
    void update(const sf::Vector2i& mousePos, sf::RenderWindow& window, const sf::Vector2f& playerPos) {
        // Convert mouse position to world coordinates
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

        // Calculate the distance between the player and the door
        float distanceToDoor = std::sqrt(std::pow(playerPos.x - doorSprite.getPosition().x, 2) +
                                         std::pow(playerPos.y - doorSprite.getPosition().y, 2));

        // Set a threshold distance for interaction (you can adjust this value as needed)
        float interactionThreshold = 400.0f;  // You can change this value based on your needs

        // If the player is close enough to the door, allow interaction
        if (distanceToDoor <= interactionThreshold) {
            // Check if the mouse is hovering over the clickable polygon (use worldMousePos)
            bool isHovering = pointInPolygon(doorPolygon, worldMousePos);
        
            if (isHovering) {
                // Set the outline to blue when the mouse is over the polygon
                doorPolygon.setOutlineColor(sf::Color::Blue);

                // If the mouse is over the clickable polygon, handle click detection
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                    // Toggle door state on click
                    toggleDoorState();
                    isClicked = true;  // Prevent multiple toggles while holding down the mouse
                }
            } else {
                // Make the outline transparent when the mouse is not over the polygon
                doorPolygon.setOutlineColor(sf::Color::Transparent);
            }
        } else {
            // Make the outline transparent if the player is too far from the door
            doorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        // If the mouse button is released, reset the isClicked flag
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            std::cout << "World Mouse Position: (" 
              << worldMousePos.x << ", " 
              << worldMousePos.y << ")" << std::endl;
        }
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorSprite);     // Draw the door sprite (with current animation)
        window.draw(doorPolygon);    // Draw the clickable polygon (for visual testing)
    }

    // Function to check if a point is inside the polygon (click detection)
    bool pointInPolygon(const sf::ConvexShape& polygon, const sf::Vector2f& point) {
        bool inside = false;
        int n = polygon.getPointCount();
        int x1 = polygon.getPoint(0).x, y1 = polygon.getPoint(0).y;
    
        for (int i = 1; i <= n; i++) {
            int x2 = polygon.getPoint(i % n).x, y2 = polygon.getPoint(i % n).y;
        
            if (point.y > std::min(y1, y2)) {
                if (point.y <= std::max(y1, y2)) {
                    if (point.x <= std::max(x1, x2)) {
                        if (y1 != y2) {
                            int xinters = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                            if (x1 == x2 || point.x <= xinters) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
            x1 = x2; y1 = y2;
        }
        return inside;
    }

    // Function to toggle the door animation (open/closed)
    void toggleDoorState() {
        if (doorOpen) {
            // Set door to closed (left = 0)
            doorSprite.setTextureRect(sf::IntRect(0, 0, 184, 537));
        } else {
            // Set door to open (left = 338 for example)
            doorSprite.setTextureRect(sf::IntRect(184, 0, 184, 537));
        }
        doorOpen = !doorOpen;  // Toggle the door state
    }

private:
    sf::Texture doorTexture;
    sf::Sprite doorSprite;
    sf::ConvexShape doorPolygon;
    bool doorOpen;  // Track if the door is open or closed
    bool isClicked; // To prevent multiple toggles from a single click
};

class Door4 {
public:
    // Updated constructor without sf::View& parameter
    Door4(const sf::Vector2f& position, const std::string& texturePath)
        : doorOpen(false), isClicked(false) {
        
        // Load the door texture and initialize the sprite
        if (!doorTexture.loadFromFile(texturePath)) {
            std::cerr << "Error loading door texture!" << std::endl;
        }
        doorSprite.setTexture(doorTexture);
        doorSprite.setPosition(position);
        doorSprite.setTextureRect(sf::IntRect(0, 0, 410, 794)); // Initial door state (closed)

        // Set up the clickable polygon (assuming door is a rectangle)
        doorPolygon.setPointCount(4);
        doorPolygon.setPoint(0, sf::Vector2f(9898, 1217));
        doorPolygon.setPoint(1, sf::Vector2f(9898, 1467));
        doorPolygon.setPoint(2, sf::Vector2f(10052, 1622));
        doorPolygon.setPoint(3, sf::Vector2f(10052, 1371));

        // Set a transparent fill and visible outline color for debugging
        doorPolygon.setFillColor(sf::Color::Transparent);
        doorPolygon.setOutlineThickness(2);
    }

    // Updated update function with sf::RenderWindow& window parameter
    void update(const sf::Vector2i& mousePos, sf::RenderWindow& window, const sf::Vector2f& playerPos) {
        // Convert mouse position to world coordinates
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);

        // Calculate the distance between the player and the door
        float distanceToDoor = std::sqrt(std::pow(playerPos.x - doorSprite.getPosition().x, 2) +
                                         std::pow(playerPos.y - doorSprite.getPosition().y, 2));

        // Set a threshold distance for interaction (you can adjust this value as needed)
        float interactionThreshold = 700.0f;  // You can change this value based on your needs

        // If the player is close enough to the door, allow interaction
        if (distanceToDoor <= interactionThreshold) {
            // Check if the mouse is hovering over the clickable polygon (use worldMousePos)
            bool isHovering = pointInPolygon(doorPolygon, worldMousePos);
        
            if (isHovering) {
                // Set the outline to blue when the mouse is over the polygon
                doorPolygon.setOutlineColor(sf::Color::Blue);

                // If the mouse is over the clickable polygon, handle click detection
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                    // Toggle door state on click
                    toggleDoorState();
                    isClicked = true;  // Prevent multiple toggles while holding down the mouse
                }
            } else {
                // Make the outline transparent when the mouse is not over the polygon
                doorPolygon.setOutlineColor(sf::Color::Transparent);
            }
        } else {
            // Make the outline transparent if the player is too far from the door
            doorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        // If the mouse button is released, reset the isClicked flag
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            std::cout << "World Mouse Position: (" 
              << worldMousePos.x << ", " 
              << worldMousePos.y << ")" << std::endl;
        }
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorSprite);     // Draw the door sprite (with current animation)
        window.draw(doorPolygon);    // Draw the clickable polygon (for visual testing)
    }

    // Function to check if a point is inside the polygon (click detection)
    bool pointInPolygon(const sf::ConvexShape& polygon, const sf::Vector2f& point) {
        bool inside = false;
        int n = polygon.getPointCount();
        int x1 = polygon.getPoint(0).x, y1 = polygon.getPoint(0).y;
    
        for (int i = 1; i <= n; i++) {
            int x2 = polygon.getPoint(i % n).x, y2 = polygon.getPoint(i % n).y;
        
            if (point.y > std::min(y1, y2)) {
                if (point.y <= std::max(y1, y2)) {
                    if (point.x <= std::max(x1, x2)) {
                        if (y1 != y2) {
                            int xinters = (point.y - y1) * (x2 - x1) / (y2 - y1) + x1;
                            if (x1 == x2 || point.x <= xinters) {
                                inside = !inside;
                            }
                        }
                    }
                }
            }
            x1 = x2; y1 = y2;
        }
        return inside;
    }

    // Function to toggle the door animation (open/closed)
    void toggleDoorState() {
        if (doorOpen) {
            // Set door to closed (left = 0)
            doorSprite.setTextureRect(sf::IntRect(0, 0, 410, 794));
        } else {
            // Set door to open (left = 338 for example)
            doorSprite.setTextureRect(sf::IntRect(410, 0, 410, 794));
        }
        doorOpen = !doorOpen;  // Toggle the door state
    }

private:
    sf::Texture doorTexture;
    sf::Sprite doorSprite;
    sf::ConvexShape doorPolygon;
    bool doorOpen;  // Track if the door is open or closed
    bool isClicked; // To prevent multiple toggles from a single click
};

bool hasPlayed = false;

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
    //clock
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
    sf::Text scrollingText("Once upon a time, at the peak of the tallest mountain, in the northern-most part of the world,\nthere ruled a great king.\nThis king had led his people to victory through countless battles, and many wars.\nThe people of Icingr, in the harsh, cold environment of their land, have managed to prosper;\nthanks to their great kings unwavering rule over many decades.\nHowever, this king was not without enemies. For scattered throughout the icy realm;\nthere were other, more malevolent forces that sought control over the land.\nA great white dragon, known as Ghaalungraan the grim, was one such enemy of the great king.\nThe dragon wanted to take the land of Icingr for himself;\nso that from his perch upon the ice-crowned peaks,\nhe could begin his campaign for dominion over all the world.\nHowever, the king was well fortified from within his position inside the castle,\nwhich was built sturdily into the side of the mountain.\nAnd Ghaalungran knew that defeating him would not be easy.\nSo, the dragon challenged him to a one on one battle.\nKnowing that the kings prideful arrogance would not let him decline,\nGhaalungran saw this as an opportunity to destroy the king,\nand rule the land once and for all.\nThe battle was fierce. Ghaalungran had greatly underestimated the kings power,\nand almost paid the price.\nHowever; with one last great breath of excruciating, brutal frost,\nthe dragon froze the king in place atop the balcony of his castle,\nand shattered him into a million pieces.\nThus began the rule of the great dragon Ghaalungran.\nGhaalungran the grim cared not for the fate of Icingrs people. \nTo him they were merely cannon fodder; an expendable tool that he could use as he pleased.\nHe sent them off to wars with the southern kingdoms, caring not if they won or lost;\nbecause as they fought, and died, his true army only grew stronger.\nAs the once mighty forces of Icingr fizzled out against the many great armies of the south,\nGhaalungrans pile of corpses grew ever higher.\nSoon, the tyrannical dragon would have the power to summon an undead army;\nthe likes of which had never been known before,\nand his conquest for dominion over the entire world would be unstoppable.\nThus begins our tale here; in the quiet mountain village of Fjelenvar.\nYou have lived here all your life, and know little of what lies beyond.\nYou do know, however, that you are an adopted child;\nand that your exact lineage has never really been made clear.\nYou were rescued from the wilderness as an infant by some of the villagers here,\nand have been raised under their guidance ever since.\nAnd now, coming of age, you yearn to forge your own destiny,\nand learn more of the world that lies outside your humble village.\nQuite frequently, you dream of becoming someone great and powerful.\nMaybe even someone that could put a stop to all the senseless wars coming from the north...", font, 42);
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

int generateRandomNumber() {
    // Create a random number generator and seed it
    static std::random_device rd;  // Random seed
    static std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<> dis(1, 20); // Define range [1, 20]

    return dis(gen);  // Generate and return the random number
}

class Minimap {
public:
    Minimap(const std::string& areaImagePath, float minimapScale, const sf::Vector2f& fogRadius)
        : scale(minimapScale), fogRadius(fogRadius) {

        // Load the area texture
        if (!backgroundTexture.loadFromFile(areaImagePath)) {
            std::cerr << "Error loading minimap area texture!" << std::endl;
        }

        // Create the background sprite (scaled-down area image)
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(scale, scale); // Scale the image down for the minimap
		
        // Set up the player indicator
        playerIndicator.setRadius(5);
        playerIndicator.setFillColor(sf::Color::White);

        // Create the fog of war overlay
        fogOverlay.setSize(sf::Vector2f(backgroundSprite.getGlobalBounds().width, backgroundSprite.getGlobalBounds().height));
        fogOverlay.setFillColor(sf::Color(0, 0, 0, 200)); // Semi-transparent black

        // Create the render texture for fog of war
        fogRenderTexture.create(backgroundSprite.getGlobalBounds().width, backgroundSprite.getGlobalBounds().height);
        fogRenderTexture.clear(sf::Color::Transparent); // Clear with transparent color
    }

    // Updates the minimap and player indicator position
    void update(const sf::Vector2f& playerPosition, const sf::Vector2f& windowSize) {
        // Adjust the minimap's position based on the player's location
        // Here, the minimap is placed at the bottom-left corner
        minimapPosition = playerPosition;
		int indicatorSpeed = 200;
        // Update the player indicator on the minimap (based on player position)
        // Adjust the player's minimap position based on the scale of the minimap
        playerIndicator.setPosition(minimapPosition.x + 20 + (playerPosition.x * scale / 3.77 - playerIndicator.getRadius()),
                                    minimapPosition.y + 130 + (playerPosition.y * scale / 3.77 - playerIndicator.getRadius()));

        // Update the fog of war (hidden area) based on the player's position
        updateFogOfWar(playerPosition);
    }

    // Draw the minimap (background, fog of war, and player indicator)
    void draw(sf::RenderWindow& window) {
        // Draw the minimap background (scaled-down area)
        backgroundSprite.setPosition(minimapPosition);
        window.draw(backgroundSprite);

        // Draw fog of war
        window.draw(fogOverlay);

        // Draw the player indicator
        window.draw(playerIndicator);
    }

private:
    sf::Texture backgroundTexture;  // Area texture for minimap
    sf::Sprite backgroundSprite;    // Scaled background sprite
    sf::CircleShape playerIndicator; // Indicator for player position
    sf::RectangleShape fogOverlay;  // Fog of war overlay
    sf::RenderTexture fogRenderTexture; // Render texture for dynamic fog of war
    sf::Vector2f minimapPosition;  // Position of minimap on screen (fixed in bottom-left corner)
    float scale;  // Scale factor for minimap
    sf::Vector2f fogRadius; // Fog of war radius around the player

    // Updates the fog of war overlay
    void updateFogOfWar(const sf::Vector2f& playerPosition) {
        // Set up a circular "visibility" area around the player
        sf::CircleShape visibilityCircle(fogRadius.x * scale);  // Scale the fog radius
        visibilityCircle.setFillColor(sf::Color::Transparent);
        visibilityCircle.setOutlineColor(sf::Color::Transparent);
        visibilityCircle.setPosition(minimapPosition.x + (playerPosition.x * scale - visibilityCircle.getRadius()),
                                     minimapPosition.y + (playerPosition.y * scale - visibilityCircle.getRadius()));

        // Clear the render texture and draw fog of war elements
        fogRenderTexture.clear(sf::Color::Transparent);
        fogRenderTexture.draw(fogOverlay); // Draw static fog overlay (if any)
        fogRenderTexture.draw(visibilityCircle); // Draw dynamic visibility circle around player
        fogRenderTexture.display(); // Update the render texture

        // Update the fog of war overlay sprite with the new render texture
        fogOverlay.setTexture(&fogRenderTexture.getTexture()); // Set updated texture
    }
};

class NPC {
public:
    NPC(float startX, float startY, sf::Font& font, sf::RenderWindow& window)
        : x(startX), y(startY), window(window), customCursorActive(false), isActive(false), selectedOption(-1) {

        std::cout << "Loading NPC texture..." << std::endl;
        
        if (!texture.loadFromFile("/home/z3ta/c++/SoV/images/sprites/dwarfidlespritesheet.png")) {
            std::cerr << "Error loading NPC texture!" << std::endl;
        } else {
            std::cout << "NPC texture loaded successfully." << std::endl;
        }

        sf::IntRect rectSourceSprite;
        sprite.setTexture(texture);
        rectSourceSprite = sf::IntRect(0, 0, 27, 45);  // Assuming each sprite frame is 27x45 (adjust as needed)
        sprite.setTextureRect(rectSourceSprite);
        sf::Vector2f sizeIncrease(1.4f, 1.4f);
        sprite.setScale(sf::Vector2f(sprite.getScale().x + sizeIncrease.x, sprite.getScale().y + sizeIncrease.y));
        sprite.setPosition(x, y);

        // Initialize the conversation box at fixed window position
        box.setSize(sf::Vector2f(800, 200));  // Adjust box size if needed
        box.setFillColor(sf::Color(0, 0, 0, 180));  // Semi-transparent black
        box.setPosition(400.f, 1100.f);  // This should always appear at the same position on screen

        npcText.setFont(font);
        npcText.setCharacterSize(20);
        npcText.setFillColor(sf::Color::White);
        npcText.setPosition(410.f, 1110.f);  // Text should appear just above the box

        // Initialize options with the correct size (maxOptions)
        options.resize(maxOptions);  // Ensure we have maxOptions number of elements
        for (int i = 0; i < maxOptions; ++i) {
            options[i].setFont(font);
            options[i].setCharacterSize(18);
            options[i].setFillColor(sf::Color::Red);
            options[i].setPosition(410.f, 1150.f + (i * 30));  // Position options below the text
        }

        std::cout << "NPC initialized with conversation box." << std::endl;
    }

    void update(sf::Vector2f playerPos, sf::Vector2i mousePos) {
        sf::Vector2f worldMousePos = window.mapPixelToCoords(mousePos);
		
		
        // Change cursor if the mouse is over the NPC
        if (sprite.getGlobalBounds().contains(worldMousePos)) {
            if (!customCursorActive) {
                changeMouseCursor("/home/z3ta/c++/SoV/images/sprites/mouse/mousesocial.png");
                customCursorActive = true;
            }
        } else if (customCursorActive) {
            resetToDefaultCursor();
            customCursorActive = false;
        }

        // Update mouse hover only if the conversation box is active
        if (isActive) {
            updateMouseHover(mousePos);
        }
    }

    void handleInput(const sf::Event& event) {
        std::cout << "Handling Input Event" << std::endl;

        // Only handle the left mouse button press event
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            std::cout << "Mouse Position: (" << event.mouseButton.x << ", " << event.mouseButton.y << ")" << std::endl;
        
            // Get mouse position in screen coordinates
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

            // Case 1: Clicked on NPC sprite to start the conversation
            if (sprite.getGlobalBounds().contains(mousePos)) {
                if (!isActive) {
                    // Start the conversation and show dialogue box
                    isActive = true;
                    setText("Hello, traveler! How can I help you?");
                    setOptions({"Ask about the quest", "Tell me about yourself", "Goodbye"});
                    std::cout << "Conversation box activated." << std::endl;
                }
            }
        
            // Case 2: Clicked inside the conversation box (check for option selection)
            else if (isActive && box.getGlobalBounds().contains(mousePos)) {
                int selectedOption = getSelectedOption();
            
                if (selectedOption == 0) {
                    setText("You asked about the quest! Here's more information.");
                    setOptions({"Continue the quest", "Goodbye"});
                } else if (selectedOption == 1) {
                    setText("You asked about me! I'm an NPC with much knowledge.");
                    setOptions({"Ask about the quest", "Goodbye"});
                } else if (selectedOption == 2) {
                    setText("Goodbye, traveler!");
                    isActive = false;  // Deactivate conversation box only when "Goodbye" is selected
                    std::cout << "Conversation box deactivated." << std::endl;
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);  // Always draw NPC sprite

        if (isActive) {
            drawConversationBox(window);  // Draw conversation box if active
        }
    }

    bool getIsActive() const {
        return isActive;
    }

    void setText(const std::string& npcDialogue) {
        npcText.setString(npcDialogue);
        std::cout << "NPC text set to: " << npcDialogue << std::endl;
    }

    void setOptions(const std::vector<std::string>& playerOptions) {
        for (size_t i = 0; i < playerOptions.size() && i < maxOptions; ++i) {
            options[i].setString(playerOptions[i]);
        }
    }

    int getSelectedOption() const {
        return selectedOption;
    }

private:
    void updateMouseHover(sf::Vector2i mousePos) {
    	selectedOption = -1;

    	// Ensure we're working with window-local coordinates here
    	// We're using the mouse position as is from the event (screen pixels)
    	sf::Vector2f windowMousePos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    	// Print out the window-local mouse position for debugging
    	std::cout << "Window Mouse Position: (" << windowMousePos.x << ", " << windowMousePos.y << ")" << std::endl;

    	// Iterate through options and check if the mouse is over the option's bounding box
    	for (size_t i = 0; i < options.size(); ++i) {
        	// Get the global bounds of the current option (in window coordinates)
        	sf::FloatRect optionBounds = options[i].getGlobalBounds();

        	// Print out the option's bounding box for debugging
        	std::cout << "Option " << i << " bounds: "
                  	<< "Left: " << optionBounds.left
                  	<< ", Top: " << optionBounds.top
                  	<< ", Width: " << optionBounds.width
                  	<< ", Height: " << optionBounds.height << std::endl;

        	// Check if the mouse position is over the text option area (in window-local coordinates)
        	if (optionBounds.contains(windowMousePos)) {
            	selectedOption = i;
            	std::cout << "Mouse hovering over option " << selectedOption << std::endl;
            	break;  // Stop once we detect the hovered option
        	}
    	}
	}

    void drawConversationBox(sf::RenderWindow& window) {
        if (isActive) {
            // Save the current view (in case you have camera transformations)
            sf::View originalView = window.getView();
			window.setView(sf::View{sf::FloatRect{{0.f, 0.f}}, sf::Vector2f(window.getSize())}});
			
            // Now draw UI elements in screen space
            window.draw(box);  // Draw the conversation box
            window.draw(npcText);  // Draw NPC dialogue

            // Draw the options
            for (size_t i = 0; i < options.size(); ++i) {
                // Set color based on hover state
                options[i].setFillColor(i == selectedOption ? sf::Color::White : sf::Color::Red);
                window.draw(options[i]);  // Draw each option
            }

            // Restore the original view
            window.setView(originalView);
			
        }
    }

    void changeMouseCursor(const std::string& cursorPath) {
        sf::Image cursorImage;
        if (!cursorImage.loadFromFile(cursorPath)) {
            std::cerr << "Error loading cursor image from path: " << cursorPath << std::endl;
            resetToDefaultCursor();
            return;
        }

        sf::Cursor customCursor;
        if (customCursor.loadFromPixels(cursorImage.getPixelsPtr(),
                                        sf::Vector2u(cursorImage.getSize().x, cursorImage.getSize().y),
                                        sf::Vector2u(0, 0))) {
            window.setMouseCursor(customCursor);
        } else {
            std::cerr << "Failed to create cursor from image!" << std::endl;
            resetToDefaultCursor();
        }
    }

    void resetToDefaultCursor() {
        sf::Cursor defaultCursor;
        if (defaultCursor.loadFromSystem(sf::Cursor::Arrow)) {
            window.setMouseCursor(defaultCursor);  // Use the default cursor
        } else {
            std::cerr << "Error loading default cursor!" << std::endl;
        }
    }

    float x, y;
    bool customCursorActive;
    bool isActive;
    sf::Sprite sprite;
    sf::Texture texture;
    sf::RectangleShape box;
    sf::Text npcText;
    std::vector<sf::Text> options;
    int selectedOption;
    static const int maxOptions = 3;
    sf::RenderWindow& window;
};

int playerHealth = 10;
int playerArmor = 17;

class Enemy {
public:
    float x, y; // Enemy's position
    int direction; // Direction the enemy is facing (0-7)
	int health = 4;
	int playerhealth;
	int playerarmor;
    const float detectionRange = 1000.0f; // Range to detect a target
	const float attackRange = 70.0f;
    float speed = 2.0f; // Enemy movement speed
	bool attacking = false;
    sf::Sprite sprite;  // The enemy's sprite
    sf::Texture texture; // The texture (spritesheet)
    sf::IntRect rectSourceSprite; // Source rect for texture (for animation)

	int generateRandomNumber() {
    	// Create a random number generator and seed it
    	static std::random_device rd;  // Random seed
    	static std::mt19937 gen(rd()); // Mersenne Twister engine
    	std::uniform_int_distribution<> dis(1, 20); // Define range [1, 20]

    	return dis(gen);  // Generate and return the random number
	}

	int enemyAttackRoll = generateRandomNumber();
    // Timing variables for animation frame updates
    float animationTimer;
    const float animationSpeed = 70.0f; // Speed of animation frame updates (seconds per frame)

    // Boolean to track whether the enemy is moving
    bool isMoving;

    // Constructor to initialize the enemy's position and load the texture
    Enemy(float startX, float startY, int& playerHealth, int& playerArmor) : x(startX), y(startY), direction(0), animationTimer(0.0f), isMoving(false), playerhealth(playerHealth), playerarmor(playerArmor) {
        if (!texture.loadFromFile("/home/z3ta/c++/SoV/images/sprites/skelspritesheetx.png")) {
            std::cerr << "Error loading enemy texture!" << std::endl;
        }

        sprite.setTexture(texture);
        rectSourceSprite = sf::IntRect(0, 0, 127, 100); // Initial position of the sprite (default direction 0)
        sprite.setTextureRect(rectSourceSprite);
		sf::Vector2f sizeIncrease(1.0f, 1.0f);
        sprite.setScale(sf::Vector2f(sprite.getScale().x + sizeIncrease.x, sprite.getScale().y + sizeIncrease.y));
        sprite.setPosition(x, y);
    }

    // Update the enemy's behavior based on a target position (e.g., player or other object)
    void update(float targetX, float targetY, float deltaTime) {
        float distance = std::sqrt(std::pow(targetX - x, 2) + std::pow(targetY - y, 2));

        // Only move and update animation if within detection range
        if (distance < detectionRange) {
            // Move the enemy towards the target
            moveTowardsTarget(targetX, targetY, deltaTime);

            // Update animation based on whether the enemy is moving
            updateAnimation(deltaTime);
			sprite.setPosition(x, y);
        } else {
            // If the enemy is outside detection range, set it to idle
            isMoving = false;
            updateAnimation(deltaTime);
			
        }
		
    }

	void updatehealth(float targetX, float targetY){
		float distance = std::sqrt(std::pow(targetX - x, 2) + std::pow(targetY - y, 2));
		std::cout << "distance: " << distance << std::endl;
		if(distance < attackRange){
			health -= 1;
			std::cout << "enemy health: " << health << std::endl;
		}
	
	}

private:
    // Method for moving the enemy towards the target (could be the player or any other target)
    void moveTowardsTarget(float targetX, float targetY, float deltaTime){
    	// Calculate direction to move
    	float dirX = targetX - x;
    	float dirY = targetY - y;
    	float length = std::sqrt(dirX * dirX + dirY * dirY);
		
    	if (length > 70 && health > 0){
        	// Normalize direction vector
        	dirX /= length;
        	dirY /= length;

        	// Move the enemy towards the target
        	x += dirX * speed * 1;  // Adjust movement by deltaTime for smooth movement
        	y += dirY * speed * 1;

        	// Update the direction the enemy is facing
        	updateMovementDirection(dirX, dirY);

        	// Set isMoving to true since the enemy is moving
        	isMoving = true;
			attacking = false;
    	} 
		
		else{
        	// If the distance to the target is too small, set isMoving to false (idle)
        	isMoving = false;
			attacking = true;
			updateAttackDirection(dirX, dirY);
    	}

		if(health <= 0){
			isMoving = false;
			updateDeathDirection(dirX, dirY);
		}

	}

    // Update the direction the enemy is facing based on the movement direction
    void updateMovementDirection(float dirX, float dirY){
    	// Calculate the angle in radians
    	float angle = std::atan2(dirY, dirX) * 180.0f / 3.14159f; // Convert radians to degrees

    	// Normalize the angle to be between 0 and 360 degrees
    	if (angle < 0) {
        	angle += 360.0f;
    	}

    	// Now map the angle to one of the 8 directions (0-7)
    	if (angle >= 337.5f || angle < 22.5f) {
        	direction = 46; // Right
    	} else if (angle >= 22.5f && angle < 67.5f) {
        	direction = 47; // Down-Right
    	} else if (angle >= 67.5f && angle < 112.5f) {
        	direction = 40; // Down
    	} else if (angle >= 112.5f && angle < 157.5f) {
        	direction = 41; // Down-Left
    	} else if (angle >= 157.5f && angle < 202.5f) {
        	direction = 42; // Left
    	} else if (angle >= 202.5f && angle < 247.5f) {
        	direction = 43; // Up-Left
    	} else if (angle >= 247.5f && angle < 292.5f) {
        	direction = 44; // Up
    	} else if (angle >= 292.5f && angle < 337.5f) {
        	direction = 45; // Up-Right
    	}
		
	}

	void updateAttackDirection(float dirX, float dirY) {
    	// Calculate the angle in radians
    	float angle = std::atan2(dirY, dirX) * 180.0f / 3.14159f; // Convert radians to degrees

    	// Normalize the angle to be between 0 and 360 degrees
    	if (angle < 0) {
        	angle += 360.0f;
    	}

    	// Now map the angle to one of the 8 directions (0-7)
    	if (angle >= 337.5f || angle < 22.5f) {
        	direction = 6; // Right
    	} else if (angle >= 22.5f && angle < 67.5f) {
        	direction = 7; // Down-Right
    	} else if (angle >= 67.5f && angle < 112.5f) {
        	direction = 0; // Down
    	} else if (angle >= 112.5f && angle < 157.5f) {
        	direction = 1; // Down-Left
    	} else if (angle >= 157.5f && angle < 202.5f) {
        	direction = 2; // Left
    	} else if (angle >= 202.5f && angle < 247.5f) {
        	direction = 3; // Up-Left
    	} else if (angle >= 247.5f && angle < 292.5f) {
        	direction = 4; // Up
    	} else if (angle >= 292.5f && angle < 337.5f) {
        	direction = 5; // Up-Right
    	}

	}

	void updateDeathDirection(float dirX, float dirY) {
		isMoving = false;
    	// Calculate the angle in radians
    	float angle = std::atan2(dirY, dirX) * 180.0f / 3.14159f; // Convert radians to degrees

    	// Normalize the angle to be between 0 and 360 degrees
    	if (angle < 0) {
        	angle += 360.0f;
    	}

    	// Now map the angle to one of the 8 directions (0-7)
    	if (angle >= 337.5f || angle < 22.5f) {
        	direction = 14; // Right
    	} else if (angle >= 22.5f && angle < 67.5f) {
        	direction = 15; // Down-Right
    	} else if (angle >= 67.5f && angle < 112.5f) {
        	direction = 8; // Down
    	} else if (angle >= 112.5f && angle < 157.5f) {
        	direction = 9; // Down-Left
    	} else if (angle >= 157.5f && angle < 202.5f) {
        	direction = 10; // Left
    	} else if (angle >= 202.5f && angle < 247.5f) {
        	direction = 11; // Up-Left
    	} else if (angle >= 247.5f && angle < 292.5f) {
        	direction = 12; // Up
    	} else if (angle >= 292.5f && angle < 337.5f) {
        	direction = 13; // Up-Right
    	}

		if (rectSourceSprite.left >= 1778) {
			rectSourceSprite.left = 1778;
		}
	}

    // Update the enemy's animation based on the direction and isMoving status
    void updateAnimation(float deltaTime) {
        // Increment the animation timer by the delta time
        animationTimer += 10;
		
        // If enough time has passed, move to the next animation frame
        if (animationTimer >= animationSpeed) {
            animationTimer = 0.0f; // Reset timer
			if (health > 0){
            	// Calculate the top position in the sprite sheet based on the current direction
            	rectSourceSprite.top = direction * 99.4; // 100 is the height of each sprite (height per row)
			}

			else{
				rectSourceSprite.top = 800;
			}
            // Determine the animation state based on movement (e.g., 0 = idle, 1 = moving)
            int animationState = isMoving ? 1 : 0;  // 1 for moving, 0 for idle

            // Update the left value based on the animation state
            
			if (health > 0){
            	// Move to the next frame in the animation (assuming 8 frames per direction)
            	if (rectSourceSprite.left >= 127 * 7) {  // If the left position exceeds the max frame (7 frames per direction)
                	rectSourceSprite.left = 0; // Reset to the first frame
            	} else {
                	rectSourceSprite.left += 127; // Move to the next frame
            	}
			}

			else if (health <= 0){
				if (rectSourceSprite.left >= 1778){
					rectSourceSprite.left = 1778;
				}
				else{
					rectSourceSprite.left += 127;
				}
			}

			if (health > 0 && attacking == true){
				if (rectSourceSprite.left == 889){
					enemyAttackRoll = generateRandomNumber();
					std::cout << "enemy attack roll: " << enemyAttackRoll << std::endl;
					if (enemyAttackRoll >= playerArmor){
						playerHealth -= 1;
                    	std::cout << "player health: " << playerHealth << std::endl;
					}
				}
			}

            // Set the updated texture rect for the sprite
            sprite.setTextureRect(rectSourceSprite);
        }
    }

public:
    // Getter for the sprite
    sf::Sprite& getSprite() {
        return sprite;
    }
};

// Define a structure for a 2D vector
struct Vectorx2f {
    float x, y;

    // Constructor
    Vectorx2f(float x = 0, float y = 0) : x(x), y(y) {}

    // Operator overloading for vector addition and subtraction
    Vectorx2f operator+(const Vectorx2f& other) const {
        return Vectorx2f(x + other.x, y + other.y);
    }

    Vectorx2f operator-(const Vectorx2f& other) const {
        return Vectorx2f(x - other.x, y - other.y);
    }

    // Dot product of two vectors
    float dot(const Vectorx2f& other) const {
        return x * other.x + y * other.y;
    }

    // Magnitude of the vector
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    // Normalize the vector
    Vectorx2f normalize() const {
        float magnitude = this->magnitude();
        return Vectorx2f(x / magnitude, y / magnitude);
    }

    // Convert from sf::Vector2f to Vectorx2f
    static Vectorx2f fromSF(const sf::Vector2f& sfVec) {
        return Vectorx2f(sfVec.x, sfVec.y);
    }

    // Convert from Vectorx2f to sf::Vector2f
    sf::Vector2f toSF() const {
        return sf::Vector2f(x, y);
    }
};

// Define a structure for a polygon
struct Polygon {
    std::vector<Vectorx2f> vertices;

    // Load vertices from an XML file
    void loadVerticesFromXML(const std::string& filename) {
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
            std::cerr << "Error loading XML file: " << filename << std::endl;
            return;
        }

        tinyxml2::XMLElement* polygonElement = doc.FirstChildElement("bodydef")
            ->FirstChildElement("bodies")
            ->FirstChildElement("body")
            ->FirstChildElement("fixture")
            ->FirstChildElement("polygon");

        if (!polygonElement) {
            std::cerr << "No polygon element found in XML." << std::endl;
            return;
        }

        for (tinyxml2::XMLElement* vertexElement = polygonElement->FirstChildElement("vertex");
             vertexElement != nullptr;
             vertexElement = vertexElement->NextSiblingElement("vertex")) {
            float x = vertexElement->FloatAttribute("x");
            float y = vertexElement->FloatAttribute("y");
            vertices.push_back(Vectorx2f(x, y));
        }
    }

    // Function to get the edges of the polygon
    std::vector<Vectorx2f> getEdges() const {
        std::vector<Vectorx2f> edges;
        for (size_t i = 0; i < vertices.size(); ++i) {
            size_t next = (i + 1) % vertices.size();
            edges.push_back(vertices[next] - vertices[i]);
        }
        return edges;
    }

    // Get the current position of the polygon (using the first vertex as the reference)
    Vectorx2f getPosition() const {
        return vertices.empty() ? Vectorx2f(0, 0) : vertices[0]; // Return the first vertex as the position
    }

    // Set the position of the polygon based on a new position
    void setPosition(const Vectorx2f& newPosition) {
        Vectorx2f currentPosition = getPosition(); // Get the current position
        Vectorx2f offset = newPosition - currentPosition; // Calculate the offset
		
        // Update each vertex position by the offset
        for (auto& vertex : vertices) {
            vertex = vertex + offset;
        }
    }
};

// Function to check for collision between two polygons using SAT
bool checkPolygonCollision(const Polygon& poly1, const Polygon& poly2, Vectorx2f& collisionNormal) {
    std::vector<Vectorx2f> edges1 = poly1.getEdges();
    std::vector<Vectorx2f> edges2 = poly2.getEdges();

    // Check for collision on all edges of both polygons
    for (const auto& edge : edges1) {
        Vectorx2f normal = Vectorx2f(-edge.y, edge.x).normalize(); // Normal of the edge
        float min1 = std::numeric_limits<float>::max();
        float max1 = std::numeric_limits<float>::min();
        float min2 = std::numeric_limits<float>::max();
        float max2 = std::numeric_limits<float>::min();

        for (const auto& vertex : poly1.vertices) {
            float dotProduct = vertex.dot(normal);
            if (dotProduct < min1) {
                min1 = dotProduct;
            }
            if (dotProduct > max1) {
                max1 = dotProduct;
            }
        }

        for (const auto& vertex : poly2.vertices) {
            float dotProduct = vertex.dot(normal);
            if (dotProduct < min2) {
                min2 = dotProduct;
            }
            if (dotProduct > max2) {
                max2 = dotProduct;
            }
        }

        if (max1 < min2 || max2 < min1) {
			
            return false; // No collision
        }
    }

    for (const auto& edge : edges2) {
        Vectorx2f normal = Vectorx2f(-edge.y, edge.x).normalize(); // Normal of the edge
        float min1 = std::numeric_limits<float>::max();
        float max1 = std::numeric_limits<float>::min();
        float min2 = std::numeric_limits<float>::max();
        float max2 = std::numeric_limits<float>::min();

        for (const auto& vertex : poly1.vertices) {
            float dotProduct = vertex.dot(normal);
            if (dotProduct < min1) {
                min1 = dotProduct;
            }
            if (dotProduct > max1) {
                max1 = dotProduct;
            }
        }

        for (const auto& vertex : poly2.vertices) {
            float dotProduct = vertex.dot(normal);
            
            if (dotProduct < min2) {
                min2 = dotProduct;
            }
            if (dotProduct > max2) {
                max2 = dotProduct;
            }
        }

        if (max1 < min2 || max2 < min1) {
            return false; // No collision
        }
    }

    // If we reach here, it means there is a collision
	
	
    collisionNormal = Vectorx2f(0, 0); // Placeholder for collision normal
	
    return true; // Collision detected
}

// Function to resolve the collision
void resolveCollision(Polygon& collisionx, const Vectorx2f& collisionNormal) {
    for (auto& vertex : collisionx.vertices) {
        vertex = vertex + collisionNormal; // Move player out of collision
		
    }
}

int main()
{
	// Load font for button text
    sf::Font font;
    if (!font.loadFromFile("/usr/share/texmf/fonts/opentype/public/tex-gyre/texgyrechorus-mediumitalic.otf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }
    Enemy enemy(50, 50, playerHealth, playerArmor);
    Enemy enemy1(500, 500, playerHealth, playerArmor);
	
	
	Minimap minimap1("/home/z3ta/c++/SoV/images/backgrounds/town1xmm.jpg", 0.1f, sf::Vector2f(100.f, 100.f)); // Example values
	Minimap minimap2("/home/z3ta/c++/SoV/images/backgrounds/town2mm.jpg", 0.1f, sf::Vector2f(100.f, 100.f)); // Example values
	Minimap minimap3("/home/z3ta/c++/SoV/images/backgrounds/wilderness1mm.jpg", 0.1f, sf::Vector2f(100.f, 100.f)); // Example values
	Door fjeltaverndoor(sf::Vector2f(900, -3472), "/home/z3ta/c++/SoV/images/sprites/doorspritesheet1.png");
	Door2 halfgiantsdoor(sf::Vector2f(249, 1733), "/home/z3ta/c++/SoV/images/sprites/hgdoorspritesheet.png");
	Door3 smithdoor(sf::Vector2f(7835, -3364), "/home/z3ta/c++/SoV/images/sprites/smithdoor.png");
	Door4 herbalistdoor(sf::Vector2f(9664, 967), "/home/z3ta/c++/SoV/images/sprites/herbalistdoorspritesheet.png");
	std::string textureFile = "/home/z3ta/c++/SoV/images/sprites/buttons/button1.png";

	int playerAttackRoll = generateRandomNumber();

	// Create a player polygon (initial position)
    Polygon collisionx;
    collisionx.vertices = {
        Vectorx2f(2790, 1840), // Initial position of player
        Vectorx2f(2790, 1910),
        Vectorx2f(2880, 1910),
        Vectorx2f(2880, 1840)
    };

	Polygon attackx;
	attackx.vertices = {
		Vectorx2f(2749, 1945), //attack range polygon
		Vectorx2f(2749, 1805),
		Vectorx2f(2919, 1805),
		Vectorx2f(2919, 1945)
	};

	Polygon barrelx;
	barrelx.vertices = {
		Vectorx2f(1705, 1090), //attack range polygon
		Vectorx2f(1805, 1090),
		Vectorx2f(1805, 1010),
		Vectorx2f(1705, 1010)
	};

	//Felstrond collision barriers

	Polygon fboundry1;
	fboundry1.vertices = {
		
		Vectorx2f(9050, -1414),
		Vectorx2f(8808, -1041),
		Vectorx2f(6695, -2250),
		Vectorx2f(6880, -2988)
		
	};

	Polygon fboundry2;
	fboundry2.vertices = {
		
		Vectorx2f(8955, -935),
		Vectorx2f(9095, -320),
		Vectorx2f(9625, -240),
		Vectorx2f(9625, -1350),
		Vectorx2f(9145, -1305)
	};

	Polygon fboundry3;
	fboundry3.vertices = {
		
		Vectorx2f(9640, -465),
		Vectorx2f(9640, 40),
		Vectorx2f(9550, 40),
		Vectorx2f(9550, -465)
	};

	Polygon fboundry4;
	fboundry4.vertices = {
		
		Vectorx2f(9620, 75),
		Vectorx2f(9340, 75),
		Vectorx2f(9340, 200),
		Vectorx2f(9620, 200)
	};

	Polygon fboundry5;
	fboundry5.vertices = {
		
		Vectorx2f(9225, 190),
		Vectorx2f(9225, 610),
		Vectorx2f(9640, 1375),
		Vectorx2f(9945, 1375),
		Vectorx2f(9945, 485)
		
	};

	Polygon fboundry6;
	fboundry6.vertices = {
		
		Vectorx2f(9720, 1425),
		Vectorx2f(3235, 4535)
		
	};

	Polygon fboundry7;
	fboundry7.vertices = {
		
		Vectorx2f(3235, 4480),
		Vectorx2f(510, 3115)
		
	};

	Polygon fboundry8;
	fboundry8.vertices = {
		
		Vectorx2f(610, 3070),
		Vectorx2f(1055, 2245)
		
	};

	Polygon fboundry9;
	fboundry9.vertices = {
		
		Vectorx2f(935, 2030),
		Vectorx2f(765, 1500)
		
	};

	Polygon fboundry10;
	fboundry10.vertices = {
		
		Vectorx2f(1410, -500),
		Vectorx2f(1405, -1095)
		
	};

	Polygon fboundry11;
	fboundry11.vertices = {
		
		Vectorx2f(1405, -1065),
		Vectorx2f(940, -1550)
		
	};

	Polygon fboundry12;
	fboundry12.vertices = {
		
		Vectorx2f(940, -1510),
		Vectorx2f(5690, -2790
)
		
	};

	Polygon fboundry13;
	fboundry13.vertices = {
		
		Vectorx2f(5690, -2830),
		Vectorx2f(6655, -2515)
		
	};

	Polygon fboundry14;
	fboundry14.vertices = {
		
		Vectorx2f(1035, 2180),
		Vectorx2f(865, 1500)
		
	};

	Polygon fboundry15;
	fboundry15.vertices = {
		
		Vectorx2f(860, 1490),
		Vectorx2f(1390, -440)
		
	};

	Polygon fhouseb1;
	fhouseb1.vertices = {
		Vectorx2f(3100, 3985),
		Vectorx2f(2440, 3585)
		
	};

	Polygon fhouseb2;
	fhouseb2.vertices = {
		Vectorx2f(2440, 3585),
		Vectorx2f(4205, 1670)
	};

	Polygon fhouseb3;
	fhouseb3.vertices = {
		Vectorx2f(4205, 1670),
		Vectorx2f(4575, 2505)
		
	};

	Polygon fhouseb4;
	fhouseb4.vertices = {
		Vectorx2f(4575, 2540),
		Vectorx2f(3100, 4025)
		
	};

	//Fjelenvar collision barriers

	Polygon fjelhouse1;
	fjelhouse1.vertices = {
		Vectorx2f(-30, 3775),
		Vectorx2f(1360, 1340)
	};

	Polygon fjelhouse2;
	fjelhouse2.vertices = {
		Vectorx2f(1360, 1340),
		Vectorx2f(-25, 200)
	};

	Polygon westbarrier;
	westbarrier.vertices = {
		Vectorx2f(-30, 6525),
		Vectorx2f(-30, -4370)
	};

	Polygon northwestbarrier1;
	northwestbarrier1.vertices = {
		Vectorx2f(-30, -2350),
		Vectorx2f(425, -3015)
	};

	Polygon tavernbarrier;
	tavernbarrier.vertices = {
		Vectorx2f(425, -3015),
		Vectorx2f(1635, -3365)
	};

	Polygon northwestbarrier2;
	northwestbarrier2.vertices = {
		Vectorx2f(1635, -3365),
		Vectorx2f(2205, -3630)
	};

	Polygon northbarrier1;
	northbarrier1.vertices = {
		Vectorx2f(2205, -3630),
		Vectorx2f(2455, -3750)
	};

	Polygon northbarrier2;
	northbarrier2.vertices = {
		Vectorx2f(2455, -3750),
		Vectorx2f(2950, -3795)
	};

	Polygon northbarrier3;
	northbarrier3.vertices = {
		Vectorx2f(2950, -3795),
		Vectorx2f(3040, -3830)
	};

	Polygon northbarrier4;
	northbarrier4.vertices = {
		Vectorx2f(3040, -3830),
		Vectorx2f(3220, -3810)
	};

	Polygon northbarrier5;
	northbarrier5.vertices = {
		Vectorx2f(3220, -3810),
		Vectorx2f(4065, -3940)
	};

	Polygon northbarrier6;
	northbarrier6.vertices = {
		Vectorx2f(4065, -3940),
		Vectorx2f(5120, -3830)
	};

	Polygon northbarrier7;
	northbarrier7.vertices = {
		Vectorx2f(5120, -3830),
		Vectorx2f(5265, -3855)
	};

	Polygon northbarrier8;
	northbarrier8.vertices = {
		Vectorx2f(5265, -3855),
		Vectorx2f(5625, -3810)
	};

	Polygon northbarrier9;
	northbarrier9.vertices = {
		Vectorx2f(5625, -3810),
		Vectorx2f(5825, -3845)
	};
	
	Polygon northbarrier10;
	northbarrier10.vertices = {
		Vectorx2f(5825, -3845),
		Vectorx2f(5975, -3980)
	};

	Polygon northbarrier11;
	northbarrier11.vertices = {
		Vectorx2f(5975, -3980),
		Vectorx2f(6995, -4035)
	};

	Polygon northbarrier12;
	northbarrier12.vertices = {
		Vectorx2f(6995, -4035),
		Vectorx2f(7665, -3410)
	};

	Polygon northbarrier13;
	northbarrier13.vertices = {
		Vectorx2f(8035, -3540),
		Vectorx2f(8495, -3540)
	};

	Polygon blacksmithshouse1;
	blacksmithshouse1.vertices = {
		Vectorx2f(7665, -3410),
		Vectorx2f(8130, -2620)
	};

	Polygon blacksmithshouse2;
	blacksmithshouse2.vertices = {
		Vectorx2f(8130, -2620),
		Vectorx2f(8405, -2695)
	};

	Polygon blacksmithshouse3;
	blacksmithshouse3.vertices = {
		Vectorx2f(8405, -2695),
		Vectorx2f(7890, -3565)
	};

	Polygon blacksmithshouse4;
	blacksmithshouse4.vertices = {
		Vectorx2f(7890, -3565),
		Vectorx2f(8035, -3540)
	};

	Polygon cliffside1;
	cliffside1.vertices = {
		Vectorx2f(8395, -3640),
		Vectorx2f(8535, -2720)
	};

	Polygon cliffside2;
	cliffside2.vertices = {
		Vectorx2f(8535, -2720),
		Vectorx2f(8470, -2405)
	};

	Polygon cliffside3;
	cliffside3.vertices = {
		Vectorx2f(8470, -2405),
		Vectorx2f(7885, -1795)
	};

	Polygon cliffside4;
	cliffside4.vertices = {
		Vectorx2f(7885, -1795),
		Vectorx2f(6975, -1125)
	};

	Polygon cliffside5;
	cliffside5.vertices = {
		Vectorx2f(6975, -1125),
		Vectorx2f(6850, -1085)
	};

	Polygon cliffside6;
	cliffside6.vertices = {
		Vectorx2f(6850, -1085),
		Vectorx2f(6215, -215)
	};

	Polygon cliffside7;
	cliffside7.vertices = {
		Vectorx2f(6215, -215),
		Vectorx2f(5735, 415)
	};

	Polygon cliffside8;
	cliffside8.vertices = {
		Vectorx2f(5220, 1355),
		Vectorx2f(4870, 1705)
	};

	Polygon cliffside9;
	cliffside9.vertices = {
		Vectorx2f(4870, 1705),
		Vectorx2f(4685, 1680)
	};

	Polygon cliffside10;
	cliffside10.vertices = {
		Vectorx2f(4685, 1680),
		Vectorx2f(4465, 2055)
	};

	Polygon cliffside11;
	cliffside11.vertices = {
		Vectorx2f(4465, 2055),
		Vectorx2f(4330, 2180)
	};

	Polygon cliffside12;
	cliffside12.vertices = {
		Vectorx2f(4330, 2180),
		Vectorx2f(4110, 2310)
	};

	Polygon cliffside13;
	cliffside13.vertices = {
		Vectorx2f(4110, 2310),
		Vectorx2f(3460, 3030)
	};

	Polygon cliffside14;
	cliffside14.vertices = {
		Vectorx2f(3460, 3030),
		Vectorx2f(3190, 3215)
	};

	Polygon cliffside15;
	cliffside15.vertices = {
		Vectorx2f(3190, 3215),
		Vectorx2f(3000, 3255)
	};

	Polygon cliffside16;
	cliffside16.vertices = {
		Vectorx2f(3000, 3255),
		Vectorx2f(2695, 3580)
	};

	Polygon cliffside17;
	cliffside17.vertices = {
		Vectorx2f(2695, 3580),
		Vectorx2f(2520, 3635)
	};

	Polygon cliffside18;
	cliffside18.vertices = {
		Vectorx2f(2520, 3635),
		Vectorx2f(2495, 3715)
	};

	Polygon cliffside19;
	cliffside19.vertices = {
		Vectorx2f(2495, 3715),
		Vectorx2f(2360, 3795)
	};

	Polygon cliffside20;
	cliffside20.vertices = {
		Vectorx2f(2360, 3795),
		Vectorx2f(1890, 3810)
	};

	Polygon cliffside21;
	cliffside20.vertices = {
		Vectorx2f(1890, 3810),
		Vectorx2f(1660, 3915)
	};

	Polygon cliffside22;
	cliffside22.vertices = {
		Vectorx2f(1660, 3915),
		Vectorx2f(305, 5120)
	};

	Polygon cliffside23;
	cliffside23.vertices = {
		Vectorx2f(305, 5120),
		Vectorx2f(-15, 5255)
	};

	Polygon cliffside24;
	cliffside24.vertices = {
		Vectorx2f(2310, 3780),
		Vectorx2f(1895, 3775)
	};

	Polygon lowercliffs1;
	lowercliffs1.vertices = {
		Vectorx2f(10315, 120),
		Vectorx2f(10010, 160)
	};

	Polygon lowercliffs2;
	lowercliffs2.vertices = {
		Vectorx2f(10010, 160),
		Vectorx2f(9910, 240)
	};

	Polygon lowercliffs3;
	lowercliffs3.vertices = {
		Vectorx2f(9910, 240),
		Vectorx2f(9890, 350)
	};

	Polygon lowercliffs4;
	lowercliffs4.vertices = {
		Vectorx2f(9890, 350),
		Vectorx2f(9290, 1045)
	};

	Polygon lowercliffs5;
	lowercliffs5.vertices = {
		Vectorx2f(9290, 1045),
		Vectorx2f(8875, 1350)
	};

	Polygon lowercliffs6;
	lowercliffs6.vertices = {
		Vectorx2f(8875, 1350),
		Vectorx2f(8830, 1335)
	};

	Polygon lowercliffs7;
	lowercliffs7.vertices = {
		Vectorx2f(8830, 1335),
		Vectorx2f(8415, 1675)
	};

	Polygon lowercliffs8;
	lowercliffs8.vertices = {
		Vectorx2f(8415, 1675),
		Vectorx2f(8445, 1770)
	};

	Polygon lowercliffs9;
	lowercliffs9.vertices = {
		Vectorx2f(8445, 1770),
		Vectorx2f(8140, 2015)
	};

	Polygon lowercliffs10;
	lowercliffs10.vertices = {
		Vectorx2f(8140, 2015),
		Vectorx2f(7910, 2115)
	};

	Polygon lowercliffs11;
	lowercliffs11.vertices = {
		Vectorx2f(7065, 2920),
		Vectorx2f(6905, 3035)
	};

	Polygon lowercliffs12;
	lowercliffs12.vertices = {
		Vectorx2f(6905, 3035),
		Vectorx2f(6630, 3470)
	};

	Polygon lowercliffs13;
	lowercliffs13.vertices = {
		Vectorx2f(6630, 3470),
		Vectorx2f(6535, 3435)
	};

	Polygon lowercliffs14;
	lowercliffs14.vertices = {
		Vectorx2f(6535, 3435),
		Vectorx2f(6385, 3965)
	};

	Polygon lowercliffs15;
	lowercliffs15.vertices = {
		Vectorx2f(6385, 3965),
		Vectorx2f(6430, 4090)
	};

	Polygon lowercliffs16;
	lowercliffs16.vertices = {
		Vectorx2f(6430, 4090),
		Vectorx2f(6230, 4360)
	};

	Polygon lowercliffs17;
	lowercliffs17.vertices = {
		Vectorx2f(6230, 4360),
		Vectorx2f(5635, 5540)
	};

	Polygon lowercliffs18;
	lowercliffs18.vertices = {
		Vectorx2f(5635, 5540),
		Vectorx2f(6025, 6005)
	};

	Polygon lowercliffs19;
	lowercliffs19.vertices = {
		Vectorx2f(6025, 6005),
		Vectorx2f(8645, 5865)
	};

	Polygon lowercliffs20;
	lowercliffs20.vertices = {
		Vectorx2f(8645, 5865),
		Vectorx2f(9140, 5695)
	};

	Polygon lowercliffs21;
	lowercliffs21.vertices = {
		Vectorx2f(9140, 5695),
		Vectorx2f(10205, 5990)
	};

	Polygon lowercliffs22;
	lowercliffs22.vertices = {
		Vectorx2f(10205, 5990),
		Vectorx2f(10665, 6020)
	};

	Polygon west1;
	west1.vertices = {
		Vectorx2f(10670, 6015),
		Vectorx2f(10670, 3895)
	};

	Polygon west2;
	west2.vertices = {
		Vectorx2f(10670, 3895),
		Vectorx2f(9860, 3820)
	};

	Polygon west3;
	west3.vertices = {
		Vectorx2f(9860, 3820),
		Vectorx2f(9725, 3385)
	};

	Polygon west4;
	west4.vertices = {
		Vectorx2f(9725, 3385),
		Vectorx2f(9860, 2620)
	};

	Polygon west5;
	west5.vertices = {
		Vectorx2f(9860, 2620),
		Vectorx2f(9960, 2305)
	};

	Polygon west6;
	west6.vertices = {
		Vectorx2f(9960, 2305),
		Vectorx2f(10290, 1950)
	};

	Polygon west7;
	west7.vertices = {
		Vectorx2f(10290, 1950),
		Vectorx2f(9455, 1075)
	};

	Polygon west8;
	west8.vertices = {
		Vectorx2f(9455, 1075),
		Vectorx2f(9750, 780)
	};

	Polygon west9;
	west9.vertices = {
		Vectorx2f(9750, 780),
		Vectorx2f(10245, 1275)
	};

	Polygon west10;
	west10.vertices = {
		Vectorx2f(10245, 1275),
		Vectorx2f(10245, 120)
	};

	Polygon bridge1;
	bridge1.vertices = {
		Vectorx2f(7100, 2900),
		Vectorx2f(6705, 2255)
	};

	Polygon bridge2;
	bridge2.vertices = {
		Vectorx2f(6705, 2255),
		Vectorx2f(6345, 1910)
	};

	Polygon bridge3;
	bridge3.vertices = {
		Vectorx2f(6345, 1910),
		Vectorx2f(5605, 1395)
	};

	Polygon bridge4;
	bridge4.vertices = {
		Vectorx2f(5605, 1395),
		Vectorx2f(5220, 1335)
	};

	Polygon bridge5;
	bridge5.vertices = {
		Vectorx2f(5655, 430),
		Vectorx2f(5855, 385)
	};

	Polygon bridge6;
	bridge6.vertices = {
		Vectorx2f(5855, 385),
		Vectorx2f(5975, 395)
	};

	Polygon bridge7;
	bridge7.vertices = {
		Vectorx2f(5975, 395),
		Vectorx2f(6350, 535)
	};

	Polygon bridge8;
	bridge8.vertices = {
		Vectorx2f(6350, 535),
		Vectorx2f(6795, 850)
	};

	Polygon bridge9;
	bridge9.vertices = {
		Vectorx2f(6795, 850),
		Vectorx2f(7280, 1310)
	};

	Polygon bridge10;
	bridge10.vertices = {
		Vectorx2f(7280, 1310),
		Vectorx2f(7665, 1760)
	};

	Polygon bridge11;
	bridge11.vertices = {
		Vectorx2f(7665, 1760),
		Vectorx2f(7975, 2240)
	};

	Polygon tree1;
	tree1.vertices = {
		Vectorx2f(6520, 5655),
		Vectorx2f(6520, 5955)
	};

	Polygon tree2;
	tree2.vertices = {
		Vectorx2f(6520, 5655),
		Vectorx2f(5685, 5655)
	};

	

	

	//wilderness1 barriers

	//tree1

	Polygon w1barrier1;
	w1barrier1.vertices = {
		Vectorx2f(1700, -1500),
		Vectorx2f(2065, -1300
)
	};

	Polygon w1barrier2;
	w1barrier2.vertices = {
		Vectorx2f(2065, -1300),
		Vectorx2f(2465, -1300)
	};

	Polygon w1barrier3;
	w1barrier3.vertices = {
		Vectorx2f(2465, -1300),
		Vectorx2f(2920, -1500)
	};

	Polygon w1barrier4;
	w1barrier4.vertices = {
		Vectorx2f(2920, -1500),
		Vectorx2f(1700, -1500)
	};

	//tree2

	Polygon w1barrier5;
	w1barrier5.vertices = {
		Vectorx2f(6885, -3385),
		Vectorx2f(7715, -3320)
	};

	Polygon w1barrier6;
	w1barrier6.vertices = {
		Vectorx2f(7715, -3320),
		Vectorx2f(7870, -3420)
	};

	Polygon w1barrier7;
	w1barrier7.vertices = {
		Vectorx2f(7870, -3420),
		Vectorx2f(7735, -3635)
	};

	Polygon w1barrier8;
	w1barrier8.vertices = {
		Vectorx2f(7735, -3635),
		Vectorx2f(6885, -3385)
	};

	//tree3

	Polygon w1barrier9;
	w1barrier9.vertices = {
		Vectorx2f(9775, -1595),
		Vectorx2f(9920, -1330)
	};

	Polygon w1barrier10;
	w1barrier10.vertices = {
		Vectorx2f(9920, -1330),
		Vectorx2f(10455, -1090)
	};

	Polygon w1barrier11;
	w1barrier11.vertices = {
		Vectorx2f(10455, -1090),
		Vectorx2f(10965, -1185)
	};

	Polygon w1barrier12;
	w1barrier12.vertices = {
		Vectorx2f(10965, -1185),
		Vectorx2f(10965, -1580)
	};

	Polygon w1barrier13;
	w1barrier13.vertices = {
		Vectorx2f(10965, 1580),
		Vectorx2f(9775, -1595)
	};

	//tree4

	Polygon w1barrier14;
	w1barrier14.vertices = {
		Vectorx2f(10315, 2685),
		Vectorx2f(10685, 2830)
	};

	Polygon w1barrier15;
	w1barrier15.vertices = {
		Vectorx2f(10685, 2830),
		Vectorx2f(10970, 2755)
	};

	Polygon w1barrier16;
	w1barrier16.vertices = {
		Vectorx2f(10970, 2755),
		Vectorx2f(10315, 2685)
	};

	//cliffside

	Polygon w1barrier17;
	w1barrier17.vertices = {
		Vectorx2f(2685, 665),
		Vectorx2f(3500, 890)
	};

	Polygon w1barrier18;
	w1barrier18.vertices = {
		Vectorx2f(3500, 890),
		Vectorx2f(3755, 845)
	};

	Polygon w1barrier19;
	w1barrier19.vertices = {
		Vectorx2f(3755, 845),
		Vectorx2f(4020, 645)
	};

	Polygon w1barrier20;
	w1barrier20.vertices = {
		Vectorx2f(4020, 645),
		Vectorx2f(4185, 420)
	};

	Polygon w1barrier21;
	w1barrier21.vertices = {
		Vectorx2f(4185, 420),
		Vectorx2f(5155, -335
)
	};

	Polygon w1barrier22;
	w1barrier22.vertices = {
		Vectorx2f(5605, -335),
		Vectorx2f(5345, -705)
	};

	Polygon w1barrier23;
	w1barrier23.vertices = {
		Vectorx2f(5345, -705),
		Vectorx2f(6230, -1030)
	};

	Polygon w1barrier24;
	w1barrier24.vertices = {
		Vectorx2f(6230, -1030),
		Vectorx2f(7180, -1275)
	};

	Polygon w1barrier25;
	w1barrier25.vertices = {
		Vectorx2f(7180, -1275),
		Vectorx2f(8825, -2475)
	};

	Polygon w1barrier26;
	w1barrier26.vertices = {
		Vectorx2f(8825, -2475),
		Vectorx2f(8410, -2665)
	};

	Polygon w1barrier27;
	w1barrier27.vertices = {
		Vectorx2f(8410, -2665),
		Vectorx2f(7380, -2100)
	};

	Polygon w1barrier28;
	w1barrier28.vertices = {
		Vectorx2f(7380, -2100),
		Vectorx2f(5320, -1715)
	};

	Polygon w1barrier29;
	w1barrier29.vertices = {
		Vectorx2f(5320, -1715),
		Vectorx2f(4810, -1255)
	};

	Polygon w1barrier30;
	w1barrier30.vertices = {
		Vectorx2f(4810, -1255),
		Vectorx2f(2765, 465)
	};

	Polygon w1barrier31;
	w1barrier31.vertices = {
		Vectorx2f(2765, 465),
		Vectorx2f(2685, 665)
	};

	//map bounds

	Polygon w1barrier32;
	w1barrier32.vertices = {
		Vectorx2f(-30, -4270),
		Vectorx2f(-30, 6730)
	};

	Polygon w1barrier33;
	w1barrier33.vertices = {
		Vectorx2f(-30, 6730),
		Vectorx2f(10965, 6730)
	};

	Polygon w1barrier34;
	w1barrier34.vertices = {
		Vectorx2f(10965, 6730),
		Vectorx2f(10965, -4270)
	};

	Polygon w1barrier35;
	w1barrier35.vertices = {
		Vectorx2f(10965, -4270),
		Vectorx2f(-30, -4270)
	};

	Polygon fjeltavernwall1;
	fjeltavernwall1.vertices = {
		Vectorx2f(300, 2415),
		Vectorx2f(2565, 1630)
	};







	srand(time(NULL));

	const unsigned WINDOW_WIDTH = 1400;
	const unsigned WINDOW_HEIGHT = 1400;
	const unsigned WINDOW_WIDTHX = 1400;
	const unsigned WINDOW_HEIGHTX = 800;
    bool walkright = 0;
	bool walkleft = 0;
    bool movediag1 = 0;
	bool movediag2 = 0;
    int playervelx = 0;
	int playervely = 0;
	int moveupleftdiagkey1 = 0;
	int moveupleftdiagkey2 = 0;
	int movedownleftdiagkey1 = 0;
	int movedownleftdiagkey2 = 0;
	int moveuprightdiagkey1 = 0;
	int moveuprightdiagkey2 = 0;
	int movedownrightdiagkey1 = 0;
	int movedownrightdiagkey2 = 0;
	int up = 1;
	int right = 2;
	int down = 3;
	int left = 4;
	int upleft = 5;
	int upright = 6;
	int downright = 7;
	int downleft = 8;
	int direction = 1;
    int directionx = 0;
	int idle = 1;
	bool isMoving = false;
	bool isMovingx = true;
	bool attacking = false;
	bool round = false;
	int repose = 0;
	float playerSpeed = 5.0f;
	int barrelhealth = 2;
	bool fireplace = true;

	
	
	//Vectors
	
	Vector2f playerCenter;
	Vector2f playerRight;
	Vector2f playerLeft;
	Vector2f enemyxCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;
	Vector2f loc;
	
	RenderWindow window(VideoMode(1400, 1400), "Shadows of Vaalundroth", Style::Default);
	window.setFramerateLimit(60);
	
    //Felstrond backdrop
    sf::Texture backgroundtxtr;
    backgroundtxtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/town1x.jpg");
    
    sf::Sprite background;
    background.setTexture(backgroundtxtr);
    background.setPosition(0, -4200);

	//Felstrond cave
	sf::Texture cavetxtr;
	cavetxtr.loadFromFile("/home/z3ta/c++/SoV/images/layers/fcaveoverlayer.png");

	sf::Sprite overhang;
	overhang.setTexture(cavetxtr);
	overhang.setPosition(9228, -560);

	//fjelenvår backdrop
    sf::Texture fjelenvartxtr;
    fjelenvartxtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/town2.jpg");
    
    sf::Sprite fjelenvar;
    fjelenvar.setTexture(fjelenvartxtr);
    fjelenvar.setPosition(0, -4200);

	//fjeltaverninn
	sf::Texture fjeltaverntxtr;
	fjeltaverntxtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/fjeltaverninn.png");
	sf::Sprite fjeltaverninn;
	fjeltaverninn.setTexture(fjeltaverntxtr);
	fjeltaverninn.setPosition(2700, 1700);
	fjeltaverninn.setRotation(71);

	//fjeltavernfireplace
	sf::IntRect fjelhearthSourceSprite(0, 0, 414, 155);
	sf::Texture fjelfireplacetxtr;
	fjelfireplacetxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/fireplacespritesheet.png");
	sf::Sprite fjelfireplace;
	fjelfireplace.setTexture(fjelfireplacetxtr);
	fjelfireplace.setPosition(2940, 2400);
	fjelfireplace.setRotation(71);

	//fjelhearthfirebox
	sf::Texture fjelfireboxtxtr;
	fjelfireboxtxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/fireplacefirebox.png");
	sf::Sprite fjelfirebox;
	fjelfirebox.setTexture(fjelfireboxtxtr);
	fjelfirebox.setPosition(2940, 2400);
	fjelfirebox.setRotation(71);
	
	//fhouse1
	sf::Texture fhouse1txtr;
	fhouse1txtr.loadFromFile("/home/z3ta/c++/SoV/images/layers/fhouse1top.png");

	sf::Sprite fhouse1x;
	fhouse1x.setTexture(fhouse1txtr);
	fhouse1x.setPosition(2439, 1367);

	//wilderness1 backdrop
	sf::Texture wilderness1txtr;
	wilderness1txtr.loadFromFile("/home/z3ta/c++/SoV/images/backgrounds/wilderness1.jpg");

	sf::Sprite wilderness1;
	wilderness1.setTexture(wilderness1txtr);
	wilderness1.setPosition(0, -4200);

	sf::Vector2f minimapPosition(playerCenter.x, playerCenter.y);
	sf::Vector2f xPosition(1500, 1500);
	sf::Vector2f xAdjust(104, 140);
	//Object Coordinates
	sf::Vector2f barrelPosition(1720, 990);
	sf::Vector2f barreltopPosition(720, 1936);
	//Building Coordinates
	sf::Vector2f tavernPosition(1000, -1000);
	sf::Vector2f tavernTopPosition(1000, -1000);
	sf::Vector2f whouseTopPosition(6500, 4184);
	sf::Vector2f wtowerTopPosition(8453, 1015);
	sf::Vector2f bhouseTopPosition(6732, -3775);
	sf::Vector2f fjeltreePosition(4995, 3455);
	sf::Vector2f fjeldoor1Position(905, -3473);

	//Player
	sf::IntRect rectSourceSprite(0, 0, 127, 127);
    sf::Texture playertxtr;
    playertxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/playerspritesheet.png");
	player.setTexture(playertxtr);
	player.setPosition(playerPosition);
	
	sf::IntRect barrelSourceSprite(0, 0, 71, 98);
	sf::IntRect xSprite(0, 0, 24, 19);
    
	sf::Sprite xcollision(playertxtr, xSprite);
	
	sf::Vector2f mapPosition(500, -200);
	sf::Texture collisionxtxtr;
	collisionxtxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/playerspritesheet.png");
	
    sf::Clock clock;
	sf::Clock clockx;
	sf::Clock fireclock;
	
	xcollision.setTexture(playertxtr);
	xcollision.setPosition(xPosition);
	sf::Vector2f sizeIncrease(1.0f, 1.0f);
	sf::Vector2f sizeDecrease(0.1f, 0.1f);
	player.setScale(sf::Vector2f(player.getScale().x + sizeIncrease.x, player.getScale().y + sizeIncrease.y));
	xcollision.setScale(sf::Vector2f(xcollision.getScale().x + sizeIncrease.x, player.getScale().y + sizeIncrease.y));
	
	
	//Barrels
	sf::Texture barreltxtr;
	barreltxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/barreldestructsequence1.png");
	sf::IntRect barrelxSourceSprite(0, 0, 100, 100);
	sf::Sprite barrel(barreltxtr, barrelxSourceSprite);
	barrel.setPosition(barrelPosition);
	


	sf::Texture barreltoptxtr;
	barreltoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/barrel1top.png");
	sf::Sprite barreltop(barreltoptxtr);
	barreltop.setPosition(barreltopPosition);

	//Buildings
	sf::Texture taverntxtr;
	taverntxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/longhousetavernsprite.png");
	sf::Sprite tavern(taverntxtr);
	tavern.setPosition(tavernPosition);

	//TavernTop
	sf::Texture taverntoptxtr;
	taverntoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/taverntop.png");
	sf::Sprite taverntop(taverntoptxtr);
	taverntop.setPosition(tavernTopPosition);

	//whousetop
	sf::Texture whousetoptxtr;
	whousetoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/whousetop.png");
	sf::Sprite whousetop(whousetoptxtr);
	whousetop.setPosition(whouseTopPosition);

	//wtowertop
	sf::Texture wtowertoptxtr;
	wtowertoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/wtowertop.png");
	sf::Sprite wtowertop(wtowertoptxtr);
	wtowertop.setPosition(wtowerTopPosition);

	//bhousetop
	sf::Texture bhousetoptxtr;
	bhousetoptxtr.loadFromFile("/home/z3ta/c++/SoV/images/buildings/bhousetop.png");
	sf::Sprite bhousetop(bhousetoptxtr);
	bhousetop.setPosition(bhouseTopPosition);

	//fjeltree
	sf::Texture fjeltreetxtr;
	fjeltreetxtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/fjeltreex.png");
	sf::Sprite fjeltree(fjeltreetxtr);
	fjeltree.setPosition(fjeltreePosition);

	//fjeltaverndoor
	sf::IntRect door1SourceSprite(0, 0, 338, 281);
	sf::Texture door1txtr;
	door1txtr.loadFromFile("/home/z3ta/c++/SoV/images/sprites/doorspritesheet1.png");
	sf::Sprite taverndoor(door1txtr, door1SourceSprite);
	taverndoor.setPosition(fjeldoor1Position);
	NPC npc(660.f, 2480.f, font, window);
	bool isPlayerFrozen = false;
	while (window.isOpen())
	{
		Event event;
		Polygon playerOriginal = collisionx;
		Polygon attackxOriginal = attackx;
		sf::Vector2f playerPos = player.getPosition();  // Get the player's position
		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
		
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			
			npc.handleInput(event);
		}

		if (hasPlayed == false){
			playIntroMovie(window);
		}
        
		//Directional rotation
        if (sf::Keyboard::isKeyPressed(Keyboard::W) && playerHealth > 0)
		{
			direction = 5;
			idle = 0;
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::D) && playerHealth > 0)
		{
			direction = 7;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::S) && playerHealth > 0)
		{
			direction = 1;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::A) && playerHealth > 0)
		{
			direction = 3;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::W) && (Keyboard::isKeyPressed(sf::Keyboard::A)) && playerHealth > 0)
		{
			direction = 4;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::W) && (sf::Keyboard::isKeyPressed(Keyboard::D)) && playerHealth > 0)
		{
			direction = 6;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::D) && (sf::Keyboard::isKeyPressed(Keyboard::S)) && playerHealth > 0)
		{
			direction = 8;
			idle = 0;
			
		}
		
		if (sf::Keyboard::isKeyPressed(Keyboard::S) && (sf::Keyboard::isKeyPressed(Keyboard::A)) && playerHealth > 0)
		{
			direction = 2;
			idle = 0;
			
		}

		//Death animations

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 1){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3048;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 2){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3175;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 3){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3302;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 4){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3429;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 5){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3556;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 6){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3683;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 7){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3810;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (clock.getElapsedTime().asSeconds() > 0.1 && playerHealth <= 0 && direction == 8){
			idle = 1;
			playerSpeed = 0;
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 3937;
			
			if (rectSourceSprite.left >= 2413){
				rectSourceSprite.left = 2413;
			}

			else
				rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		//Walking animations
		
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 1 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2032;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
        if (clock.getElapsedTime().asSeconds() > 0.1f && direction == 2 && idle == 0)
        {
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2159;
			isMoving = true;
            if (rectSourceSprite.left >= 700)
                rectSourceSprite.left = 0;
            else
                rectSourceSprite.left += 127;
            
            player.setTextureRect(rectSourceSprite);
            clock.restart();
        }
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 3 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2286;
			isMoving = true;
            if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else 
			    rectSourceSprite.left += 127;
		
		    player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1f && direction == 4 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2413;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 5 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2540;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 6 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2667;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 7 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2794;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		if (clock.getElapsedTime().asSeconds() > 0.1 && direction == 8 && idle == 0)
		{
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 2921;
			isMoving = true;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}
		

		//Idle animations
		if (event.type == sf::Event::KeyReleased)
		{
			if(event.key.code == sf::Keyboard::W)
			{
				
				isMoving = false;
			}

			if(event.key.code == sf::Keyboard::A)
			{
				
				isMoving = false;
			}

			if(event.key.code == sf::Keyboard::S)
			{
				
				isMoving = false;
			}

			if(event.key.code == sf::Keyboard::D)
			{
				
				isMoving = false;
				
			}

		if (idle == 1 && direction == 1 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1016;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 2 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1143;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 3 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1270;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 4 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1397;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 5 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1524;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 6 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1651;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 7 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1778;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		if (idle == 1 && direction == 8 && isMoving == false && playerHealth > 0){
			rectSourceSprite.height = 127;
			rectSourceSprite.top = 1905;
			rectSourceSprite.left = 0;
			isMoving = false;
			if (rectSourceSprite.left >= 700)
			    rectSourceSprite.left = 0;
			else
			    rectSourceSprite.left += 127;
			player.setTextureRect(rectSourceSprite);
			clock.restart();
		}

		}

		//Attack animations
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && clock.getElapsedTime().asSeconds() > 0.1)
        {
			
			std::cout << "PositionX: " << playerCenter.x << std::endl;
			std::cout << "PositionY: " << playerCenter.y << std::endl;
			if (direction == 1)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 0;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
					
				}
			    else
			        rectSourceSprite.left += 127;

				if (rectSourceSprite.left == 0 && round == true){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
					
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 2)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 127;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 3)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 254;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 4)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 381;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 5)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 508;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 6)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 635;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 7)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 762;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			if (direction == 8)
			{
				rectSourceSprite.height = 127;
			    rectSourceSprite.top = 889;
			    if (rectSourceSprite.left >= 700){
					
			        rectSourceSprite.left = 0;
					round = true;
				}
			    else
			        rectSourceSprite.left += 127;
				if (rectSourceSprite.left == 0){
					attacking = true;
					playerAttackRoll = generateRandomNumber();
					std::cout << "Player Attack Role: " << playerAttackRoll << std::endl;
					if (playerAttackRoll >= 17){
						if (area == 3){
							enemy.updatehealth(playerCenter.x, playerCenter.y);
							enemy1.updatehealth(playerCenter.x, playerCenter.y);
						}
					}
				}
				else
					attacking = false;
			    player.setTextureRect(rectSourceSprite);
			    clock.restart();
			}
			
	    
        }

		if (fireplace && fireclock.getElapsedTime().asSeconds() > 0.2){
			if (fjelhearthSourceSprite.left >= 2478){
				fjelhearthSourceSprite.left = 0;
			}

			else{
				fjelhearthSourceSprite.left += 414;
			}
			fjelfireplace.setTextureRect(fjelhearthSourceSprite);
			fireclock.restart();
		}

		if (barrelhealth <= 0 && clock.getElapsedTime().asSeconds() > 0.1)
		{
			
			barrelxSourceSprite.left += 100;
			if (barrelxSourceSprite.left >= 400){
				barrelxSourceSprite.left = 400;
			}
			barrel.setTextureRect(barrelxSourceSprite);
			clock.restart();
		}

		//Vectors
		playerCenter = Vector2f(player.getPosition());
		playerRight = Vector2f(player.getPosition());
		playerLeft = Vector2f(player.getPosition());
		
		mousePosWindow = Vector2f(Mouse::getPosition(window));

		aimDir = mousePosWindow - playerCenter;
		float aimDirMagnitude = sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
		
        if (aimDirMagnitude != 0) 
		{
            aimDirNorm = aimDir / aimDirMagnitude;
        } 
		
		else 
		
		{
            aimDirNorm = aimDir; // Handle division by zero or very small magnitude case
        }

		float PI = 3.14159265f;
		float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI;
		//player.setRotation(deg + 90);

		//Notes
		const auto prevPos = player.getPosition();

		// Move player with arrow keys
        if (Keyboard::isKeyPressed(Keyboard::A) && !isPlayerFrozen) {
			player.move(-playerSpeed, 0.f);
        }
        if (Keyboard::isKeyPressed(Keyboard::D) && !isPlayerFrozen) {
			player.move(playerSpeed, 0.f);
        }
        if (Keyboard::isKeyPressed(Keyboard::W) && !isPlayerFrozen) {
			player.move(0.f, -playerSpeed);
        }
        if (Keyboard::isKeyPressed(Keyboard::S) && !isPlayerFrozen) {
			player.move(0.f, playerSpeed);
        }

		else{
			idle = 1;
		}

		// Adjust the offset based on your player's height and desired collision position
		float collisionOffsetY = 100.0f; // Adjust this value to move the collision box down
		Vectorx2f adjustedPosition = Vectorx2f::fromSF(player.getPosition());
		adjustedPosition.y += collisionOffsetY; // Move the collision box down
		adjustedPosition.x += collisionOffsetY;
		// Set the collision position to the adjusted position
		collisionx.setPosition(adjustedPosition);
		

		
		
		// Check for collision
        Vectorx2f collisionNormal;

		if (checkPolygonCollision(fboundry1, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry2, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry3, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry4, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry5, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry6, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry7, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry8, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry9, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry10, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry11, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry12, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry13, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry14, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fboundry15, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fhouseb1, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fhouseb2, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fhouseb3, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fhouseb4, collisionx, collisionNormal) && area == 1) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		//fjelenvar

		if (checkPolygonCollision(fjelhouse1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjelhouse2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(westbarrier, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northwestbarrier1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(tavernbarrier, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northwestbarrier2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier3, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier4, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier5, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier6, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier7, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier8, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier9, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier10, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier11, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier12, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(northbarrier13, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(blacksmithshouse1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(blacksmithshouse2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(blacksmithshouse3, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(blacksmithshouse4, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside3, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside4, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside5, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside6, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside7, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside8, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside9, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside10, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside11, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside12, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside13, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside14, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside15, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside16, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside17, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside18, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside19, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside20, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside21, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside22, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside23, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(cliffside24, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs3, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs4, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs5, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs6, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs7, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs8, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs9, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs10, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs11, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }
		
		if (checkPolygonCollision(lowercliffs12, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs13, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs14, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs15, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs16, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs17, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs18, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs19, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs20, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs21, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(lowercliffs22, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(west1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(west2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(west3, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(west4, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(west5, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(west6, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }
		
		if (checkPolygonCollision(west7, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(west8, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(west9, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(west10, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge3, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge4, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge5, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge6, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge7, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge8, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge9, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge10, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(bridge11, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(tree1, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(tree2, collisionx, collisionNormal) && area == 2) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		//wilderness1 collision

		if (checkPolygonCollision(w1barrier1, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier2, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier3, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier4, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier5, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier6, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier7, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier8, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier9, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier10, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier11, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier12, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier13, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier14, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier15, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier16, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier17, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier18, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier19, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier20, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier21, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier22, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier23, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier24, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier25, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier26, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier27, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier28, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier29, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier30, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier31, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier32, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier33, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier34, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(w1barrier35, collisionx, collisionNormal) && area == 3) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }

		if (checkPolygonCollision(fjeltavernwall1, collisionx, collisionNormal) && area == 4) {
            // If a collision is detected, revert to the original position
			
            collisionx = playerOriginal; // Reset player position
			attackx = attackxOriginal;
			player.setPosition(prevPos);
            resolveCollision(collisionx, collisionNormal); // Adjust player position based on collision normal
        }



		

		

		

		if (checkPolygonCollision(barrelx, attackx, collisionNormal) && attacking == true && round == true && area == 1) {
            // If an attack is detected, deal damage to the barrel
			area = 2;
            barrelhealth -= 1;
			std::cout << "The value of x is: " << barrelhealth << std::endl;
			std::cout << "The value of z is: " << barrelxSourceSprite.left << std::endl;
			round = false;
        }
		
		// Draw the player polygon
        sf::ConvexShape playerShape;
        playerShape.setPointCount(collisionx.vertices.size());
        for (size_t i = 0; i < collisionx.vertices.size(); ++i) {
            playerShape.setPoint(i, sf::Vector2f(collisionx.vertices[i].x, collisionx.vertices[i].y));
        }
		
        playerShape.setFillColor(sf::Color::Green);

		// Draw the boundry polygon
        sf::ConvexShape boundry1Shape;
        boundry1Shape.setPointCount(fboundry1.vertices.size());
        for (size_t i = 0; i < fboundry1.vertices.size(); ++i) {
            boundry1Shape.setPoint(i, sf::Vector2f(fboundry1.vertices[i].x, fboundry1.vertices[i].y));
        }

		boundry1Shape.setFillColor(sf::Color::Blue);

		sf::ConvexShape boundry2Shape;
        boundry2Shape.setPointCount(fboundry2.vertices.size());
        for (size_t i = 0; i < fboundry2.vertices.size(); ++i) {
            boundry2Shape.setPoint(i, sf::Vector2f(fboundry2.vertices[i].x, fboundry2.vertices[i].y));
        }
		
		boundry2Shape.setFillColor(sf::Color::Blue);

		sf::ConvexShape boundry4Shape;
        boundry4Shape.setPointCount(fboundry4.vertices.size());
        for (size_t i = 0; i < fboundry4.vertices.size(); ++i) {
            boundry4Shape.setPoint(i, sf::Vector2f(fboundry4.vertices[i].x, fboundry4.vertices[i].y));
        }
		
		boundry4Shape.setFillColor(sf::Color::Blue);
		
		// Draw the attack range polygon
		sf::ConvexShape attackrangeShape;
        attackrangeShape.setPointCount(attackx.vertices.size());
        for (size_t i = 0; i < attackx.vertices.size(); ++i) {
            attackrangeShape.setPoint(i, sf::Vector2f(attackx.vertices[i].x, attackx.vertices[i].y));
        }
		
        attackrangeShape.setFillColor(sf::Color::Blue);

		//Draw
        sf::View view;
		view.setCenter(player.getPosition());
		window.setView(view);
		
		// Draw the building polygon
        
		 // Get time elapsed since last frame
        float deltaTime = clockx.getElapsedTime().asSeconds();
		
		// Draw the barrelx polygon
		sf::ConvexShape barrelxShape;
		barrelxShape.setPointCount(barrelx.vertices.size());
		for (size_t i = 0; i < barrelx.vertices.size(); ++i) {
			barrelxShape.setPoint(i, sf::Vector2f(barrelx.vertices[i].x, barrelx.vertices[i].y));
		}
		barrelxShape.setFillColor(sf::Color::Red);
		
		window.clear();
		
		mousePos = sf::Mouse::getPosition(window);

		if (area == 1){
        	window.draw(background);

			window.draw(boundry1Shape);
		
			window.draw(barrel);

			window.draw(player);
			window.draw(fhouse1x);
			window.draw(overhang);
			window.draw(boundry4Shape);
			minimap1.draw(window);
		
		}

		if (area == 2){
        	window.draw(fjelenvar);
			fjeltaverndoor.render(window);
			fjeltaverndoor.update(mousePos, window, playerPos);
			halfgiantsdoor.render(window);
			halfgiantsdoor.update(mousePos, window, playerPos);
			smithdoor.render(window);
			smithdoor.update(mousePos, window, playerPos);
			herbalistdoor.render(window);
			herbalistdoor.update(mousePos, window, playerPos);
			
			window.draw(player);
			window.draw(fjeltree);
			minimap2.draw(window);
		}

		if (area == 3){
			window.draw(wilderness1);
            window.draw(enemy.getSprite());
			window.draw(enemy1.getSprite());
			window.draw(player);
			minimap3.draw(window);
			enemy.update(player.getPosition().x, player.getPosition().y, deltaTime);
			enemy1.update(player.getPosition().x, player.getPosition().y, deltaTime);
			
		}

		if (area == 4){
			window.draw(fjeltaverninn);
			window.draw(fjelfireplace);
			window.draw(fjelfirebox);
			window.draw(player);
			npc.update(playerPosition, mousePos);  // Update NPC based on player position and mouse position
			npc.handleInput(event);  // Handle mouse input for the conversation
			npc.draw(window);
		}
		
		minimap1.update(sf::Vector2f(playerCenter - mapPosition), sf::Vector2f(window.getSize()));
		minimap2.update(sf::Vector2f(playerCenter - mapPosition), sf::Vector2f(window.getSize()));
		minimap3.update(sf::Vector2f(playerCenter - mapPosition), sf::Vector2f(window.getSize()));
		
		window.display();
		

	}

	return 0;
}
