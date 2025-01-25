#ifndef DOOR_H
#define DOOR_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

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

// Second door class
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
            bool isHovering = pointInPolygon(doorPolygon, worldMousePos);

            if (isHovering) {
                doorPolygon.setOutlineColor(sf::Color::Blue);

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isClicked) {
                    toggleDoorState();
                    isClicked = true;
                }
            } else {
                doorPolygon.setOutlineColor(sf::Color::Transparent);
            }
        } else {
            doorPolygon.setOutlineColor(sf::Color::Transparent);
        }

        // Reset click flag when mouse button is released
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            isClicked = false;
        }
    }

    void render(sf::RenderWindow& window) {
        window.draw(doorSprite);     // Draw the door sprite
        window.draw(doorPolygon);    // Draw the clickable polygon
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
            x1 = x2; y1 = y2;
        }
        return inside;
    }

    void toggleDoorState() {
        if (doorOpen) {
            doorSprite.setTextureRect(sf::IntRect(0, 0, 570, 1732));
        } else {
            doorSprite.setTextureRect(sf::IntRect(570, 0, 570, 1732));
        }
        doorOpen = !doorOpen;  // Toggle door state
    }

private:
    sf::Texture doorTexture;
    sf::Sprite doorSprite;
    sf::ConvexShape doorPolygon;
    bool doorOpen;
    bool isClicked;
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

#endif // DOOR_H